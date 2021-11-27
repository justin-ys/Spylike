#include "objects.h"

void Object::setID(int objID) {
	ID = objID;
}

int Object::getID() {
	return ID;
}

void Object::registerEventManager(std::shared_ptr<EventManager> manager) {
	eventManager = manager;
}
