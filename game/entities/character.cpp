#include "character.h"
#include "rendering.h"
#include "logger.h"
#include "event.h"
#include "misc.h"
#include <algorithm>
#include <memory>
#include <stdlib.h>
#include <cmath>

extern SpylikeLogger LOGGER;

void Player::on_init() {
	eventManager->subscribe(shared_from_this(), "INPUT_KeyPress");
	SpylikeEvents::CameraEvent ce("CAMERA_MoveCenter", getPos());
	eventManager->emit(ce);
}

void Player::on_event(Event& e) {
	if (e.type == "INPUT_KeyPress") {
		SpylikeEvents::KeyInputEvent& ke = dynamic_cast<SpylikeEvents::KeyInputEvent&>(e);
		Coordinate pos = getPos();
		if (state == PState::Idle && (ke.c == 'v' || ke.c == 'b' || ke.c == 'n' || ke.c == 'g')) {
			Coordinate attackPos = pos;
			switch(ke.c) {
				case ('g'): {
					attackPos.y--;
					break;
				}
				case ('v'): {
					attackPos.x--;
					break;
				}
				case ('b'): {
					attackPos.y++;
					break;
				}
				case ('n'): {
					attackPos.x++;
				}
			}
			if (world->isInMap(attackPos)) {
				std::shared_ptr<Tile> attackTile = world->getTile(attackPos);
				if (attackTile) {
					for (auto& entity : attackTile->getEntities()) {
							std::shared_ptr<Character> character = std::dynamic_pointer_cast<Character>(entity);
							if (character) {
								character->hurt(20);
								std::shared_ptr<Character> boss = std::dynamic_pointer_cast<Boss>(entity);
								if (!boss && character->isAlive()) {
									Coordinate newCharPos = character->getPos();
									if (getPos().x < newCharPos.x) newCharPos.x++;
									else if (getPos().x > newCharPos.x) newCharPos.x--;
									else if (getPos().y > newCharPos.y) newCharPos.y--;
									else newCharPos.y++;
									world->moveEntity(character->getID(), newCharPos);
									if (world->worldType == WorldType::Platform && ke.c == 'b') {
										yVel = 3;
										moveTimer.reset();
									}
								}
							}
					}
				}
				state = PState::Attacking;
				attackLoc = attackPos;
				attackTimer.reset();
			}
		}
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
				world->moveEntity(getID(), newPos);
			}
		}
		else {
			if (ke.c == 'a' || ke.c == 'd') {
				Coordinate newPos = pos;
				switch(ke.c) {
					case ('a'): {
						newPos.x--;
						xVel = -1;
						break;
					}
					case ('d'): {
						newPos.x++;
						xVel = 1;
					}
				}
				bool res = world->moveEntity(getID(), newPos);
				if (res) {
					slideTimer.reset();
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

void Player::draw(Camera& painter) {
	if (world->worldType == WorldType::Platform) {
		int yDist = getPos().y - painter.getOrigin().y;
		nextCamPos = Coordinate(getPos().x - painter.getScreenWidth()/2, painter.getOrigin().y);
		changePosFlag = true;
		if (yDist > 3*(painter.getScreenHeight()/4)) {
			nextCamPos.y = painter.getOrigin().y + painter.getScreenHeight()/4;
		}
		else if (yDist < painter.getScreenHeight()/4) {
			nextCamPos.y = painter.getOrigin().y - painter.getScreenHeight()/4;
		}
	}
	if (state == PState::Hurt) { 
		painter.drawString(getPos(), hurtSprite.getCurrentFrame(), "Entity");
		hurtSprite.nextFrame();
	}
	else {
		if (state == PState::Attacking) {
			if (attackLoc.x != getPos().x) painter.draw(attackLoc, '-', "Effect");
			else painter.draw(attackLoc, '|', "Effect");
		}
		painter.draw(getPos(), '@', "Entity");
	}
}

void Player::on_update() {
	if (world->worldType == WorldType::Roguelike) {
		SpylikeEvents::CameraEvent ce("CAMERA_MoveCenter", getPos());
		eventManager->emit(ce);
	}
	else if (world->worldType == WorldType::Platform) {
		if (changePosFlag && world->active) {
			SpylikeEvents::CameraEvent ce("CAMERA_Move", nextCamPos);
			eventManager->emit(ce);
			changePosFlag = false;
		}
		moveTimer.tick();
		slideTimer.tick();
		if (yVel > 0 && (moveTimer.getElapsed() > 3/yVel)) {
			Coordinate newPos = Coordinate(getPos().x+xVel, getPos().y-1);
			bool res = world->moveEntity(getID(), newPos);
			if (res) {
				yVel--;
			}
			else yVel = 0;
			slideTimer.reset();
			moveTimer.reset();
		}
		else {
			if (moveTimer.getElapsed() > 2) {
				Coordinate below = Coordinate(getPos().x, getPos().y+1);
				bool res = world->moveEntity(getID(), below);
				if (res && slideTimer.getElapsed() > 2) {
					Coordinate newPos = Coordinate(below.x+xVel, below.y);
					bool res = world->moveEntity(getID(), newPos);
					if (!res) xVel = 0;
					slideTimer.reset();
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
		case(PState::Attacking): {
			attackTimer.tick();
			if (attackTimer.getElapsed() > 2) state = PState::Idle;
		}
	}
	
	if (!changePosFlag && world->active) {
		SpylikeEvents::CameraEvent ce("CAMERA_MoveCenterH", getPos());
		eventManager->emit(ce);
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
					if (world->worldType == WorldType::Platform) yDir = 0;
					Coordinate newPos(getPos().x + xDir, getPos().y + yDir);
					world->moveEntity(getID(), newPos);
				}
				else state = GobState::Idle;
			}
			seekTimer.tick();
			break;
		}
		case (GobState::Hurt): {
			hurtTimer.tick();
			if (hurtTimer.getElapsed() > 6) state = GobState::Pursue;
		}
	}
	if (world->worldType == WorldType::Platform) {
		if (moveTimer.getElapsed() > 2) {
			Coordinate below = Coordinate(getPos().x, getPos().y+1);
			bool res = world->moveEntity(getID(), below);
			falling = res;
			moveTimer.reset();
		}
	}
}
			
void Goblin::draw(Camera& painter) {
	if (state == GobState::Found) {
		painter.draw(Coordinate(getPos().x, getPos().y-1), '!', "Effect");
	}
	if (falling) {
		painter.draw(Coordinate(getPos().x, getPos().y-1), '?', "Effect");
	}
	if (state == GobState::Hurt && hurtTimer.getElapsed() % 2) {
		painter.draw(getPos(), '*', "Entity");
	}
	else {
		#ifdef USE_NCURSESW
		painter.draw(getPos(), L"§", "Entity");
		#else
		painter.draw(getPos(), ';', "Entity");
		#endif
	}
}

void Goblin::on_collide(std::shared_ptr<TileEntity> collider) {
	std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(collider);
	if (player) {
		player->hurt(10);
	}
}

void Goblin::hurt(int damage) {
	if (state != GobState::Hurt) {
		health -= damage;
		state = GobState::Hurt;
		hurtTimer.reset();
		if (health <= 0) {
			std::shared_ptr<Treasure> treasure = std::make_shared<Treasure>(5);
			treasure->init(eventManager);
			world->registerEntity(treasure, getPos());
			kill();
		}
	}
}

void Skeleton::draw(Camera& painter) {
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
	if (fireTimer.getElapsed() > 30) {
		std::vector<std::shared_ptr<Player>> res = world->findEntities<Player>(getPos(), 15);
		if (res.size() > 0) {
			std::shared_ptr<Player> player = res[0];
			int xVel = 0;
			if (player->getPos().x > getPos().x) xVel = 45;
			else if (player->getPos().x < getPos().x) xVel = -45;
			int yVel = 0;
			if (player->getPos().y != getPos().y) {
				int yDir = abs(player->getPos().y - getPos().y)/(player->getPos().y - getPos().y);
				if (player->getPos().x != getPos().x) yVel = yDir*abs((xVel*(player->getPos().y - getPos().y))/(player->getPos().x - getPos().x));
				else yVel = yDir*10;
			}
			if (xVel != 0 || yVel != 0) {
				std::shared_ptr<SkeletonArrow> arrow = std::make_shared<SkeletonArrow>(xVel, yVel);
				arrow->init(eventManager);
				world->registerEntity(arrow, getPos());
				arrow->update();
			}
		}
		fireTimer.reset();
	}
	if (world->worldType == WorldType::Platform) {
	    moveTimer.tick();
		if (moveTimer.getElapsed() > 2) {
			Coordinate below = Coordinate(getPos().x, getPos().y+1);
			bool res = world->moveEntity(getID(), below);
			falling = res;
			moveTimer.reset();
		}
	}
}

void Skeleton::hurt(int damage) {
	health -= damage;
	if (health <= 0) {
		std::shared_ptr<Treasure> treasure = std::make_shared<Treasure>(6);
		treasure->init(eventManager);
		world->registerEntity(treasure, getPos());
		kill();
	}
}

void SkeletonArrow::draw(Camera& painter) {
	painter.draw(getPos(), '#', "Entity");
}

void SkeletonArrow::on_update() {
	xFlag += xVel;
	yFlag += yVel;
	Coordinate newPos = getPos();
	if (abs(xFlag) >= 100) {
		newPos.x += abs(xVel)/xVel;
		xFlag = 0;
	}
	if (abs(yFlag) >= 100) {
		newPos.y += abs(yVel)/yVel;
		yFlag = 0;
	}
	if (newPos != getPos()) {
		bool res = world->moveEntity(getID(), newPos);
		if (!res) kill();
	}
}

void SkeletonArrow::on_collide(std::shared_ptr<TileEntity> collider) {
	std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(collider);
	if (player) {
		player->hurt(10);
	}
	kill();
}

void Boss::on_init() {
	SpylikeEvents::AudioPlayEvent ap("AUDIO_PlayMusic", "1-f.wav", 0.25);
	eventManager->emit(ap);
	std::vector<std::shared_ptr<Player>> res = world->findEntities<Player>(getPos(), 100);
	if (res.size() > 0) {
		res[0]->health = 100;
	}
	std::vector<Coordinate> positions = {getPos(), Coordinate(getPos().x, getPos().y-2), Coordinate(getPos().x+1, getPos().y-2), Coordinate(getPos().x+2, getPos().y-2), Coordinate(getPos().x+1, getPos().y-1), Coordinate(getPos().x+2, getPos().y)};
	for (Coordinate& pos : positions) {
		std::shared_ptr<BossSeg> seg = std::make_shared<BossSeg>(std::dynamic_pointer_cast<Boss>(shared_from_this()));
		seg->init(eventManager);
		world->registerEntity(seg, pos);
	}
}

void Boss::draw(Camera& painter) {
	if (isHurt && hurtTimer.getElapsed() % 2) {
		painter.draw(Coordinate(getPos().x, getPos().y-2), '*', "Entity");
		painter.draw(Coordinate(getPos().x+1, getPos().y-2), '*', "Entity");
		painter.draw(Coordinate(getPos().x+2, getPos().y-2), '*', "Entity");
		painter.draw(Coordinate(getPos().x+1, getPos().y-1), '*', "Entity");
		painter.draw(getPos(), '*', "Entity");
		painter.draw(Coordinate(getPos().x+2, getPos().y), '*', "Entity");
	}
	else if (state != BossState::Death) {
		painter.draw(Coordinate(getPos().x, getPos().y-2), '(', "Entity");
		painter.draw(Coordinate(getPos().x+1, getPos().y-2), '&', "Entity");
		painter.draw(Coordinate(getPos().x+2, getPos().y-2), ')', "Entity");
		painter.draw(Coordinate(getPos().x+1, getPos().y-1), '|', "Entity");
		painter.draw(getPos(), '/', "Entity");
		painter.draw(Coordinate(getPos().x+2, getPos().y), '\\', "Entity");
	}
	else {
		if (hurtTimer.getElapsed() < 20) {
			painter.draw(Coordinate(getPos().x, getPos().y-2), '(', "Entity");
			painter.draw(Coordinate(getPos().x+1, getPos().y-2), '&', "Entity");
			painter.draw(Coordinate(getPos().x+2, getPos().y-2), ')', "Entity");
			painter.draw(Coordinate(getPos().x+1, getPos().y-1), '|', "Entity");
			painter.draw(getPos(), '\\', "Entity");
			painter.draw(Coordinate(getPos().x+2, getPos().y), '\\', "Entity");
		}
		else {
			painter.draw(getPos(), '/', "Entity");
			painter.draw(Coordinate(getPos().x-1, getPos().y), '\\', "Entity");
			painter.draw(Coordinate(getPos().x-2, getPos().y), '-', "Entity");
			painter.draw(Coordinate(getPos().x-3, getPos().y), ')', "Entity");
			painter.draw(Coordinate(getPos().x-4, getPos().y), '#', "Entity");
			painter.draw(Coordinate(getPos().x-5, getPos().y), '(', "Entity");
		}
	}
	if (state == BossState::Alert) {
		painter.drawString(Coordinate(getPos().x+1, getPos().y-3), alertSprite.getCurrentFrame(), "Entity");
		alertSprite.nextFrame();
	}
	if (state == BossState::Attack1) {
		if ((fireTimer.getElapsed() % 20) < 3) {
			painter.draw(Coordinate(getPos().x, getPos().y-1), '\\', "Entity");
		}
		else if ((fireTimer.getElapsed() % 20) < 6) {
			painter.draw(Coordinate(getPos().x, getPos().y-1), '/', "Entity");
		}
		
	}
	if (state == BossState::Attack2) {
		if ((fireTimer.getElapsed() % 25 < 6) && (fireTimer.getElapsed() > 25)) {
			painter.draw(Coordinate(getPos().x, getPos().y-1), '\\', "Entity");
		}
	}
}

void Boss::on_update() {
	if (isHurt) {
		hurtTimer.tick();
		if (hurtTimer.getElapsed() > 15) isHurt = false;
	}
	if (state == BossState::Alert) {
		alertTimer.tick();
		if (alertTimer.getElapsed() > 60) {
			if (rand() % 2) state = BossState::Attack1;
			else state = BossState::Attack2;
			std::vector<std::shared_ptr<Player>> res = world->findEntities<Player>(getPos(), 3);
			if (res.size() > 0) {
				auto& player = res[0];
				player->xVel = -3;
				player->yVel = 4;
			}
			fireTimer.reset();
			alertTimer.reset();
		}
	}
	else if (state == BossState::Attack1) {
		fireTimer.tick();
		if ((fireTimer.getElapsed() % 20) < 5) {
			Coordinate arrowPos = Coordinate(getPos().x-1, getPos().y);
			int yVel;
			if ((fireTimer.getElapsed() % 20) % 3 == 0) yVel = 0;
			else if ((fireTimer.getElapsed() % 20) % 3 == 1) yVel = -15;
			else yVel = -10;
			auto tile = world->getTile(arrowPos);
			if (!tile || tile->getEntities().size() == 0) {
				int xVel = -60;
				if (phase2) xVel = -80;
				std::shared_ptr<SkeletonArrow> arrow = std::make_shared<SkeletonArrow>(xVel, yVel);
				arrow->init(eventManager);
				world->registerEntity(arrow, arrowPos);
			}
		}
		if (fireTimer.getElapsed() > 60) {
			state = BossState::Alert;
		}
	}
	else if (state == BossState::Attack2) {
		fireTimer.tick();
		int freq = 25;
		if (phase2) freq = 15;
		if ((fireTimer.getElapsed() % freq) == 0) {
			for (int y=0; y<3; y++) {
				int xVel = -75;
				if (phase2) xVel = -100;
				std::shared_ptr<SkeletonArrow> arrow = std::make_shared<SkeletonArrow>(xVel, 0);
				arrow->init(eventManager);
				world->registerEntity(arrow, Coordinate(getPos().x, getPos().y-y));
			}
		}
		if ((fireTimer.getElapsed() > 100 && !phase2) || (fireTimer.getElapsed() > 150 && phase2)) {
			state = BossState::Alert;
		}
	}
	else if (state == BossState::Death) {
		hurtTimer.tick();
		if (hurtTimer.getElapsed() > 50) {
			SpylikeEvents::MenuEvent me("MENU_Show", "endgame");
			eventManager->emit(me);
		}
	}
	
}

void Boss::hurt(int damage) {
	if (!isHurt) {
		hurtTimer.reset();
		health -= damage;
		if (health < 120) {
			phase2 = true;
		}
		if (health <= 0) {
			isCollidable = false;
			Event ev("AUDIO_PauseMusic");
			eventManager->emit(ev);
			hurtTimer.reset();
			state = BossState::Death;
		}
		else isHurt = true;
	}
}

void BossSeg::on_update() {
	if (boss->health <= 0) isCollidable = false;
}
