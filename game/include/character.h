#ifndef SPYLIKE_GENTITIES_H
#define SPYLIKE_GENTITIES_H
#include "levelmap.h"
#include "event.h"
#include "camera.h"
#include "timer.h"
#include "sprites.h"

class Character : public TileEntity {
	public:
		Character() : TileEntity(true) {}
		virtual void hurt(int damage) = 0;
		int health = 100; // should use hurt, but game manager needs to change it manually too
};

class Player : public Character {
	enum PState { Idle, Hurt };
	PState state = Idle;
	Timer hurtTimer;
	Timer moveTimer;
	Timer slideTimer;
	std::vector<SpriteFrame> hurtFrames{SpriteFrame{SpriteDelta(0, "")}, SpriteFrame{SpriteDelta(0, "@")}};
	Sprite hurtSprite{hurtFrames, 1};
	int yVel = 0;
	int xVel = 0;
	void on_event(Event& e) override;
	void on_update() override;
	void on_init() override;
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

class Skeleton : public Character {
	Timer fireTimer;
	void on_event(Event& e) override {}
	void on_update() override;
	void draw(GeometryRenderer& painter) override;
	void on_collide(std::shared_ptr<TileEntity> collider) override;
	public:
		void hurt(int damage) override;
};

class SkeletonArrow : public TileEntity {
	void on_event(Event& e) override {}
	void on_update() override;
	void draw(GeometryRenderer& painter) override;
	void on_collide(std::shared_ptr<TileEntity> collider) override;
	Timer moveTimer;
	Coordinate initialPos;
	int xFlag=0;
	int yFlag=0;
	int xVel;
	int yVel;
	public:
		SkeletonArrow(int xVel, int yVel) : TileEntity(true), xVel{xVel}, yVel{yVel} {}
};
	
#endif
