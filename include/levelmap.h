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

struct IDBlock {
	int startID;
	int endID;
};

class TileEntity : public SpritedObject {
	std::shared_ptr<LevelMap> world;
	public:
		bool isAlive = true;
		const Tile* tile;
		void registerWorld(std::shared_ptr<LevelMap> levelMap);
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


class LevelMap : public std::enable_shared_from_this<LevelMap> {
	int width;
	int height;
	std::vector<std::shared_ptr<Tile>> tileMap;
	int getTileIndex(Coordinate coord);
	std::map<int, Coordinate> trackedEntities; //entityID: location mapping
	std::shared_ptr<EventManager> manager;
	IDBlock idRange; //range of IDs the manager is allowed to assign to
	std::vector<int> freeIDs;
	int currentID;
	int getNextID();
	public:
		LevelMap(int width, int height, std::shared_ptr<EventManager> eventManager, IDBlock idRange);
		std::shared_ptr<Tile> getTile(Coordinate coord);
		void putEntity(std::shared_ptr<TileEntity> ent, Coordinate coord);
		void destroyTile(Coordinate coord);
		std::shared_ptr<TileEntity> removeEntity(int entityID); //returns pointer to entity that was removed if it exists
		void moveEntity(int entityID, Coordinate pos);
		void updateTile(Coordinate coord);
		void drawTile(Coordinate coord, GeometryRenderer& camera);
		bool isInMap(Coordinate coord); //convienience function
		void registerEntity(std::shared_ptr<TileEntity> ent, Coordinate pos);
		Coordinate getEntityPos(int entityID);
};


#endif
