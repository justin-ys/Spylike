#include "geometry.h"

void Wall::draw(GeometryRenderer& painter) {
	painter.draw(Coordinate(getPos().x, getPos().y), 178, "Entity");
}
