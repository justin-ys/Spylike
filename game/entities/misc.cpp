#include "misc.h"
#include "event.h"

void LevelTransition::on_init() {
	levelPath = "game/resource/levels/" + dynamicProperties["transition"] + ".spm";
	LOGGER.log(levelPath, DEBUG);
}

void LevelTransition::on_collide(std::shared_ptr<TileEntity> collider) {
	std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(collider);
	if (player) {
		SpylikeEvents::LevelChangeEvent le("LEVEL_Change", levelPath);
		eventManager->emit(le);
	}
}

void Key::on_collide(std::shared_ptr<TileEntity> collider) {
	std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(collider);
	if (player) {
		isCollidable = false;
		Event ev("GAME_KeyCollect");
		eventManager->emit(ev);
		collectedTimer.reset();
		state = KeyState::Collected;
	}
}

void Key::on_update() {
	if (state == KeyState::Collected) {
		collectedTimer.tick();
		if (collectedTimer.getElapsed() > 30) kill();
	}
}

void Key::draw(Camera& painter) {
	switch (state) {
		case (KeyState::Idle): {
			painter.draw(getPos(), '%', "Entity");
			break;
		}
		case (KeyState::Collected): {
			painter.drawString(Coordinate(getPos().x-10, getPos().y-1), "Key Collected!", "Effect");
		}
	}
}

void Door::on_init() {
	eventManager->subscribe(shared_from_this(), "GAME_DoorResponse");
}

void Door::on_event(Event& e) {
	if (e.type == "GAME_DoorResponse") {
		SpylikeEvents::DoorResponseEvent& dr = dynamic_cast<SpylikeEvents::DoorResponseEvent&>(e);
		if (dr.res) kill();
		else { displayTimer.reset(); state = DoorState::FailedOpen; }
	}
}

void Door::on_collide(std::shared_ptr<TileEntity> collider) {
	std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(collider);
	if (player) {
		Event ev("GAME_DoorRequest");
		eventManager->emit(ev);
	}
}

void Door::on_update() {
	if (state == DoorState::FailedOpen) {
		displayTimer.tick();
		if (displayTimer.getElapsed() > 30) state = DoorState::Idle;
	}
}

void Door::draw(Camera& painter) {
	painter.draw(getPos(), '=', "Entity");
	if (state == DoorState::FailedOpen) {
		painter.drawString(Coordinate(getPos().x-10, getPos().y-1), "Get a key first!", "Effect");
	}
}

void Treasure::draw(Camera& painter) {
	switch (state) {
		case (TreasureState::Idle): {
			painter.draw(getPos(), '$', "Entity");
			break;
		}
		case (TreasureState::Collected): {
			painter.drawString(Coordinate(getPos().x, getPos().y-1), "+" + std::to_string(amount), "Effect");
		}
	}
}

void Treasure::on_collide(std::shared_ptr<TileEntity> collider) {
	std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(collider);
	if (player) {
		int treasure = std::stoi(world->getGameState("treasure"));
		treasure += amount;
		world->setGameState("treasure", std::to_string(treasure));
		isCollidable = false;
		state = TreasureState::Collected;
	}
}

void Treasure::on_update() {
	if (state == TreasureState::Collected) {
		collectedTimer.tick();
		if (collectedTimer.getElapsed() > 15) kill();
	}
}
