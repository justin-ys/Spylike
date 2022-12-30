#ifndef SPYLIKE_SCREENRENDERING_H
#define SPYLIKE_SCREENRENDERING_H

#include <ncurses.h>
#include <string>

struct MouseEvent {
	enum {MOUSELEFT, MOUSEMIDDLE, MOUSERIGHT, MOUSELEFT_RELEASED, MOUSEMIDDLE_RELEASED, MOUSERIGHT_RELEASED, MOUSENONE} mouseInput;
	int x;
	int y;
};

class TerminalScreen {
    public:
        int width;
        int height;
        virtual void write(int x, int y, char c) = 0;
        virtual void write(int x, int y, std::string s) = 0;
        //virtual char* read(int x, int y) = 0; 
        virtual void clear() = 0;
        virtual void end() = 0;
        char getInput();
        //virtual MouseEvent getMouse() = 0;
};

class NcursesTerminalScreen : public TerminalScreen {
    WINDOW* win;
    public:
        NcursesTerminalScreen(int w, int h);
        void write(int x, int y, char c);
        void write(int x, int y, std::string s);
        //char* read(int x, int y);
        char getInput();
        //MouseEvent getMouse();
        void clear();
        void end();
};
    
#endif       
