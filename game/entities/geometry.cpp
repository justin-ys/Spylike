#include "geometry.h"

void Wall::draw(Camera& painter) {
	painter.draw(Coordinate(getPos().x, getPos().y), '~', "Entity");
}
