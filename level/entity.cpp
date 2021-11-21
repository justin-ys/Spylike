#include "entity.h"
#include "event.h"

void ManagedEntity::update() { parent.update(); }
void ManagedEntity::draw(GeometryRenderer& painter) { parent.draw(painter); }
void ManagedEntity::on_event(Event e) { parent.event(e); }

void ManagerEntity::createChild(Coordinate pos) {
	//TODO: we need an object manager to generate IDs from, so this is on hold for now
}

