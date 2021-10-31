#ifndef SPYLIKE_OBJECTS_H
#define SPYLIKE_OBJECTS_H

#include "rendering.h"
#include "scheduling.h"
#include "event.h"

class Object : public EventHandler {
	public:
		int ID;
		Object(int ID) : ID(ID) {}
		virtual void update() = 0;
		virtual void on_event(Event e) = 0;
};

class SpritedObject : public Object {
	public:
		SpritedObject(int ID): Object(ID) {}
		virtual void update() = 0;
		virtual void on_event(Event e) = 0;
		virtual void draw(GeometryRenderer& painter) = 0;
};
		

#endif
