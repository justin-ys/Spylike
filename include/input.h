#ifndef SPYLIKE_INPUT_H
#define SPYLIKE_INPUT_H

#include "event.h"
#include "screen.h"

class InputManager {
	std::shared_ptr<EventManager> manager;
	TerminalScreen& screen;
	public:
		InputManager(std::shared_ptr<EventManager> manager, TerminalScreen& screen): manager{manager}, screen{screen} {}
		void update();
};

#endif
