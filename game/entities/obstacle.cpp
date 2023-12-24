#include "obstacle.h"
#include "character.h"
#include <memory>
#include <cstdlib>

void Lava::draw(GeometryRenderer& painter) {
	painter.draw(getPos(), '#', "Entity");
}

void Lava::on_collide(std::shared_ptr<TileEntity> collider) {
	std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(collider);
	if (player) {
		player->hurt(5);
	}
}

void Lava::on_update() {
	moveTimer.tick();
	if (moveTimer.getElapsed() > 2) {
		moveTimer.reset();
		Coordinate newPos = Coordinate(getPos().x, getPos().y-1);
		if (world->isInMap(newPos)) {
			bool res = world->moveEntity(getID(), newPos);
		}
		count += 1;
	}
	if (count > 7) kill();

}

void LavaGenerator::draw(GeometryRenderer& painter) {
	painter.draw(getPos(), '#', "Entity");
}

void LavaGenerator::on_collide(std::shared_ptr<TileEntity> collider) {
	std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(collider);
	if (player) {
		player->hurt(10);
	}
}

void LavaGenerator::on_update() {
	if (rand() % 60 == 17) {
		std::shared_ptr<Lava> lava = std::make_shared<Lava>();
		lava->init(eventManager);
		world->registerEntity(lava, Coordinate(getPos().x, getPos().y-1));
	}
}

void Spike::draw(GeometryRenderer& painter) {
	painter.draw(getPos(), '#', "Entity");
}

void Spike::on_collide(std::shared_ptr<TileEntity> collider) {
	std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(collider);
	if (player) {
		player->hurt(5);
	}
}
