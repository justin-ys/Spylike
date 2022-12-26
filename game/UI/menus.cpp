#include "menus.h"
#include "rendering.h"
#include "event.h"
#include <string>

MenuButton::MenuButton(int width, int height, std::string buttonText, std::string buttonID) : width{width}, height{height}, text{buttonText}, buttonID{buttonID} {}

void MenuButton::click() {
	eventManager->emit(Event("ButtonClickEvent", buttonID));
}

void MenuButton::draw(GeometryRenderer& painter) {
	std::string drawnText = text;
	if (text.length() < width) {
		if (width < 3) {
			drawnText = "";
		}
		else {
			drawnText = text.substr(0, text.length() - width);
			if (drawnText.length() >= 3) {
				drawnText = text.substr(0, text.length() - 3) + "...";
			}
			else {
				drawnText = "";
			}
		}
	}
	
	Coordinate topLeft = Coordinate(tile->pos.x - width/2, tile->pos.y + width/2);
	Coordinate bottomRight = Coordinate(tile->pos.x + width/2, tile->pos.y - width/2);
	painter.drawBox(bottomRight, topLeft, "UI");
	int drawOffset = width/2 - drawnText.length()/2;
	painter.drawString(drawOffset, drawnText, "UI");
}

void MenuButton::on_event(Event e) {}

void MenuButton::on_update() {}
