#ifndef SPYLIKE_THEGAME_H
#define SPYLIKE_THEGAME_H

#include "scheduling.h"
#include "event.h"
#include "rendering.h"
#include "camera.h"
#include "levelmap.h"
#include "input.h"
#include "screen.h"

class GameManager {
	bool paused;
	std::shared_ptr<EventManager> eventManager;
	std::shared_ptr<InputManager> inputManager;
	std::shared_ptr<Camera> camera;
	GeometryRenderer* renderer;
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
	public:
		void loadLevel(Level level);
		void pause();
		void quit();	
		void run();
};

#endif
