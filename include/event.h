#ifndef SPYLIKE_EVENTS_H
#define SPYLIKE_EVENTS_H

#include <string>
#include <vector>
#include <map>
#include <memory>

struct Event {
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

}

class EventHandler {
	public:
		virtual void on_event(Event& e) = 0;
};

class EventManager {
	std::map<std::string, std::vector<std::shared_ptr<EventHandler>>> eventSubscribers;
	public:
		void subscribe(std::shared_ptr<EventHandler> handler, std::string eventType);
		void emit(Event& event);
};
	
#endif
