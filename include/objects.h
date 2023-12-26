#ifndef SPYLIKE_OBJECTS_H
#define SPYLIKE_OBJECTS_H

#include "rendering.h"
#include "scheduling.h"
#include "event.h"
#include <memory>
#include <cassert>

class Object : public EventHandler {
	protected:
		std::shared_ptr<EventManager> eventManager;
		virtual void on_event(Event& e) = 0;
		virtual void on_update() = 0;
		virtual void on_init() {}
		int ID = -1;
		void registerEventManager(std::shared_ptr<EventManager> manager);
	public:
		void update() {
			assert(eventManager);
			assert(ID != -1);
			on_update();
		};
		void event(Event& e) {
			assert(eventManager);
			on_event(e);
		};
		void init(std::shared_ptr<EventManager> manager) {
			registerEventManager(manager);
			on_init();
		}
		void setID(int objID);
		int getID();
};

class SpritedObject : public Object {
	public:
		virtual void draw(TextRenderManager& painter) = 0;
};
		

#endif
