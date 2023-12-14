#include "character.h"
#include "rendering.h"
#include "logger.h"
#include "event.h"
#include <algorithm>
#include <memory>
#include <stdlib.h>

extern SpylikeLogger LOGGER;

void Player::on_init() {
	eventManager->subscribe(shared_from_this(), "INPUT_KeyPress");
	SpylikeEvents::CameraEvent ce("CAMERA_Move", getPos());
	eventManager->emit(ce);
}

void Player::on_event(Event& e) {
	if (e.type == "INPUT_KeyPress") {
		SpylikeEvents::KeyInputEvent& ke = dynamic_cast<SpylikeEvents::KeyInputEvent&>(e);
		Coordinate pos = getPos();
		if (world->worldType == WorldType::Roguelike) {
			if (ke.c == 'w' || ke.c == 'a' || ke.c == 's' || ke.c == 'd') {
				Coordinate newPos = pos;
				switch(ke.c) {
					case ('w'): {
						newPos.y--;
						break;
					}
					case ('a'): {
						newPos.x--;
						break;
					}
					case ('s'): {
						newPos.y++;
						break;
					}
					case ('d'): {
						newPos.x++;
					}
				}
				if (world->isInMap(newPos)) {
					bool res = world->moveEntity(getID(), newPos);
					if (res) {
						SpylikeEvents::CameraEvent ce("CAMERA_Move", newPos);
						eventManager->emit(ce);
					}
				}
			}
		}
		else {
			if (ke.c == 'a' || ke.c == 'd') {
				Coordinate newPos = pos;
				SpylikeEvents::CameraEvent ce("CAMERA_MOVE");
				switch(ke.c) {
					case ('a'): {
						newPos.x--;
						ce = SpylikeEvents::CameraEvent("CAMERA_MoveLeft");
						xVel = -1;
						break;
					}
					case ('d'): {
						newPos.x++;
						xVel = 1;
						ce = SpylikeEvents::CameraEvent("CAMERA_MoveRight");
					}
				}
				if (world->isInMap(newPos)) {
					bool res = world->moveEntity(getID(), newPos);
					if (res) {
						eventManager->emit(ce);
						slideTimer.reset();
					}
				}
			}
			else if (ke.c == 'w' || ke.c == ' ' && (yVel == 0)) {
				Coordinate below = Coordinate(pos.x, pos.y+1);
				bool res = world->moveEntity(getID(), below);
				if (!res) {
					world->moveEntity(getID(), pos);
					yVel = 6;
					moveTimer.reset();
				}
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
	if (world->worldType == WorldType::Platform) {
		moveTimer.tick();
		slideTimer.tick();
		if (yVel > 0 && (moveTimer.getElapsed() > 3/yVel)) {
			Coordinate newPos = Coordinate(getPos().x+xVel, getPos().y-1);
			if (world->isInMap(newPos)) {
				bool res = world->moveEntity(getID(), newPos);
				if (res) yVel--;
			}
			else yVel = 0;
			slideTimer.reset();
			moveTimer.reset();
		}
		else {
			if (moveTimer.getElapsed() > 2) {
				Coordinate below = Coordinate(getPos().x, getPos().y+1);
				if (world->isInMap(below)) {
					bool res = world->moveEntity(getID(), below);
					if (res && slideTimer.getElapsed() > 2) {
						Coordinate newPos = Coordinate(below.x+xVel, below.y);
						bool res = world->moveEntity(getID(), newPos);
						if (!res) xVel = 0;
						slideTimer.reset();
					}
				}
				moveTimer.reset();
			}
			if (slideTimer.getElapsed() > 5) {
				xVel = 0;
			}
		}
	}
		
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

void Player::on_collide(std::shared_ptr<TileEntity> collider) {
	if (collider->getPos().y < getPos().y) yVel = 0;
}

void Player::hurt(int damage) {
	if (state != PState::Hurt) {
		health -= damage;
		SpylikeEvents::PlayerHurtEvent ev("GAME_PlayerHurt", health);
		eventManager->emit(ev);
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
			if (seekTimer.getElapsed() > 8) {
				state = GobState::Pursue;
				seekTimer.reset();
			}
			break;
		}
		case (GobState::Pursue): {
			if (seekTimer.getElapsed() >= 7) {
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

void Skeleton::draw(GeometryRenderer& painter) {
	painter.draw(getPos(), '&', "Entity");
}

void Skeleton::on_collide(std::shared_ptr<TileEntity> collider) {
	std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(collider);
	if (player) {
		player->hurt(10);
	}
}


void Skeleton::on_update() {
	fireTimer.tick();
	if (fireTimer.getElapsed() > 20) {
		std::vector<std::shared_ptr<Player>> res = world->findEntities<Player>(getPos(), 30);
		if (res.size() > 0) {
			std::shared_ptr<Player> player = res[0];
			int xVel = 0;
			if (player->getPos().x > getPos().x) xVel = 100;
			else if (player->getPos().x < getPos().x) xVel = -100;
			int yVel = 0;
			if (player->getPos().y != getPos().y) {
				int yDir = abs(player->getPos().y - getPos().y)/(player->getPos().y - getPos().y);
				if (player->getPos().x != getPos().x) yVel = yDir*abs((xVel*(player->getPos().y - getPos().y))/(player->getPos().x - getPos().x));
				else yVel = yDir;
			}
			std::shared_ptr<SkeletonArrow> arrow = std::make_shared<SkeletonArrow>(xVel, yVel);
			arrow->init(eventManager);
			world->registerEntity(arrow, Coordinate(getPos().x-1, getPos().y));
		}
		fireTimer.reset();
	}
}

void Skeleton::hurt(int damage) {
	health -= damage;
}

void SkeletonArrow::draw(GeometryRenderer& painter) {
	painter.draw(getPos(), '#', "Entity");
}

void SkeletonArrow::on_update() {
	moveTimer.tick();
	if (moveTimer.getElapsed() > 2) {	
		moveTimer.reset();
		xFlag += xVel;
		yFlag += yVel;
		Coordinate newPos = getPos();
		if (abs(xFlag) >= 100) {
			newPos.x = newPos.x + abs(xVel)/xVel;
			xFlag = 0;
		}
		if (abs(yFlag) >= 100) {
			newPos.y = newPos.y + abs(yVel)/yVel;
			yFlag = 0;
		}
		if (newPos != getPos()) {
			if (world->isInMap(newPos)) world->moveEntity(getID(), newPos);
			else kill();
		}
	}
}

void SkeletonArrow::on_collide(std::shared_ptr<TileEntity> collider) {
	std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(collider);
	if (player) {
		player->hurt(10);
	}
	kill();
}

