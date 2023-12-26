#ifndef SPYLIKE_OBSTACLE_H
#define SPYLIKE_OBSTACLE_H

#include "levelmap.h"
#include "timer.h"

class Lava : public TileEntity {
	void on_event(Event& e) override {}
	void on_update() override;
	void draw(Camera& painter) override;
	void on_collide(std::shared_ptr<TileEntity> collider) override;
	int count = 0;
	Timer moveTimer;
	public:
		Lava() : TileEntity(true) {}
};

class LavaGenerator : public TileEntity {
	void on_event(Event& e) override {}
	void on_update() override;
	void draw(Camera& painter) override;
	void on_collide(std::shared_ptr<TileEntity> collider) override;
	public:
		LavaGenerator() : TileEntity(true) {}
};

class Spike : public TileEntity {
	void on_event(Event& e) override {}
	void on_update() override {}
	void draw(Camera& painter) override;
	void on_collide(std::shared_ptr<TileEntity> collider) override;
	public:
		Spike() : TileEntity(true) {}
};

#endif
