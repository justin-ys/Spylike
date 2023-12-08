#include "levelmap.h"
#include "rendering.h"
#include "logger.h"
#include <vector>
#include <stdexcept>
#include <memory>
#include <map>
#include <algorithm>

extern SpylikeLogger LOGGER;

void TileEntity::registerWorld(std::shared_ptr<LevelMap> levelMap) {
	world = levelMap;
}

void TileEntity::setTile(Tile* tileObj) {
	tile = tileObj;
}

void TileEntity::setParent(std::shared_ptr<TileEntity> ent) {
	if (parent != ent) {
		parent = ent;
		ent->addChild(shared_from_this());
	}
}

void TileEntity::removeParent() {
	if (parent != nullptr) {
		parent = nullptr;
		parent->removeChild(getID());
	}
}

std::shared_ptr<TileEntity> TileEntity::getParent() {
	return parent;
}

void TileEntity::addChild(std::shared_ptr<TileEntity> ent) {	
	if (std::find(children.begin(), children.end(), ent) == children.end()) {
		children.push_back(ent);
		ent->setParent(shared_from_this());
	}
}

void TileEntity::removeChild(EntityID entityID) {
	int idxToRemove = -1;
	for (int i=0; i<children.size(); i++) {
		if (children[i]->getID() == entityID) {
			idxToRemove = i;
		}
	}
	if (idxToRemove >= 0) {
		std::shared_ptr<TileEntity> ent = children[idxToRemove];
		children.erase(children.begin() + idxToRemove);
		ent->removeParent();
	}
}

std::vector<std::shared_ptr<TileEntity>> TileEntity::getChildren() {
	return children;
}

	
std::vector<std::shared_ptr<TileEntity>> Tile::getEntities() {
	return entities;
}

void TileEntity::kill() {
	world->removeEntity(shared_from_this());
	alive = false;
}

bool TileEntity::isAlive() {
	return alive;
}

Coordinate TileEntity::getPos() {
	return tile->pos;
}
	
void Tile::addEntity(std::shared_ptr<TileEntity> ent) {
	ent->setTile(this);
	entities.push_back(ent);
}

void Tile::removeEntity(EntityID entityID) {
	int idxToRemove = -1;
	for (int i=0; i<entities.size(); i++) {
		if (entities[i]->getID() == entityID) {
			entities[i]->setTile(nullptr);
			idxToRemove = i;
		}
	}
	if (idxToRemove >= 0) {
		entities.erase(entities.begin() + idxToRemove);
	}
}

LevelMap::LevelMap(int width, int height, std::shared_ptr<EventManager> eventManager, IDBlock idRange, WorldType wt) : width{width}, height{height}, manager{eventManager}, idRange{idRange}, worldType{wt} {
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

void LevelMap::removeEntity(std::shared_ptr<TileEntity> ent) {
	auto currentTile = getTile(ent->tile->pos); // entity's tile is const
	if (currentTile) {
		currentTile->removeEntity(ent->getID());
		if (currentTile->getEntities().size() == 0) {
			destroyTile(currentTile->pos);
		}
		for (auto child : ent->getChildren()) {
			removeEntity(child);
		}
		trackedEntities.erase(ent->getID());
	}
}

std::shared_ptr<TileEntity> LevelMap::findEntity(int entityID) {
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
	return targetEntity;
}

void LevelMap::moveEntity(std::shared_ptr<TileEntity> ent, Coordinate pos) {
	if (ent) {
		for (auto child : ent->getChildren()) { 
			if (child->tile) {
				moveEntity(child, pos + (child->tile->pos - ent->tile->pos));
			}
		}
		auto currentTile = getTile(ent->tile->pos);
		if (currentTile) {
			currentTile->removeEntity(ent->getID());
			if (currentTile->getEntities().size() == 0) {
				destroyTile(currentTile->pos);
			}
			trackedEntities.erase(ent->getID());
		}
		putEntity(ent, pos);
	}
	trackedEntities[ent->getID()] = pos;
}

void LevelMap::moveEntity(EntityID entityID, Coordinate pos) {
	moveEntity(findEntity(entityID), pos);
}

void LevelMap::updateTile(Coordinate coord) {
	std::shared_ptr<Tile> tile = getTile(coord);
	if (tile != nullptr) {
		for (auto ent : tile->getEntities()) {
			ent->update();
		}
	}
}

void LevelMap::drawTile(Coordinate coord, GeometryRenderer& painter) {
	std::shared_ptr<Tile> tile = getTile(coord);
	if (tile != nullptr) {
		for (auto ent : tile->getEntities()) {
			ent->draw(painter);
		}
	}
}

bool LevelMap::isInMap(Coordinate coord) {
	return (((coord.x >= 0) && (coord.x < width)) && ((coord.y >= 0) && (coord.y < height)));
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
	
