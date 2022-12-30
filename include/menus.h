#ifndef SPYLIKE_MENUS_H
#define SPYLIKE_MENUS_H

#include "levelmap.h"
#include "event.h"
#include <string>
#include <memory>

class MenuButton : public TileEntity {
	void on_update();
	void on_event(Event& e);
	void draw(GeometryRenderer& painter);
	int width;
	int height;
	std::string buttonID;
	std::string text;
	public:
		MenuButton(int width, int height, std::string buttonText, std::string buttonID);
		void click();
};

#endif
