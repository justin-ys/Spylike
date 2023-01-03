#ifndef SPYLIKE_MENUS_H
#define SPYLIKE_MENUS_H

#include "levelmap.h"
#include "event.h"
#include <string>
#include <memory>
#include <map>
#include <queue>

class MenuButton : public TileEntity {
	void on_update();
	void on_event(Event& e);
	int width;
	int height;
	std::string text;
	std::string buttonID;
	public:
		MenuButton(int width, int height, std::string buttonText, std::string buttonID);
		void draw(GeometryRenderer& painter);
		void click();
		std::string getButtonID();
};

class Menu : public TileEntity {
	int width;
	int height;
	std::map<std::string, std::shared_ptr<MenuButton>> buttons;
	std::queue<std::string> selectionList;
	std::string currentSelection = "";
	void on_update();
	void on_event(Event& e);
	public:
		Menu(int width, int height): width{width}, height{height} {}
		void draw(GeometryRenderer& painter);
		void addButton(std::shared_ptr<MenuButton> button, Coordinate pos);
		void setSelection(std::string buttonID);
		void selectNext();
		void click();
};
		

#endif
