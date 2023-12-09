#include "entity.h"
#include "event.h"
#include <memory>

//TODO: Still not entirely sure how to implement this and how it will be used

void ManagedEntity::on_update() { parent.update(); }
void ManagedEntity::draw(GeometryRenderer& painter) { parent.draw(painter); }
void ManagedEntity::on_event(Event& e) { parent.event(e); }

void ManagerEntity::createChild(Coordinate pos) { }

