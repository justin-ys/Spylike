#ifndef SPYLIKE_GENTITIES_H
#define SPYLIKE_GENTITIES_H
#include "levelmap.h"
#include "event.h"
#include "camera.h"

class Character : public TileEntity {
	public:
		Character() : TileEntity(true) {}
		int health = 100;
};

class Player : public Character {
	void on_event(Event& e) override;
	void on_update() override;
	void draw(GeometryRenderer& painter) override;
	void on_collide(std::shared_ptr<TileEntity> collider) override;
};

#endif
