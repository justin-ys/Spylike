#include "character.h"
#include "rendering.h"
#include "logger.h"
#include "event.h"
#include <algorithm>
#include <memory>

extern SpylikeLogger LOGGER;

void Player::on_init() {
	eventManager->subscribe(shared_from_this(), "INPUT_KeyPress");
}

void Player::on_event(Event& e) {
	if (e.type == "INPUT_KeyPress") {
		SpylikeEvents::KeyInputEvent& ke = dynamic_cast<SpylikeEvents::KeyInputEvent&>(e);
		Coordinate pos = getPos();
		if (ke.c == 'w' || ke.c == 'a' || ke.c == 's' || ke.c == 'd') {
			Coordinate newPos = pos;
			SpylikeEvents::CameraEvent ce("CAMERA_MOVE");
			switch(ke.c) {
				case ('w'): {
					newPos.y--;
					ce = SpylikeEvents::CameraEvent("CAMERA_MoveUp");
					break;
				}
				case ('a'): {
					newPos.x--;
					ce = SpylikeEvents::CameraEvent("CAMERA_MoveLeft");
					break;
				}
				case ('s'): {
					newPos.y++;
					ce = SpylikeEvents::CameraEvent("CAMERA_MoveDown");
					break;
				}
				case ('d'): {
					newPos.x++;
					ce = SpylikeEvents::CameraEvent("CAMERA_MoveRight");
				}
			}
			if (world->isInMap(newPos)) {
				bool res = world->moveEntity(getID(), newPos);
				if (res) eventManager->emit(ce);
			}
		}
	}
}

void Player::draw(GeometryRenderer& painter) {
	if (state == PState::Hurt) { 
		painter.drawString(getPos(), hurtSprite.getCurrentFrame(), "Entity");
		hurtSprite.nextFrame();
	}
	else {
		painter.draw(getPos(), '@', "Entity");
	}
}

void Player::on_update() {
	switch(state) {
		case(PState::Idle): { break; }
		case(PState::Hurt): {
			if (health == 0) {
				Event playerDeath = Event("GAME_PlayerDeath");
				eventManager->emit(playerDeath);
			}
			hurtTimer.tick();
			if (hurtTimer.getElapsed() == 15) state = PState::Idle;
			break;
		}
	}
}

void Player::on_collide(std::shared_ptr<TileEntity> collider) {}

void Player::hurt(int damage) {
	if (state != PState::Hurt) {
		health -= damage;
		state = PState::Hurt;
		hurtTimer.reset();
	}
}

void Goblin::on_update() {
	switch(state) {
		case (GobState::Idle): {
			std::vector<std::shared_ptr<Player>> res = world->findEntities<Player>(getPos(), 15);
			if (res.size() > 0) {
				state = GobState::Found;
				seekTimer.reset();
			}
			break;
		}
		case (GobState::Found): {
			seekTimer.tick();
			if (seekTimer.getElapsed() > 12) {
				state = GobState::Pursue;
				seekTimer.reset();
			}
			break;
		}
		case (GobState::Pursue): {
			if (seekTimer.getElapsed() >= 12) {
				seekTimer.reset();
				std::vector<std::shared_ptr<Player>> res = world->findEntities<Player>(getPos(), 15);
				if (res.size() > 0) {
					std::shared_ptr<Player> player = res[0];
					int yDir;
					int xDir;
					if (player->getPos().y < getPos().y) yDir = -1;
					else yDir = 1;
					if (player->getPos().x < getPos().x) xDir = -1;
					else xDir = 1;
					Coordinate newPos(getPos().x + xDir, getPos().y + yDir);
					if (world->isInMap(newPos)) world->moveEntity(getID(), newPos);
				}
				else state = GobState::Idle;
			}
			seekTimer.tick();
		}
	}
}
			
void Goblin::draw(GeometryRenderer& painter) {
	if (state == GobState::Found) {
		painter.draw(Coordinate(getPos().x, getPos().y-1), '!', "Effect");
	}
	painter.draw(getPos(), '$', "Entity");
}

void Goblin::on_collide(std::shared_ptr<TileEntity> collider) {
	std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(collider);
	if (player) {
		player->hurt(10);
	}
}

void Goblin::hurt(int damage) {
	health -= damage;
}
