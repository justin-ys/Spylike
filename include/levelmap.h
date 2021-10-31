#ifndef SPYLIKE_MAP_H
#define SPYLIKE_MAP_H

#include "objects.h"
#include "rendering.h"
#include <map>
#include <vector>
#include <memory>

class Tile;
class TileEntity;
class LevelMap;

class TileEntity : public SpritedObject {
	LevelMap& world;
	public:
		bool isAlive;
		const Tile* tile;
		TileEntity(int entityID, LevelMap& world) : SpritedObject(entityID), world(world), isAlive(true) {}
};

class Tile {
	std::vector<std::shared_ptr<TileEntity>> entities;
	public:
		Tile(Coordinate pos) : pos(pos) {}
		Coordinate pos;
		std::vector<std::shared_ptr<TileEntity>> getEntities();
		void addEntity(std::shared_ptr<TileEntity> ent);
		void removeEntity(int entityID);
};


class LevelMap {
	int width;
	int height;
	std::vector<std::shared_ptr<Tile>> tileMap;
	int getTileIndex(Coordinate coord);
	public:
		LevelMap(int width, int height);
		std::shared_ptr<Tile> getTile(Coordinate coord);
		void putEntity(std::shared_ptr<TileEntity> ent, Coordinate coord);
		void destroyTile(Coordinate coord);
		std::shared_ptr<TileEntity> removeEntity(Coordinate origin, int entityID, Coordinate pos); //returns pointer to entity that was removed if it exists
		void moveEntity(Coordinate origin, int entityID, Coordinate pos);
		void updateTile(Coordinate coord);
		void drawTile(Coordinate coord, GeometryRenderer& camera);
		bool isInMap(Coordinate coord); //convienience function
};


#endif
