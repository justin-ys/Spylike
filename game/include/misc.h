#ifndef SPYLIKE_ENT_MISC_H
#define SPYLIKE_ENT_MISC_H

#include "levelmap.h"
#include "event.h"
#include "character.h"
#include "rendering.h"
#include <string>

class LevelTransition : public TileEntity {
	std::string levelPath;
	public:
		LevelTransition(std::string levelPath) : TileEntity(true), levelPath{levelPath} {}
		void on_collide(std::shared_ptr<TileEntity> collider) override;
		void on_event(Event& e) override {}
		void on_update() override {}
		void draw(GeometryRenderer& painter) override {}
};

#endif
