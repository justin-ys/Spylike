#include "input.h"
#include "screen.h"
#include "rendering.h"
#include "logger.h"

extern SpylikeLogger LOGGER;

void InputManager::update() {
	char ch = screen.getInput();
	if (ch != '\0') {
		SpylikeEvents::KeyInputEvent ev {"KeyPressEvent", ch};
		manager->emit(ev);
	}
	/*
	MouseEvent mouse = screen.getMouse();
	if (mouse.mouseInput != MouseEvent::MOUSENONE) {
		std::string eventString;
		switch (mouse.mouseInput) {
			case MouseEvent::MOUSELEFT:
				eventString = "MouseLeftEvent";
				break;
			case MouseEvent::MOUSELEFT_RELEASED:
				eventString = "MouseLeftReleasedEvent";
				break;
			case MouseEvent::MOUSERIGHT:
				eventString = "MouseRightEvent";
				break;
			case MouseEvent::MOUSERIGHT_RELEASED:
				eventString = "MouseRightReleasedEvent";
				break;
			case MouseEvent::MOUSEMIDDLE:
				eventString = "MouseMiddleEvent";
				break;
			case MouseEvent::MOUSEMIDDLE_RELEASED:
				eventString = "MouseMiddleReleasedEvent";
				break;
		}
		SpylikeEvents::MouseInputEvent ev {eventString, mouse.x, mouse.y};
		LOGGER.log("Mouse input detected!", DEBUG);
		manager->emit(ev);
	}
	*/
}
			
