#ifndef SPYLIKE_GEOMETRY_H
#define SPYLIKE_GEOMETRY_H

#include "levelmap.h"

class Geometry: public TileEntity {
	public:
		Geometry() : TileEntity(true) {}
};

class Wall : public Geometry {
	void on_update() override {};
	void on_event(Event& e) override {};
	void draw(GeometryRenderer& painter) override;
	void on_collide(std::shared_ptr<TileEntity> collider) override {};
};
	

#endif
