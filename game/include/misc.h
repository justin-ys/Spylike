#ifndef SPYLIKE_ENT_MISC_H
#define SPYLIKE_ENT_MISC_H

#include "levelmap.h"
#include "event.h"
#include "character.h"
#include "rendering.h"
#include "timer.h"
#include <string>

class LevelTransition : public TileEntity {
	std::string levelPath;
	void on_collide(std::shared_ptr<TileEntity> collider) override;
	void on_init() override;
	public:
		LevelTransition() : TileEntity(true) {}
};

class Key : public TileEntity {
	Timer collectedTimer;
	enum KeyState { Idle, Collected };
	KeyState state = KeyState::Idle;
	void on_collide(std::shared_ptr<TileEntity> collider) override;
	void on_update() override;
	void draw(Camera& painter) override;
	public:
		Key() : TileEntity(true) {}

};

class Door : public TileEntity {
	Timer displayTimer;
	enum DoorState { Idle, FailedOpen };
	DoorState state = DoorState::Idle;
	void on_init() override;
	void on_event(Event& e) override;
	void on_collide(std::shared_ptr<TileEntity> collider) override;
	void on_update() override;
	void draw(Camera& painter) override;
	public:
		Door() : TileEntity(true) {}
};

class Treasure : public TileEntity {
	Timer collectedTimer;
	enum TreasureState { Idle, Collected };
	TreasureState state = TreasureState::Idle;
	void draw(Camera& painter) override;
	void on_update() override;
	void on_init() override;
	int amount;
	public:
		void on_collide(std::shared_ptr<TileEntity> collider) override;
		Treasure(int dollars=5) : TileEntity(true), amount{dollars} {}
};

class Typewriter : public TileEntity {
	Timer charTimer;
	std::string text="";
	int cIdx = -1;
	int delay = 1; // frames
	bool started=false;
	void draw(Camera& painter) override;
	void on_update() override;
	void on_init() override;
	public:
		Typewriter() : TileEntity(false) {}
};

#endif
