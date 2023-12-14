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
	public:
		void loadLevel(Level level);
		void pause();
		void quit();	
		void run();
		void showMenu(Menu menu, bool pause=true);
		void closeMenu();
		void on_event(Event& e) override;
};

#endif
