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
#include "geometry.h"
#include <vector>
#include <string>
#include <memory>
#include <unistd.h>
#include <string>

extern SpylikeLogger LOGGER;

void GameManager::RunLevelTask::update() {
	manager.camera->clearScreen();
	for (int y=0; y<manager.map->height; y++) {
		for (int x=0; x<manager.map->width; x++) {
			manager.map->updateTile(Coordinate(x, y));
			manager.map->drawTile(Coordinate(x, y), *manager.gameRenderer);
		}
	}
	manager.gameRenderer->drawBox(Coordinate(0, 0), Coordinate(manager.menuManager->getScreenWidth(), manager.menuManager->getScreenHeight()), "Overlay");
	manager.camera->renderToScreen();
	manager.inputManager->update();

}

void GameManager::TickTask::update() {}

void GameManager::MenuTask::update() {
	manager.menuManager->clearScreen();
	manager.activeMenu->draw(*manager.menuRenderer);
	manager.activeMenu->update();
	manager.menuRenderer->drawBox(Coordinate(0, 0), Coordinate(manager.menuManager->getScreenWidth(), manager.menuManager->getScreenHeight()), "Overlay");
	manager.menuManager->renderToScreen();
	manager.menuInputManager->update();
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
	eventManager->subscribe(shared_from_this(), "MENU_Show");
	eventManager->subscribe(shared_from_this(), "MENU_ButtonClick");
	IDBlock idAllocation = {0, 1024};
	map = std::make_shared<LevelMap>(level.width, level.height, eventManager, idAllocation);
	for (auto entPair : level.entities) {
		map->registerEntity(entPair.first, entPair.second);
	}
}

// Note: You must close any active menus, before showing a new one.
void GameManager::showMenu(Menu menu, bool pause) {
	if (!scheduler.isRunning("MenuTask")) {
		activeMenu = std::make_shared<Menu>(std::move(menu));
		activeMenu->setID(1025);
		activeMenu->init(eventManager);
		menuEventManager->subscribe(activeMenu, "INPUT_KeyPress");
		if (pause) {
			scheduler.pauseTask("RunLevel");
		}
		camera->clearScreen();
		camera->renderToScreen();
		camera->lock();
		scheduler.resumeTask("MenuTask");
	}
}

void GameManager::closeMenu() {
	menuEventManager->unsubscribe(activeMenu);
	activeMenu = nullptr;
	scheduler.resumeTask("RunLevel");
	scheduler.pauseTask("MenuTask");
	camera->unlock();
}

void GameManager::on_event(Event& e) {
	if (e.type == "MENU_Show") {
		showMenu(SpylikeMenus::testMenu(), true);
	}
	if (e.type == "MENU_ButtonClick") {
		SpylikeEvents::MenuButtonEvent& mb = dynamic_cast<SpylikeEvents::MenuButtonEvent&>(e);
		if (mb.buttonID == "close") closeMenu();
	}
}


void GameManager::run() {
	std::vector<RenderLayer> layers {RenderLayer("Entity", 1), RenderLayer("Effect", 2), RenderLayer("UI", 3), RenderLayer("Overlay", 4)};
	NcursesTerminalScreen screen(80, 25);
	camera = std::make_shared<Camera>(screen, 80, 25, layers);
	menuManager = std::make_shared<TextRenderManager>(screen, layers);

	eventManager = std::make_shared<EventManager>();
	menuEventManager = std::make_shared<EventManager>();
	inputManager = std::make_shared<InputManager>(eventManager, screen);
	menuInputManager = std::make_shared<InputManager>(menuEventManager, screen);
	GeometryRenderer theRenderer = GeometryRenderer(*camera);
	gameRenderer = &theRenderer;
	GeometryRenderer theMenuRenderer = GeometryRenderer(*menuManager);
	menuRenderer = &theMenuRenderer;

	std::map<std::shared_ptr<TileEntity>, Coordinate> entities = {{std::make_shared<Player>(), Coordinate(4, 4)}, {std::make_shared<Goblin>(), Coordinate(1,1)},
		{std::make_shared<Wall>(), Coordinate(3, 3)}, {std::make_shared<Wall>(), Coordinate(4, 3)}, {std::make_shared<Wall>(), Coordinate(5, 3)}, {std::make_shared<Wall>(), Coordinate(3, 4)}};
	Level level = Level(WorldType::Platform, 100, 50, entities);
	loadLevel(level);
	
	scheduler.addTask(std::make_unique<RunLevelTask>(*this));
	scheduler.addTask(std::make_unique<TickTask>(*this));
	scheduler.addTask(std::make_unique<MenuTask>(*this));
	scheduler.pauseTask("MenuTask");
	scheduler.run();
}	
