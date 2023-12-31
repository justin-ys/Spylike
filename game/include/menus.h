#ifndef SPYLIKE_MENUS_H
#define SPYLIKE_MENUS_H

#include "levelmap.h"
#include "event.h"
#include <string>
#include <memory>
#include <map>
#include <queue>
#include <memory>

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
		void draw(TextRenderManager& painter);
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
	void on_init() final;
	virtual void menu_init() {}
	public:
		Menu(int width, int height): width{width}, height{height} {}
		virtual void draw(TextRenderManager& painter);
		void addButton(MenuButton button);
		void setSelection(std::string buttonID);
		void selectNext();
		void click();
};

class PauseMenu : public Menu {
	public:
		PauseMenu(int width, int height): Menu(width, height) {}
		void draw(TextRenderManager& painter) override;
};

class GameOverMenu : public Menu {
	void menu_init() override;
	public:
		GameOverMenu(int width, int height): Menu(width, height) {}
		void draw(TextRenderManager& painter) override;
};

class StartMenu : public Menu {
	void menu_init() override;
	public:
		StartMenu(int width, int height): Menu(width, height) {}
		void draw(TextRenderManager& painter) override;
};

class EndMenu : public Menu {
	void menu_init() override;
	public:
		EndMenu(int width, int height): Menu(width, height) {}
		void draw(TextRenderManager& painter) override;
};

namespace SpylikeMenus {
	std::shared_ptr<Menu> testMenu();
	std::shared_ptr<Menu> pauseMenu();
	std::shared_ptr<Menu> gameOver();
	std::shared_ptr<Menu> startMenu();
	std::shared_ptr<Menu> endGame();
}

#endif
