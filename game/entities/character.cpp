#include "character.h"
#include "rendering.h"
#include "logger.h"
#include "event.h"
#include <algorithm>

extern SpylikeLogger LOGGER;

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
				eventManager->emit(ce);
				world->moveEntity(getID(), newPos);
			}
		}
	}
}

void Player::draw(GeometryRenderer& painter) {
	painter.draw(getPos(), '@', "UI");
}

void Player::on_update() {}
