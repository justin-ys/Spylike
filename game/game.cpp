#include <iostream>
#include <cmath>
#include "rendering.h"
#include "scheduling.h"
#include "camera.h"
#include "screen.h"
#include "sprites.h"
#include "levelmap.h"
#include "event.h"
#include "logger.h"
#include "menus.h"
#include "input.h"
#include "character.h"
#include "obstacle.h"
#include "game.h"
#include <vector>
#include <string>
#include <memory>
#include <unistd.h>
#include <string>

extern SpylikeLogger LOGGER;

void GameManager::RunLevelTask::update() {
	manager.camera->clearScreen();
	for (int y=0; y<20; y++) {
		for (int x=0; x<60; x++) {
			manager.map->updateTile(Coordinate(x, y));
			manager.map->drawTile(Coordinate(x, y), *manager.renderer);
		}
	}
}

void GameManager::TickTask::update() {
	manager.inputManager->update();
}

void GameManager::pause() {
	paused = true;
}

void GameManager::quit() {
	exit(0);
}

void GameManager::loadLevel(Level level) {
	eventManager->clear();
	eventManager->subscribe(camera, "CAMERA_MoveUp");
	eventManager->subscribe(camera, "CAMERA_MoveDown");
	eventManager->subscribe(camera, "CAMERA_MoveLeft");
	eventManager->subscribe(camera, "CAMERA_MoveRight");
	IDBlock idAllocation = {0, 1024};
	map = std::make_shared<LevelMap>(level.width, level.height, eventManager, idAllocation);
	for (auto entPair : level.entities) {
		map->registerEntity(entPair.first, entPair.second);
	}
}
	
void GameManager::run() {
	std::vector<RenderLayer> layers {RenderLayer("Entity", 1), RenderLayer("Effect", 2), RenderLayer("UI", 3)};
	NcursesTerminalScreen screen(60, 20);
	camera = std::make_shared<Camera>(screen, 60, 20, layers);

	eventManager = std::make_shared<EventManager>();
	inputManager = std::make_shared<InputManager>(eventManager, screen);
	GeometryRenderer theRenderer = GeometryRenderer(*camera);
	renderer = &theRenderer;

	std::map<std::shared_ptr<TileEntity>, Coordinate> entities = {{std::make_shared<Player>(), Coordinate(4, 4)}, {std::make_shared<Goblin>(), Coordinate(1,1)}};
	Level level = Level(WorldType::Platform, 60, 20, entities);
	loadLevel(level);

	FrameScheduler scheduler;
	scheduler.addTask(std::make_unique<RunLevelTask>(*this));
	scheduler.addTask(std::make_unique<TickTask>(*this));
	scheduler.run();
}
