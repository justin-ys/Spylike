#include "obstacle.h"
#include "character.h"
#include <memory>

void Lava::draw(GeometryRenderer& painter) {
	painter.draw(getPos(), '#', "UI");
}

void Lava::on_collide(std::shared_ptr<TileEntity> collider) {
	std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(collider);
	if (player) {
		player->hurt(10);
	}
}

void Lava::on_update() {
	//Coordinate newPos = Coordinate(getPos().x-1, getPos().y-1);
	//if (world->isInMap(newPos)) {
		//bool res = world->moveEntity(getID(), newPos);
	//}
}
