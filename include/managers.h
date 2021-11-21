#ifndef SPYLIKE_MANAGERS_H
#define SPYLIKE_MANAGERS_H

#include "levelmap.h"
#include "rendering.h"
#include <memory>

struct entityHolder {
	int entityID;
	Coordinate location;
}

class ObjectManager {
	vector<entityHolder> trackedEntities;
	public:
		void registerEntity(std::shared_pointer<TileEntity> ent);
};	

#endif
