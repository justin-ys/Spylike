#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <memory>
#include <unistd.h>
#include <string>
#include <cstring>

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
#include "animation.h"

#ifdef USE_DISCORD
#include "discord_rpc.h"
#endif

extern SpylikeLogger LOGGER;

std::string formatSeconds(int seconds) {
	int minutes = seconds/60;
	seconds = seconds - 60*minutes;
	return std::to_string(minutes) + ":" + std::to_string(seconds);
}

GameManager::GameManager() {
	#ifdef USE_DISCORD
	memset(&handlers, 0, sizeof(handlers));
    handlers.ready = handleDiscordReady;
    handlers.disconnected = handleDiscordDisconnected;
    handlers.errored = handleDiscordError;
    handlers.joinGame = handleDiscordJoin;
    handlers.spectateGame = handleDiscordSpectate;
    handlers.joinRequest = handleDiscordJoinRequest;
    Discord_Initialize(DISCORD_CLIENT_ID, &handlers, 1, NULL);
	#endif
}


void GameManager::RunLevelTask::update() {
	auto& theMap = manager.map;
	manager.camera->clearScreen();
	Coordinate origin = manager.camera->getOrigin();
	for (int y=origin.y; y<(origin.y+manager.camera->getScreenHeight()); y++) {
		for (int x=origin.x; x<(origin.x+manager.camera->getScreenWidth()); x++) {
			if (theMap->isInMap(Coordinate(x, y))) {
				theMap->updateTile(Coordinate(x, y));
			}
			if (manager.killUpdates) {
				manager.killUpdates = false;
				return;
			}
		}
		
	}
	for (int y=origin.y; y<(origin.y+manager.camera->getScreenHeight()); y++) {
		for (int x=origin.x; x<(origin.x+manager.camera->getScreenWidth()); x++) {
			if (manager.map->isInMap(Coordinate(x, y))) {
				manager.map->drawTile(Coordinate(x, y), *manager.camera);
			}
		}
		
	}
	//manager.menuRenderer->drawBox(Coordinate(0, 0), Coordinate(manager.menuManager->getScreenWidth(), manager.menuManager->getScreenHeight()), "Overlay");
	//manager.menuManager->renderToScreen();
	manager.camera->toggleAbsolute();
	manager.camera->drawString(Coordinate(0, 0), "Health: " + std::to_string(manager.playerHealth), "UI");
	manager.camera->drawString(Coordinate(0, 1), "Time elapsed: " + formatSeconds(manager.scheduler.timeElapsed()), "UI");
	manager.camera->toggleAbsolute();
	manager.camera->renderToScreen();
	manager.inputManager->update();
}

void GameManager::TickTask::update() {}

void GameManager::MenuTask::update() {
	manager.menuManager->clearScreen();
	manager.activeMenu->draw(*manager.menuManager);
	manager.activeMenu->update();
	manager.menuManager->drawBox(Coordinate(0, 0), Coordinate(manager.menuManager->getScreenWidth(), manager.menuManager->getScreenHeight()-1), "Overlay");
	manager.menuManager->renderToScreen();
	manager.menuInputManager->update();
}

void GameManager::AnimationTask::update() {
	anim.draw(*manager.menuManager);
	manager.menuManager->renderToScreen();
	if (anim.isFinished()) {
		manager.scheduler.pauseTask("AnimationTask");
	}
}

void GameManager::StartupTask::update() {
	if (!startedAnimation) {
		#ifdef USE_DISCORD
		DiscordRichPresence discordPresence;
		memset(&discordPresence, 0, sizeof(discordPresence));
		discordPresence.state = "Main menu";
		discordPresence.largeImageKey = "rg_logo";
		Discord_UpdatePresence(&discordPresence);
		Discord_RunCallbacks();
		#endif
		std::vector<std::string> frames = {"hello", "world!"};
		Animation anim(frames, 20, "UI");
		manager.scheduler.addTask(std::make_unique<AnimationTask>(manager, anim));
		startedAnimation = true;
	}
	else if (!manager.scheduler.isRunning("AnimationTask")) {
		manager.scheduler.destroyTask("AnimationTask");
		manager.showMenu(SpylikeMenus::startMenu(), true);
		manager.scheduler.resetElapsed();
		manager.scheduler.pauseTask("StartupTask");
	}
}


void GameManager::pause() {
	paused = true;
}

void GameManager::quit() {
	screen.end();
	exit(0);
}

void GameManager::loadLevel(Level level) {
	if (map) map->active = false;
	killUpdates = true;
	LOGGER.log("Loading level", DEBUG);
	eventManager->clear();
	eventManager->subscribe(camera, "CAMERA_MoveUp");
	eventManager->subscribe(camera, "CAMERA_MoveDown");
	eventManager->subscribe(camera, "CAMERA_MoveLeft");
	eventManager->subscribe(camera, "CAMERA_MoveRight");
	eventManager->subscribe(camera, "CAMERA_Move");
	eventManager->subscribe(camera, "CAMERA_MoveCenter");
	eventManager->subscribe(camera, "CAMERA_MoveCenterH");
	eventManager->subscribe(camera, "CAMERA_MoveCenterV");
	eventManager->subscribe(audioManager, "AUDIO_PlayMusic");
	eventManager->subscribe(audioManager, "AUDIO_PauseMusic");
	eventManager->subscribe(shared_from_this(), "MENU_Show");
	eventManager->subscribe(shared_from_this(), "INPUT_KeyPress");
	eventManager->subscribe(shared_from_this(), "MENU_ButtonClick");
	eventManager->subscribe(shared_from_this(), "LEVEL_Change");
	eventManager->subscribe(shared_from_this(), "GAME_PlayerHurt");
	eventManager->subscribe(shared_from_this(), "GAME_KeyCollect");
	eventManager->subscribe(shared_from_this(), "GAME_DoorRequest");
	IDBlock idAllocation = {0, 1024};
	map = std::make_shared<LevelMap>(level.width, level.height, eventManager, idAllocation, level.worldType);
	for (auto entPair : level.entities) {
		std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(entPair.first);
		if (player) {
			player->health = playerHealth;
		}
		map->registerEntity(entPair.first, entPair.second);
	}
	if (!audioManager->isPlaying()) audioManager->playMusic("1-1.wav", 0.25);
	#ifdef USE_DISCORD
    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));
    discordPresence.state = level.title.c_str();
    discordPresence.details = "Playing a level";
    discordPresence.largeImageKey = "rg_logo";
    Discord_UpdatePresence(&discordPresence);
    Discord_RunCallbacks();
    #endif
}

// Note: You must close any active menus, before showing a new one.
void GameManager::showMenu(std::shared_ptr<Menu> menu, bool pause) {
	scheduler.pauseElapsed();
	audioManager->pauseMusic();
	if (!scheduler.isRunning("MenuTask")) {
		activeMenu = menu;
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
	scheduler.unpauseElapsed();
	menuEventManager->unsubscribe(activeMenu);
	activeMenu = nullptr;
	scheduler.resumeTask("RunLevel");
	scheduler.pauseTask("MenuTask");
	camera->unlock();
	audioManager->resumeMusic();
}

void GameManager::on_event(Event& e) {
	if (e.type == "MENU_Show") {
		showMenu(SpylikeMenus::testMenu(), true);
	}
	if (e.type == "MENU_ButtonClick") {
		SpylikeEvents::MenuButtonEvent& mb = dynamic_cast<SpylikeEvents::MenuButtonEvent&>(e);
		if (mb.buttonID == "close") closeMenu();
		if (mb.buttonID == "restart") {closeMenu(); playerHealth=100; keyCollected=false; loadLevel(load_from_file("game/resource/levels/1-1.spm"));}
		if (mb.buttonID == "quit") quit();
	}
	if (e.type == "LEVEL_Change") {
		SpylikeEvents::LevelChangeEvent& lc = dynamic_cast<SpylikeEvents::LevelChangeEvent&>(e);
		Level level = load_from_file(lc.levelPath);
		loadLevel(level);
	}
	if (e.type == "INPUT_KeyPress") {
		SpylikeEvents::KeyInputEvent& ke = dynamic_cast<SpylikeEvents::KeyInputEvent&>(e);
		if (ke.c == 27) {
			showMenu(SpylikeMenus::pauseMenu());
		}
	}
	if (e.type == "GAME_PlayerHurt") {
		SpylikeEvents::PlayerHurtEvent& ph = dynamic_cast<SpylikeEvents::PlayerHurtEvent&>(e);
		playerHealth = ph.health;
		if (playerHealth <= 0) showMenu(SpylikeMenus::gameOver());
	}
	if (e.type == "GAME_KeyCollect") {
		keyCollected = true;
	}
	if (e.type == "GAME_DoorRequest") {
		SpylikeEvents::DoorResponseEvent ev("GAME_DoorResponse", keyCollected);
		eventManager->emit(ev);
	}
}


void GameManager::run() {
	std::vector<RenderLayer> layers {RenderLayer("Entity", 1), RenderLayer("Effect", 2), RenderLayer("UI", 3), RenderLayer("Overlay", 4)};
	camera = std::make_shared<Camera>(screen, 80, 30, layers);
	menuManager = std::make_shared<TextRenderManager>(screen, layers);


	eventManager = std::make_shared<EventManager>();
	menuEventManager = std::make_shared<EventManager>();
	inputManager = std::make_shared<InputManager>(eventManager, screen);
	menuInputManager = std::make_shared<InputManager>(menuEventManager, screen);
	
	audioManager = std::make_shared<MiniaudioManager>("game/resource/audio/");

	Level level = load_from_file("game/resource/levels/1-3.spm");
	loadLevel(level);
	
	scheduler.addTask(std::make_unique<RunLevelTask>(*this));
	scheduler.addTask(std::make_unique<TickTask>(*this));
	scheduler.addTask(std::make_unique<MenuTask>(*this));
	scheduler.addTask(std::make_unique<StartupTask>(*this));
	scheduler.pauseTask("MenuTask");
	scheduler.pauseTask("RunLevel");
	scheduler.run();
}	
