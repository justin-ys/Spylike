#ifndef SPYLIKE_GENTITIES_H
#define SPYLIKE_GENTITIES_H
#include "levelmap.h"
#include "event.h"
#include "camera.h"

class Player : public TileEntity {
	Camera& camera;
	void on_event(Event& e) override;
	void on_update() override;
	void draw(GeometryRenderer& painter) override;
	public:
		Player(Camera& camera) : camera{camera} {}
};

#endif
