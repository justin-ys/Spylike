#ifndef SPYLIKE_THEGAME_H
#define SPYLIKE_THEGAME_H

#include "scheduling.h"
#include "event.h"
#include "rendering.h"
#include "camera.h"
#include "levelmap.h"
#include "input.h"
#include "screen.h"
#include "menus.h"
#include "geometry.h"
#include "character.h"
#include "obstacle.h"
#include "logger.h"
#include "misc.h"
#include "audio.h"

#include <string>
#include <iostream>
#include <memory>

extern SpylikeLogger LOGGER;

inline Level load_from_file(std::string path) {
	LOGGER.log("Reading level from " + path, DEBUG);
	std::ifstream input(path);
	std::string levelTypeStr;
	WorldType levelType;
	getline(input, levelTypeStr);
	if (levelTypeStr == "Roguelike") levelType = WorldType::Roguelike;
	else levelType = WorldType::Platform;
	std::string widthStr;
	std::string heightStr;
	int width;	
	int height;
	getline(input, widthStr);	
	getline(input, heightStr);
	width = std::stoi(widthStr);
	height = std::stoi(heightStr);
	std::map<std::shared_ptr<TileEntity>, Coordinate> entities = {};
	std::string entLine;
	while (getline(input, entLine)) {
		std::string name;
		int idx = 0;
		while(entLine[idx] != ' ') {
			name += entLine[idx];
			idx++;
		}
		std::shared_ptr<TileEntity> ent;
		if (name == "Player") ent = std::make_shared<Player>();
		else if (name == "Goblin") ent = std::make_shared<Goblin>();
		else if (name == "Spike") ent = std::make_shared<Spike>();
		else if (name == "Skeleton") ent = std::make_shared<Skeleton>();
		else if (name == "Lava") ent = std::make_shared<LavaGenerator>();
		else if (name == "Key") ent = std::make_shared<Key>();
		else if (name == "Door") ent = std::make_shared<Door>();
		else if (name == "Boss") ent = std::make_shared<Boss>();
		else ent = std::make_shared<Wall>();
		idx++;
		std::string entXStr;
		while (entLine[idx] != ',') {
			entXStr += entLine[idx];
			idx++;
		}
		int entX = std::stoi(entXStr);
		idx++;
		std::string entYStr;
		while (idx != entLine.length() && entLine[idx] != ' ') {
			entYStr += entLine[idx];
			idx++;
		}
		int entY = std::stoi(entYStr);
		if (name == "LevelTrans") {
			while (entLine[idx] != ':') idx++;
			idx++;
			std::string levelPath;
			while (idx < entLine.length() && entLine[idx] != ' ') {
				levelPath += entLine[idx];
				idx++;
			}
			levelPath = "game/resource/levels/" + levelPath + ".spm";
			ent = std::make_shared<LevelTransition>(levelPath);
		}
		entities[ent] = Coordinate(entX, entY);
	}
	return Level(levelType, width, height, entities);
}

class GameManager : public EventHandler, public std::enable_shared_from_this<GameManager> {
	bool paused;
	FrameScheduler scheduler = FrameScheduler(20);
	std::shared_ptr<EventManager> eventManager;
	std::shared_ptr<EventManager> menuEventManager;
	std::shared_ptr<InputManager> inputManager;
	std::shared_ptr<InputManager> menuInputManager;
	std::shared_ptr<Camera> camera;
	std::shared_ptr<TextRenderManager> menuManager;
	GeometryRenderer* gameRenderer;
	GeometryRenderer* menuRenderer;
	std::shared_ptr<Menu> activeMenu;
	std::shared_ptr<LevelMap> map;
	std::shared_ptr<AudioManager> audioManager;
	int playerHealth = 100;
	bool keyCollected = false;
	class RunLevelTask : public ScheduledTask {
		GameManager& manager;
		public:
			RunLevelTask(GameManager& manager) : ScheduledTask("RunLevel"), manager{manager} {}
			void update() override;
	};
	class TickTask : public ScheduledTask {
		GameManager& manager;
		public:
			TickTask(GameManager& manager) : ScheduledTask("RunTick"), manager{manager} {}
			void update() override;
	};
	class MenuTask : public ScheduledTask {
		GameManager& manager;
		public:
			MenuTask(GameManager& manager) : ScheduledTask("MenuTask"), manager{manager} {}
			void update() override;
	};
	class StartupTask : public ScheduledTask {
		GameManager& manager;
		public:
			StartupTask(GameManager& manager) : ScheduledTask("StartupTask"), manager{manager} {}
			void update() override;
	};
	public:
		void loadLevel(Level level);
		void pause();
		void quit();	
		void run();
		void showMenu(std::shared_ptr<Menu> menu, bool pause=true);
		void closeMenu();
		void on_event(Event& e) override;
};

#endif
