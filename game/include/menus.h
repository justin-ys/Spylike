#ifndef SPYLIKE_MENUS_H
#define SPYLIKE_MENUS_H

#include "levelmap.h"
#include "event.h"
#include <string>
#include <memory>
#include <map>
#include <queue>

class MenuButton : public SpritedObject {
	void on_update();
	void on_event(Event& e);
	std::string text;
	std::string buttonID;
	public:
		const int width;
		const int height;
		Coordinate pos; // top left
		MenuButton(Coordinate pos, int width, int height, std::string buttonText, std::string buttonID);
		void draw(GeometryRenderer& painter);
		void click();
		std::string getButtonID();
};

class Menu : public SpritedObject {
	int width;
	int height;
	int nextID = -1;
	std::map<std::string, MenuButton> buttons;
	std::queue<std::string> selectionList;
	std::string currentSelection = "";
	void on_update();
	void on_event(Event& e);
	void on_init() override;
	public:
		Menu(int width, int height): width{width}, height{height} {}
		void draw(GeometryRenderer& painter);
		void addButton(MenuButton button);
		void setSelection(std::string buttonID);
		void selectNext();
		void click();
};

namespace SpylikeMenus {
	Menu testMenu();
	Menu pauseMenu();
}

#endif
