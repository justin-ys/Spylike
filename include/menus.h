#ifndef SPYLIKE_MENUS_H
#define SPYLIKE_MENUS_H

#include "objects.h"
#include "rendering.h"

class StartupLogo : public SpritedObject {
	public:
		void update();
		void draw(GeometryRenderer& painter);
};

#endif
