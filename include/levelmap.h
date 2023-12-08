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

typedef int EntityID;

enum WorldType { Roguelike, Platform };

class TileEntity : public SpritedObject, public std::enable_shared_from_this<TileEntity> {
	std::shared_ptr<TileEntity> parent = nullptr;
	std::vector<std::shared_ptr<TileEntity>> children;
	bool alive = true;
	protected:
		std::shared_ptr<LevelMap> world;
	public:
	    // getter/setter
		const Tile* tile = nullptr;
		void setTile(Tile* tileObj);
		void setParent(std::shared_ptr<TileEntity> ent);
		void removeParent();
		std::shared_ptr<TileEntity> getParent();
		// check if removing tile removes children
		void addChild(std::shared_ptr<TileEntity> ent);
		void removeChild(EntityID entityID);
		std::vector<std::shared_ptr<TileEntity>> getChildren();
		void registerWorld(std::shared_ptr<LevelMap> levelMap);
		void kill();
		bool isAlive();
		Coordinate getPos();
};

class Tile {
	std::vector<std::shared_ptr<TileEntity>> entities;
	public:
		Tile(Coordinate pos) : pos(pos) {}
		// why can we change pos
		const Coordinate pos;
		std::vector<std::shared_ptr<TileEntity>> getEntities();
		void addEntity(std::shared_ptr<TileEntity> ent);
		void removeEntity(EntityID entityID);
};


class LevelMap : public std::enable_shared_from_this<LevelMap> {
	std::vector<std::shared_ptr<Tile>> tileMap;
	int getTileIndex(Coordinate coord);
	std::map<int, Coordinate> trackedEntities; //entityID: location mapping
	std::shared_ptr<EventManager> manager;
	IDBlock idRange; //range of IDs the manager is allowed to assign to
	std::vector<int> freeIDs;
	int currentID;
	int getNextID();
	public:
		const int width;
		const int height;
		const WorldType worldType;
		LevelMap(int width, int height, std::shared_ptr<EventManager> eventManager, IDBlock idRange, WorldType wt=Platform);
		std::shared_ptr<Tile> getTile(Coordinate coord);
		void updateTile(Coordinate coord);
		void drawTile(Coordinate coord, GeometryRenderer& painter);
		void destroyTile(Coordinate coord);
		std::shared_ptr<TileEntity> findEntity(int entityID);
		void putEntity(std::shared_ptr<TileEntity> ent, Coordinate coord);
		void removeEntity(std::shared_ptr<TileEntity> ent);
		void moveEntity(std::shared_ptr<TileEntity> ent, Coordinate pos);
		void moveEntity(EntityID entityID, Coordinate pos);
		bool isInMap(Coordinate coord); //convenience function
		void registerEntity(std::shared_ptr<TileEntity> ent, Coordinate pos);
};


#endif
