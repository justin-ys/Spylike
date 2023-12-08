#include "character.h"
#include "rendering.h"
#include "logger.h"
#include <algorithm>

extern SpylikeLogger LOGGER;

void Player::on_event(Event& e) {
	if (e.type == "INPUT_KeyPress") {
		SpylikeEvents::KeyInputEvent& ke = dynamic_cast<SpylikeEvents::KeyInputEvent&>(e);
		Coordinate pos = getPos();
		if (ke.c == 'w' || ke.c == 'a' || ke.c == 's' || ke.c == 'd') {
			Coordinate newPos = pos;
			Coordinate camPos = camera.getOrigin();
			switch(ke.c) {
				case ('w'): {
					newPos.y--;
					camPos.y--;
					break;
				}
				case ('a'): {
					newPos.x--;
					camPos.x--;
					break;
				}
				case ('s'): {
					newPos.y++;
					camPos.y++;
					break;
				}
				case ('d'): {
					newPos.x++;
					camPos.x++;
				}
			}
			if (world->isInMap(newPos)) {
				world->moveEntity(getID(), newPos);
				camera.setOrigin(camPos);
			}
		}
	}
}

void Player::draw(GeometryRenderer& painter) {
	painter.draw(getPos(), '@', "UI");
}

void Player::on_update() {}
