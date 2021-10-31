#include "event.h"

#include <vector>
#include <memory>

void EventManager::subscribe(std::shared_ptr<EventHandler> handler, std::string eventType) {
	if (eventSubscribers.find(eventType) == eventSubscribers.end()) {
		eventSubscribers[eventType] = std::vector<std::shared_ptr<EventHandler>> {handler};
	}
	else {
		eventSubscribers[eventType].push_back(handler);
	}
}

void EventManager::emit(Event event) {
	if (eventSubscribers.find(event.type) != eventSubscribers.end()) {
		for (auto& handler : eventSubscribers[event.type]) {
			handler->on_event(event);
		}
	}
}
