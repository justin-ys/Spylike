#ifndef SPYLIKE_WORLD_ENTITIES_H
#define SPYLIKE_WORLD_ENTITIES_H

#include "levelmap.h"

class CollidableEntity : public TileEntity {
	public:
		bool solid = true;
		virtual void on_collide(TileEntity collider);
};

class GeometryTile : public TileEntity {
	char sprite;
	public:
		GeometryTile(char sprite) : sprite(sprite) {}
		void on_draw(Camera& painter);
};

#endif
