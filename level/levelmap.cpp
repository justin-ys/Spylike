#include "levelmap.h"
#include "rendering.h"
#include "logger.h"
#include <vector>
#include <stdexcept>
#include <memory>

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
		if (entities[i]->ID == entityID) {
			entities[i]->tile = nullptr;
			idxToRemove = i;
		}
	}
	if (idxToRemove >= 0) {
		entities.erase(entities.begin() + idxToRemove);
	}
}
			

LevelMap::LevelMap(int width, int height) : width{width}, height{height} {
	tileMap = std::vector<std::shared_ptr<Tile>>(width*height);
	std::fill(tileMap.begin(), tileMap.end(), std::shared_ptr<Tile>(nullptr));
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

std::shared_ptr<TileEntity> LevelMap::removeEntity(Coordinate origin, int entityID, Coordinate pos) {
	std::shared_ptr<Tile> currentTile = getTile(origin);
	std::shared_ptr<TileEntity> targetEntity = nullptr;
	if (currentTile != nullptr) {
		for (auto entity : currentTile->getEntities()) {
			if (entity->ID == entityID) {
				targetEntity = entity;
				break;
			}
		}
	}
	if (targetEntity) {
		currentTile->removeEntity(targetEntity->ID);
		if (currentTile->getEntities().size() == 0) {
			destroyTile(origin);
		}
		return targetEntity;
	}
	return nullptr;
}

void LevelMap::moveEntity(Coordinate origin, int entityID, Coordinate pos) {
	std::shared_ptr<TileEntity> ent = removeEntity(origin, entityID, pos);
	if (ent) {
		putEntity(ent, pos);
	}
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
