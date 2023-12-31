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
	enum PState { Idle, Hurt, Attacking };
	PState state = Idle;
	Timer hurtTimer;
	Timer moveTimer;
	Timer slideTimer;
	Timer attackTimer;
	std::vector<SpriteFrame> hurtFrames{SpriteFrame{SpriteDelta(0, "")}, SpriteFrame{SpriteDelta(0, "@")}};
	Sprite hurtSprite{hurtFrames, 1};
	Coordinate attackLoc = Coordinate(0, 0);
	void on_event(Event& e) override;
	void on_update() override;
	void on_init() override;
	void draw(Camera& painter) override;
	void on_collide(std::shared_ptr<TileEntity> collider) override;
	Coordinate nextCamPos = Coordinate(-1, -1);
	bool changePosFlag = false;
	public:
		int yVel = 0;
		int xVel = 0;
		void hurt(int damage) override;
};

class Goblin : public Character {
	enum GobState {Idle, Found, Pursue};
	GobState state = Idle;
	Timer seekTimer;
	Timer moveTimer;
	bool falling = false;
	void on_event(Event& e) override {}
	void on_update() override;
	void draw(Camera& painter) override;
	void on_collide(std::shared_ptr<TileEntity> collider) override;
	public:
		int health = 60;
		void hurt(int damage) override;
};

class Skeleton : public Character {
	Timer fireTimer;
	Timer moveTimer;
	bool falling=false;
	void on_event(Event& e) override {}
	void on_update() override;
	void draw(Camera& painter) override;
	void on_collide(std::shared_ptr<TileEntity> collider) override;
	public:
		int health = 100;
		void hurt(int damage) override;
};

class SkeletonArrow : public TileEntity {
	void on_event(Event& e) override {}
	void on_update() override;
	void draw(Camera& painter) override;
	void on_collide(std::shared_ptr<TileEntity> collider) override;
	Coordinate initialPos;
	int xFlag=0;
	int yFlag=0;
	int xVel;
	int yVel;
	public:
		SkeletonArrow(int xVel, int yVel) : TileEntity(true), xVel{xVel}, yVel{yVel} {}
};

class Boss : public Character {
	enum BossState { Alert, Attack1, Attack2, Death};
	bool isHurt = false;
	BossState state = BossState::Alert;
	Timer alertTimer;
	Timer fireTimer;
	Timer hurtTimer;
	std::vector<SpriteFrame> alertFrames{SpriteFrame{SpriteDelta(0, "")}, SpriteFrame{SpriteDelta(0, "!")}, SpriteFrame{SpriteDelta(0, "")}, SpriteFrame{SpriteDelta(0, "!")}, SpriteFrame{SpriteDelta(0, "")}, SpriteFrame{SpriteDelta(0, "")}};
	Sprite alertSprite{alertFrames, 1};
	void on_update() override;
	void draw(Camera& painter) override;
	void on_collide(std::shared_ptr<TileEntity> collider) override {}
	void on_init() override;
	bool phase2 = false;
	public:
		void hurt(int damage) override;
        int health = 240;
};

class BossSeg : public Character {
	std::shared_ptr<Boss> boss;
	public:
		BossSeg(std::shared_ptr<Boss> boss) : boss{boss} {}
		void hurt(int damage) override { boss->hurt(damage); }
		void on_update() override;
};
	
#endif
