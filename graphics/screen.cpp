#include <curses.h>
#include <iostream>
#include <cassert>
#include "screen.h"
#include "logger.h"

extern SpylikeLogger LOGGER;

NcursesTerminalScreen::NcursesTerminalScreen(int w, int h) {
	width = w;
	height = h;
    initscr();
    cbreak();
    noecho();
    refresh();
    win = newwin(0, 0, 0, 0);
    wrefresh(win);
    nodelay(win, TRUE);
    mousemask(ALL_MOUSE_EVENTS, NULL);
}

void NcursesTerminalScreen::write(int x, int y, char c) {
	assert(x <= width);
    assert(y <= height);
    mvwaddch(win, y, x, c);
	wrefresh(win);
}

void NcursesTerminalScreen::write(int x, int y, std::string s) {
	assert(x <= width);
    assert(y <= height);
    const char* charStr = s.c_str();
    mvwprintw(win, y, x, charStr);
	wrefresh(win);
}

char NcursesTerminalScreen::getInput() {
	int ch = wgetch(win);
	if ((ch == ERR) || (ch == KEY_MOUSE)) {
		return '\0';
	}
	//clear buffer, flushinp does not appear to work
	while (true) {
		if (wgetch(win) == ERR) {
			break;
		}
	}
	return static_cast<char>(ch);
}

// Does not seem to be compatible with most terminals
/*
MouseEvent NcursesTerminalScreen::getMouse() {
	MEVENT mevent;
	MouseEvent mouseEvent;
	int mouseClick = wgetch(win);
	if (mouseClick != KEY_MOUSE) {
		mouseEvent.mouseInput = MouseEvent::MOUSENONE;
		return mouseEvent;
	}
	
	if (getmouse(&mevent) == OK) {
		LOGGER.log("here", DEBUG);
		while (true) {
			if (wgetch(win) == ERR) {
					break;
			}
		}
		mouseEvent.x = mevent.x;
		mouseEvent.y = mevent.y;
		if (mevent.bstate & BUTTON1_PRESSED) {
			mouseEvent.mouseInput = MouseEvent::MOUSELEFT;
		}
		else if (mevent.bstate & BUTTON1_RELEASED) {
			mouseEvent.mouseInput = MouseEvent::MOUSELEFT_RELEASED;
		}
		else if (mevent.bstate & BUTTON2_PRESSED) {
			mouseEvent.mouseInput = MouseEvent::MOUSERIGHT;
		}
		else if (mevent.bstate & BUTTON2_RELEASED) {
			mouseEvent.mouseInput = MouseEvent::MOUSERIGHT_RELEASED;
		}
		else if (mevent.bstate & BUTTON3_CLICKED) {
			mouseEvent.mouseInput = MouseEvent::MOUSEMIDDLE;
		}
		else if (mevent.bstate & BUTTON3_RELEASED) {
			mouseEvent.mouseInput = MouseEvent::MOUSEMIDDLE_RELEASED;
		}
		else {
			mouseEvent.mouseInput = MouseEvent::MOUSENONE;
		}
	}
	return mouseEvent;
}

char* NcursesTerminalScreen::read(int x, int y) {
    cchar_t* cell;
    in_wch(cell);
    return (char*)cell;
}
*/
    
void NcursesTerminalScreen::clear() {
    werase(win);
	//wrefresh(win);
}

void NcursesTerminalScreen::end() { 
    endwin();
}
