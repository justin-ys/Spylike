#ifndef SPYLIKE_ENTITY_H
#define SPYLIKE_ENTITY_H

#include "levelmap.h"
#include "event.h"
#include "rendering.h"
#include <memory>
#include <vector>

class ManagedEntity : public TileEntity {
	public:
		TileEntity& parent;
		ManagedEntity(TileEntity& parentTile, int entityID, LevelMap& world) : TileEntity(entityID, world), parent(parentTile) {}
		void update();
		void draw(GeometryRenderer& painter);
		void on_event(Event e);
};

class ManagerEntity : public TileEntity {
	std::vector<std::unique_ptr<ManagedEntity>> childTiles = std::vector<std::unique_ptr<ManagedEntity>>();
	void createChild(Coordinate pos);
};

#endif
