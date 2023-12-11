#include <iostream>
#include <cmath>
#include "rendering.h"
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
using namespace std;
#include <vector>
#include <string>
#include <memory>
#include <unistd.h>
#include <string>

extern SpylikeLogger LOGGER;

namespace Game {
	void GameManager::RunLevelTask::update() {
		inputManager->update();
		for (int y=0; y<20; y++) {
			for (int x=0; x<60; x++) {
				map->updateTile(Coordinate(x, y));
				map->drawTile(Coordinate(x, y), renderer);
			}
		}
		camera->clearScreen();
	}

	void GameManager::pause() {
		paused = true;
	}
	
	void GameManager::quit() {
		exit();
	}

	void GameManager::tick() {

		screen.end();
	}
		
	void GameManager::run() {
		vector<RenderLayer> layers = {RenderLayer("Entity", 1), RenderLayer("Effect", 2), RenderLayer("UI", 3)};
		NcursesTerminalScreen screen(60, 20);

		eventManager = std::shared_ptr<EventManager>(new EventManager());
		inputManager = std::shared_ptr<InputManager>(new InputManager(manager, screen));
		
		std::shared_ptr<Camera> camera = std::make_shared<Camera>(screen, 60, 20, layers);
		GeometryRenderer renderer(*camera);
		manager->subscribe(camera, "CAMERA_MoveUp");
		manager->subscribe(camera, "CAMERA_MoveDown");
		manager->subscribe(camera, "CAMERA_MoveLeft");
		manager->subscribe(camera, "CAMERA_MoveRight");
		
		std::shared_ptr<epicEntity> ent = std::make_shared<epicEntity>(coolSprite);
		
		//std::shared_ptr<MenuButton> button = std::make_shared<MenuButton>(20, 4, "mug moment", "Entity");
		//std::shared_ptr<Menu> menu = std::make_shared<Menu>(60, 20);
		std::shared_ptr<Player> player = std::make_shared<Player>();
		manager->subscribe(player, "INPUT_KeyPress");

		std::shared_ptr<Goblin> goblin = std::make_shared<Goblin>();
	
		//menu->addChild(button);
		
		IDBlock idAllocation = {0, 1024};
		std::shared_ptr<LevelMap> map = std::make_shared<LevelMap>(60, 20, manager, idAllocation);
		map->registerEntity(ent, Coordinate(5,5));
		//map->registerEntity(menu, Coordinate(1,1));
		map->registerEntity(player, Coordinate(10, 10));
		map->registerEntity(goblin, Coordinate(15, 15));
		//menu->addButton(button, Coordinate(0,0));
		//map->moveEntity(menu, Coordinate(3,3));
		//menu->click();
		
		bool flag = true;
	}
}
