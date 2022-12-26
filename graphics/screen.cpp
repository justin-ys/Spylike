#include <ncurses.h>
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
	if (ch == ERR) {
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

/*
char* NcursesTerminalScreen::read(int x, int y) {
    cchar_t* cell;
    in_wch(cell);
    return (char*)cell;
}
*/
    
void NcursesTerminalScreen::clear() {
    werase(win);
	wrefresh(win);
}

void NcursesTerminalScreen::end() { 
    endwin();
}
