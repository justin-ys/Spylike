#include "levelmap.h"
#include "rendering.h"
#include "logger.h"
#include <vector>
#include <stdexcept>
#include <memory>
#include <map>

extern SpylikeLogger LOGGER;

void TileEntity::registerWorld(std::shared_ptr<LevelMap> levelMap) {
	world = levelMap;
}

std::vector<std::shared_ptr<TileEntity>> Tile::getEntities() {
	return entities;
}
	
void Tile::addEntity(std::shared_ptr<TileEntity> ent) {
	ent->tile = this;
	entities.push_back(ent);
}

void Tile::removeEntity(int entityID) {
	int idxToRemove = -1;
	for (int i=0; i<entities.size(); i++) {
		if (entities[i]->getID() == entityID) {
			entities[i]->tile = nullptr;
			idxToRemove = i;
		}
	}
	if (idxToRemove >= 0) {
		entities.erase(entities.begin() + idxToRemove);
	}
}

LevelMap::LevelMap(int width, int height, std::shared_ptr<EventManager> eventManager, IDBlock idRange) : width{width}, height{height}, manager{eventManager}, idRange{idRange} {
	tileMap = std::vector<std::shared_ptr<Tile>>(width*height);
	std::fill(tileMap.begin(), tileMap.end(), std::shared_ptr<Tile>(nullptr));
	currentID = idRange.startID;
	trackedEntities = std::map<int, Coordinate>();
	freeIDs = std::vector<int>();
}

int LevelMap::getTileIndex(Coordinate coord) {
	int tileIndex = coord.x + coord.y*width;
	if (tileIndex > tileMap.size() - 1) {
		throw std::invalid_argument("Coordinate outside of map");
	}
	return tileIndex;
}

std::shared_ptr<Tile> LevelMap::getTile(Coordinate coord) {
	return tileMap[getTileIndex(coord)];
}

void LevelMap::putEntity(std::shared_ptr<TileEntity> ent, Coordinate coord) {
	if (tileMap[getTileIndex(coord)] != nullptr) {
		tileMap[getTileIndex(coord)]->addEntity(ent);
	}
	else {
		std::shared_ptr<Tile> tile = std::make_shared<Tile>(coord);
		tileMap[getTileIndex(coord)] = tile;
		tile->addEntity(ent);
	}
}

void LevelMap::destroyTile(Coordinate coord) {
	tileMap[getTileIndex(coord)] = std::shared_ptr<Tile>(nullptr);
}

std::shared_ptr<TileEntity> LevelMap::removeEntity(int entityID) {
	if (trackedEntities.find(entityID) == trackedEntities.end()) {
		return nullptr;
	}
	Coordinate pos = trackedEntities[entityID];
	std::shared_ptr<Tile> currentTile = getTile(pos);
	std::shared_ptr<TileEntity> targetEntity = nullptr;
	if (currentTile != nullptr) {
		for (auto entity : currentTile->getEntities()) {
			if (entity->getID() == entityID) {
				targetEntity = entity;
				break;
			}
		}
	}
	if (targetEntity) {
		currentTile->removeEntity(targetEntity->getID());
		if (currentTile->getEntities().size() == 0) {
			destroyTile(pos);
		}
		trackedEntities.erase(targetEntity->getID());
		return targetEntity;
	}
	return nullptr;
}

void LevelMap::moveEntity(int entityID, Coordinate pos) {
	std::shared_ptr<TileEntity> ent = removeEntity(entityID);
	if (ent) {
		putEntity(ent, pos);
	}
	trackedEntities[entityID] = pos;
}


void LevelMap::updateTile(Coordinate coord) {
	std::shared_ptr<Tile> tile = getTile(coord);
	if (tile != nullptr) {
		for (auto ent : tile->getEntities()) {
			ent->update();
		}
	}
}

void LevelMap::drawTile(Coordinate coord, GeometryRenderer& camera) {
	std::shared_ptr<Tile> tile = getTile(coord);
	if (tile != nullptr) {
		for (auto ent : tile->getEntities()) {
			ent->draw(camera);
		}
	}
}

bool LevelMap::isInMap(Coordinate coord) {
	return ((coord.x > 0) && (coord.x < width) && (coord.y > 0) && (coord.y < height));
}

int LevelMap::getNextID() {
	if (currentID <= idRange.endID) {
		currentID += 1;
		return (currentID - 1);
	}
	else {
		if (freeIDs.size() > 0) {
			int chosenID = freeIDs.back();
			freeIDs.pop_back();
			return chosenID;
		}
		else {
			throw std::runtime_error("World has run out of its entityID allocation");
		}
	}
}
		
void LevelMap::registerEntity(std::shared_ptr<TileEntity> ent, Coordinate pos) {
	ent->registerWorld(shared_from_this());
	ent->registerEventManager(manager);
	ent->setID(getNextID());
	trackedEntities.insert({ent->getID(), pos});
	putEntity(ent, pos);
}
	
