#ifndef SPYLIKE_MAP_H
#define SPYLIKE_MAP_H

#include "objects.h"
#include "rendering.h"
#include "camera.h"
#include "logger.h"
#include <map>
#include <vector>
#include <memory>
#include <cmath>

extern SpylikeLogger LOGGER;

class Tile;
class TileEntity;
class LevelMap;

struct IDBlock {
	int startID;
	int endID;
};

typedef int EntityID;

enum WorldType { Roguelike, Platform };

class Camera; // forward dec defined in camera.h

class TileEntity : public Object, public std::enable_shared_from_this<TileEntity> {
	std::shared_ptr<TileEntity> parent = nullptr;
	std::vector<std::shared_ptr<TileEntity>> children;
	bool alive = true;
	protected:
		std::shared_ptr<LevelMap> world;
	public:
		bool isCollidable;
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
		virtual void on_collide(std::shared_ptr<TileEntity> collider) {}
		virtual void draw(Camera& painter) = 0;
		TileEntity(bool collidable) : isCollidable{collidable} {}
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
		bool active = true;
		const int width;
		const int height;
		const WorldType worldType;
		LevelMap(int width, int height, std::shared_ptr<EventManager> eventManager, IDBlock idRange, WorldType wt=Platform);
		std::shared_ptr<Tile> getTile(Coordinate coord);
		void updateTile(Coordinate coord);
		void drawTile(Coordinate coord, Camera& painter);
		void destroyTile(Coordinate coord);
		std::shared_ptr<TileEntity> findEntity(int entityID);
		// Searches for entities of type Ent within range tiles of origin and returns them. If range=0, searches everything
		template<typename Ent> std::vector<std::shared_ptr<Ent>> findEntities(Coordinate origin=Coordinate(0,0), int range=0) {
			std::vector<std::shared_ptr<Ent>> res;
			for (auto entPair : trackedEntities) {
				if ((range <= 0) || (sqrt(pow(origin.x - entPair.second.x, 2) + pow(origin.y - entPair.second.y, 2)) <= range)) {
					std::shared_ptr<TileEntity> ent = findEntity(entPair.first);
					std::shared_ptr<Ent> specialized = std::dynamic_pointer_cast<Ent>(ent);
					if (specialized) res.push_back(std::dynamic_pointer_cast<Ent>(ent));
				}
			}
			return res;
		}
		void putEntity(std::shared_ptr<TileEntity> ent, Coordinate coord);
		void removeEntity(std::shared_ptr<TileEntity> ent);
		// Returns true if entity successfully moved, false otherwise (e.g. collision)
		bool moveEntity(std::shared_ptr<TileEntity> ent, Coordinate pos);
		bool moveEntity(EntityID entityID, Coordinate pos);
		bool isInMap(Coordinate coord); //convenience function
		void registerEntity(std::shared_ptr<TileEntity> ent, Coordinate pos);
};

struct Level {
	WorldType worldType;
	int width;
	int height;
	std::map<std::shared_ptr<TileEntity>, Coordinate> entities;
	Level(WorldType wt, int w, int h, std::map<std::shared_ptr<TileEntity>, Coordinate> entities) : worldType{wt}, width{w}, height{h}, entities{entities} {}
};

#endif
