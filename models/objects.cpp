#include "objects.h"

void Object::setID(int objID) {
	ID = objID;
}

void Object::registerEventManager(std::shared_ptr<EventManager> manager) {
	eventManager = manager;
}
