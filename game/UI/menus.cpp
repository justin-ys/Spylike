#include "menus.h"
#include "rendering.h"
#include "event.h"
#include "logger.h"
#include <string>

extern SpylikeLogger LOGGER;

MenuButton::MenuButton(int width, int height, std::string buttonText, std::string buttonID) : width{width}, height{height}, text{buttonText}, buttonID{buttonID} {}

void MenuButton::click() {
	//eventManager->emit(Event("ButtonClickEvent", buttonID));
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
	//LOGGER.log(std::to_string(tile->pos.x), DEBUG);
	Coordinate topLeft = Coordinate(tile->pos.x, tile->pos.y);
	Coordinate bottomRight = Coordinate(tile->pos.x + width - 1, tile->pos.y + height - 1);
	painter.drawBox(topLeft, bottomRight, "UI");
	Coordinate drawOffset = Coordinate(width/2 - drawnText.length()/2 + tile->pos.x, 
		height/2 + tile->pos.y);
	painter.drawString(drawOffset, drawnText, "UI");
}

void MenuButton::on_event(Event& e) {}

void MenuButton::on_update() {}
