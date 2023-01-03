#include "menus.h"
#include "rendering.h"
#include "event.h"
#include "logger.h"
#include <string>
#include <stdexcept>

extern SpylikeLogger LOGGER;

MenuButton::MenuButton(int width, int height, std::string buttonText, std::string buttonID) : width{width}, height{height}, text{buttonText}, buttonID{buttonID} {}

void MenuButton::click() {
	auto ev = SpylikeEvents::MenuButtonEvent("ButtonClickEvent", buttonID);
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
	Coordinate topLeft = Coordinate(tile->pos.x, tile->pos.y);
	Coordinate bottomRight = Coordinate(tile->pos.x + width - 1, tile->pos.y + height - 1);
	painter.drawBox(topLeft, bottomRight, "UI");
	Coordinate drawOffset = Coordinate(width/2 - drawnText.length()/2 + tile->pos.x, 
		height/2 + tile->pos.y);
	painter.drawString(drawOffset, drawnText, "UI");
}

std::string MenuButton::getButtonID() {
	return buttonID;
}

void MenuButton::on_event(Event& e) {}

void MenuButton::on_update() {}


void Menu::draw(GeometryRenderer& painter) {}

// pos is *relative* to menu
void Menu::addButton(std::shared_ptr<MenuButton> button, Coordinate pos) {	
	if (!tile) {
		throw std::runtime_error("Menu must be registered before adding buttons");
	}
	buttons.insert({button->getButtonID(), button});
	selectionList.push(button->getButtonID());
	world->registerEntity(button, tile->pos + pos);
	addChild(button);
	currentSelection = button->getButtonID();
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
		buttons[currentSelection]->click();
	}
}

void Menu::on_event(Event& e) {}

void Menu::on_update() {}
