#ifndef SPYLIKE_OBJECTS_H
#define SPYLIKE_OBJECTS_H

#include "rendering.h"
#include "scheduling.h"
#include "event.h"
#include <memory>
#include <cassert>

class Object : public EventHandler {
	std::shared_ptr<EventManager> eventManager;
	virtual void on_event(Event e) = 0;
	virtual void on_update() = 0;
	public:
		int ID = -1;
		void update() {
			assert(eventManager);
			assert(ID != -1);
			on_update();
		};
		void event(Event e) {
			assert(eventManager);
			on_event(e);
		};
		void setID(int objID);
		void registerEventManager(std::shared_ptr<EventManager> manager);
};

class SpritedObject : public Object {
	public:
		virtual void draw(GeometryRenderer& painter) = 0;
};
		

#endif
