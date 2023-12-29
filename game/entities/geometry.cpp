#include "geometry.h"

void Wall::draw(Camera& painter) {
	#ifdef USE_NCURSESW
	painter.draw(Coordinate(getPos().x, getPos().y), L"\u2592", "Entity");
	#else
	painter.draw(Coordinate(getPos().x, getPos().y), '~', "Entity");
	#endif
}
