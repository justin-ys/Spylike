#ifndef SPYLIKE_OBSTACLE_H
#define SPYLIKE_OBSTACLE_H

#include "levelmap.h"

class Lava : public TileEntity {
	void on_event(Event& e) override {}
	void on_update() override;
	void draw(GeometryRenderer& painter) override;
	void on_collide(std::shared_ptr<TileEntity> collider) override;
	public:
		Lava() : TileEntity(true) {}
};
#endif
