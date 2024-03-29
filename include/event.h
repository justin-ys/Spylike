#ifndef SPYLIKE_EVENTS_H
#define SPYLIKE_EVENTS_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "rendering.h"

struct Event {
	virtual ~Event() = default;
	std::string type;
	Event(std::string type): type(type) {}
};

namespace SpylikeEvents {

	// UNUSED: Mouse input likely incompatible with most terminals
	struct MouseInputEvent : Event {
		int x;
		int y;
		MouseInputEvent(std::string type, int x, int y) : Event(type), x(x), y(y) {}
	};
	
	struct KeyInputEvent : Event {
		char c;
		KeyInputEvent(std::string type, char c) : Event(type), c(c) {}
	};

	struct CameraEvent : Event {
		Coordinate pos;
		CameraEvent(std::string type, Coordinate pos=Coordinate(0,0)) : Event(type), pos{pos} {}
	};
	
	struct MenuButtonEvent : Event {
		std::string buttonID;
		MenuButtonEvent(std::string type, std::string buttonID) : Event(type), buttonID(buttonID) {}
	};

	struct MenuEvent : Event {
		std::string menuID;
		MenuEvent(std::string type, std::string menuID) : Event(type), menuID(menuID) {}
	};

	struct LevelChangeEvent : Event {
		std::string levelPath;
		LevelChangeEvent(std::string type, std::string levelPath) : Event(type), levelPath(levelPath) {}
	};
	
	struct PlayerHurtEvent : Event {
		int health;
		PlayerHurtEvent(std::string type, int health) : Event(type), health(health) {}
	};

	struct DoorResponseEvent : Event {
		bool res;
		DoorResponseEvent(std::string type, bool res) : Event(type), res(res) {}
	};

	struct AudioPlayEvent : Event {
		std::string sound;
		float volume;
		AudioPlayEvent(std::string type, std::string sound, float volume=0.5) : Event(type), sound(sound), volume(volume) {}
	};
}

class EventHandler {
	public:
		virtual void on_event(Event& e) = 0;
};

class EventManager {
	std::map<std::string, std::vector<std::shared_ptr<EventHandler>>> eventSubscribers;
	public:
		void subscribe(std::shared_ptr<EventHandler> handler, std::string eventType);
		void unsubscribe(std::shared_ptr<EventHandler> handler);
		void emit(Event& event);
		void clear() { eventSubscribers = {}; }
};
	
#endif
