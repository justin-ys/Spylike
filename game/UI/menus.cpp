#include "menus.h"
#include "rendering.h"
#include "event.h"
#include "logger.h"
#include <curses.h>
#include <string>
#include <stdexcept>
#include <memory>

extern SpylikeLogger LOGGER;

MenuButton::MenuButton(Coordinate pos, int width, int height, std::string buttonText, std::string buttonID) : pos{pos}, width{width}, height{height}, text{buttonText}, buttonID{buttonID} {}

void MenuButton::click() {
	auto ev = SpylikeEvents::MenuButtonEvent("MENU_ButtonClick", buttonID);
	eventManager->emit(ev);
}

void MenuButton::draw(GeometryRenderer& painter) {
	std::string drawnText = text;
	// note that subtracting 2 from width may cause integer overflow(???) so we have to do it this way
	if (((text.length() + 2) > width) || (height < 3)) {
		if (width <= 3) {
			drawnText = "";
		}
		else {
			drawnText = text.substr(0, width - 5) + "...";
		}
	}
	Coordinate topLeft = pos;
	Coordinate bottomRight = Coordinate(pos.x + width - 1, pos.y + height - 1);
	painter.drawBox(topLeft, bottomRight, "UI");
	Coordinate drawOffset = Coordinate(width/2 - drawnText.length()/2 + pos.x, 
		height/2 + pos.y);
	painter.drawString(drawOffset, drawnText, "UI");
}

std::string MenuButton::getButtonID() {
	return buttonID;
}

void MenuButton::on_event(Event& e) {}

void MenuButton::on_update() {}

void Menu::draw(GeometryRenderer& painter) {
	Coordinate selectionPos = buttons.find(currentSelection)->second.pos;
	Coordinate arrowPos = Coordinate(selectionPos.x + buttons.find(currentSelection)->second.width/2, selectionPos.y - 1);
	painter.draw(arrowPos, 'v', "UI");
	painter.draw(Coordinate(arrowPos.x, arrowPos.y-1), '|', "UI");
	for (auto& buttonPair : buttons) {
		buttonPair.second.draw(painter);
	}
}

void PauseMenu::draw(GeometryRenderer& painter) {
	/*
	painter.drawString(Coordinate(1, 1), ""
 "|||\n"
"|   |\n"
"|   |        ||         |      |    |||||   |||||||\n"
"||||        |  |        |      |    |       |      \n"
"|          ||||||       |      |    |||||   |||||||\n"
"|         |      |      |      |        |   |      \n"
"|        |        |      ||||||     |||||   |||||||\n", "UI");
*/
	painter.drawString(Coordinate(painter.getScreenWidth()/2-5, 1), "PAUSED!", "UI");
	Menu::draw(painter);
}

void GameOverMenu::draw(GeometryRenderer& painter) {
	painter.drawString(Coordinate(painter.getScreenWidth()/2-8, 1), "GAME OVER!", "UI");
	Menu::draw(painter);
}

void StartMenu::draw(GeometryRenderer& painter) {
	painter.drawString(Coordinate(painter.getScreenWidth()/2-12, 1), "WELCOME TO SPYLIKE!", "UI");	
	painter.drawString(Coordinate(painter.getScreenWidth()/2-7, 2), "CONTROLS:", "UI");
	painter.drawString(Coordinate(painter.getScreenWidth()/2-10, 3), "W,A,S,D MOVEMENT", "UI");
	painter.drawString(Coordinate(painter.getScreenWidth()/2-9, 4), "V,B,N,G ATTACK", "UI");
	painter.drawString(Coordinate(painter.getScreenWidth()/2-9, 5), "ESC PAUSE", "UI");
	Menu::draw(painter);
}


// pos is *relative* to menu
void Menu::addButton(MenuButton button) {
	buttons.insert({button.getButtonID(), button});
	selectionList.push(button.getButtonID());
	selectNext();
}

void Menu::setSelection(std::string buttonID) {
	currentSelection = buttonID;
}

void Menu::selectNext() {
	if (selectionList.size() > 0) {
		selectionList.push(selectionList.front());
		selectionList.pop();
		setSelection(selectionList.front());
	}
}

void Menu::click() {
	if (currentSelection != "") {
		buttons.find(currentSelection)->second.click();
	}
}

void Menu::on_event(Event& e) {
	if (e.type == "INPUT_KeyPress") {
		SpylikeEvents::KeyInputEvent& ke = dynamic_cast<SpylikeEvents::KeyInputEvent&>(e);
		if (ke.c == ' ' || ke.c == KEY_ENTER) {
			click();
		}
		else if (ke.c == 'w' || ke.c == 'a' || ke.c == 's' || ke.c == 'd') {
			selectNext();
		}
	}
}

void Menu::on_update() {
	for (auto& buttonPair : buttons) {
		buttonPair.second.update();
	}
}

// must set ID before initalization
void Menu::on_init() {
	assert(ID != -1);
	int nextID = ID+1;
	for (auto& buttonPair : buttons) {
		buttonPair.second.setID(nextID);
		nextID++;	
		buttonPair.second.init(eventManager);
	}
}

namespace SpylikeMenus {
	std::shared_ptr<Menu> testMenu() {
		Menu menu(80, 40);
		MenuButton button(Coordinate(4, 4), 10, 5, "hello!", "close");
		MenuButton button2(Coordinate(25, 4), 10, 5, "world!", "testButton2");
		menu.addButton(button);
		menu.addButton(button2);
		return std::make_shared<Menu>(menu);
	}
	std::shared_ptr<Menu> pauseMenu() {
		PauseMenu menu(80, 40);
		MenuButton button(Coordinate(12, 10), 15, 5, "Resume", "close");
		MenuButton button2(Coordinate(45, 10), 15, 5, "Quit", "quit");
		menu.addButton(button);
		menu.addButton(button2);
		return std::make_shared<PauseMenu>(menu);
	}
	std::shared_ptr<Menu> gameOver() {
		GameOverMenu menu(80, 40);
		MenuButton button(Coordinate(12, 10), 15, 5, "Start over", "restart");
		MenuButton button2(Coordinate(45, 10), 15, 5, "Quit", "quit");
		menu.addButton(button);
		menu.addButton(button2);
		return std::make_shared<GameOverMenu>(menu);
	}
	std::shared_ptr<Menu> startMenu() {
		StartMenu menu(80, 40);
		MenuButton button(Coordinate(12, 10), 15, 5, "Start game", "restart");
		MenuButton button2(Coordinate(45, 10), 15, 5, "Quit", "quit");
		menu.addButton(button);
		menu.addButton(button2);
		return std::make_shared<StartMenu>(menu);
	}
	
}

	
