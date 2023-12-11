#ifndef SPYLIKE_GENTITIES_H
#define SPYLIKE_GENTITIES_H
#include "levelmap.h"
#include "event.h"
#include "camera.h"
#include "timer.h"
#include "sprites.h"

class Character : public TileEntity {
	protected:
		int health = 100;
	public:
		Character() : TileEntity(true) {}
		virtual void hurt(int damage) = 0;
};

class Player : public Character {
	enum PState { Idle, Hurt };
	PState state = Idle;
	Timer hurtTimer;
	std::vector<SpriteFrame> hurtFrames{SpriteFrame{SpriteDelta(0, "")}, SpriteFrame{SpriteDelta(0, "@")}};
	Sprite hurtSprite{hurtFrames, 1};
	void on_event(Event& e) override;
	void on_update() override;
	void draw(GeometryRenderer& painter) override;
	void on_collide(std::shared_ptr<TileEntity> collider) override;
	public:
		void hurt(int damage) override;
};

class Goblin : public Character {
	enum GobState {Idle, Found, Pursue};
	GobState state = Idle;
	Timer seekTimer;
	void on_event(Event& e) override {}
	void on_update() override;
	void draw(GeometryRenderer& painter) override;
	void on_collide(std::shared_ptr<TileEntity> collider) override;
	public:
		void hurt(int damage) override;
};
#endif
