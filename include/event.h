#ifndef SPYLIKE_EVENTS_H
#define SPYLIKE_EVENTS_H

#include <string>
#include <vector>
#include <map>
#include <memory>

struct Event {
	std::string type;
	std::string ID;
};

class EventHandler {
	public:
		virtual void on_event(Event e) = 0;
};

class EventManager {
	std::map<std::string, std::vector<std::shared_ptr<EventHandler>>> eventSubscribers;
	public:
		void subscribe(std::shared_ptr<EventHandler> handler, std::string eventType);
		void emit(Event event);
};
	
#endif
