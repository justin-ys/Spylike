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
		void on_update();
		void draw(Camera& painter);
		void on_event(Event& e);
};

class ManagerEntity : public TileEntity {
	std::vector<std::shared_ptr<ManagedEntity>> childTiles = std::vector<std::shared_ptr<ManagedEntity>>();
	void createChild(Coordinate pos);
};

#endif
