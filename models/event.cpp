#include "event.h"
#include "logger.h"

#include <vector>
#include <memory>
#include <algorithm>

extern SpylikeLogger LOGGER;

void EventManager::subscribe(std::shared_ptr<EventHandler> handler, std::string eventType) {
	if (eventSubscribers.find(eventType) == eventSubscribers.end()) {
		eventSubscribers[eventType] = std::vector<std::shared_ptr<EventHandler>> {handler};
	}
	else {
		eventSubscribers[eventType].push_back(handler);
	}
}

void EventManager::emit(Event& event) {
	std::vector<std::shared_ptr<EventHandler>> toClean;
	if (eventSubscribers.find(event.type) != eventSubscribers.end()) {
		for (auto& handler : eventSubscribers[event.type]) {
			if(handler) handler->on_event(event);
			else toClean.push_back(handler);
		}
	}
	for (auto& handler : toClean) {
		unsubscribe(handler);
	}
}

void EventManager::unsubscribe(std::shared_ptr<EventHandler> handler) {
	for (auto& subscriberPair : eventSubscribers) {
		auto& subscribers = subscriberPair.second;
		subscribers.erase(std::remove(subscribers.begin(), subscribers.end(), handler), subscribers.end());
	}
}
