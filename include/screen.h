#ifndef SPYLIKE_SCREENRENDERING_H
#define SPYLIKE_SCREENRENDERING_H

#include <curses.h>
#include <string>

struct MouseEvent {
	enum {MOUSELEFT, MOUSEMIDDLE, MOUSERIGHT, MOUSELEFT_RELEASED, MOUSEMIDDLE_RELEASED, MOUSERIGHT_RELEASED, MOUSENONE} mouseInput;
	int x;
	int y;
};

class TerminalScreen {
    public:
        const int width;
        const int height;
	TerminalScreen(int w, int h) : width{w}, height{h} {}
        virtual void write(int x, int y, char c) = 0;
        virtual void write(int x, int y, std::string s) = 0;
	virtual void update() = 0;
        //virtual char* read(int x, int y) = 0; 
        virtual void clear() = 0;
        virtual void end() = 0;
        virtual char getInput() = 0;
        //virtual MouseEvent getMouse() = 0;
};

class NcursesTerminalScreen : public TerminalScreen {
    WINDOW* win;
    WINDOW* inputWin;
    public:
        NcursesTerminalScreen(int w, int h);
        void write(int x, int y, char c);
        void write(int x, int y, std::string s);
	void update();
        //char* read(int x, int y);
        char getInput();
        //MouseEvent getMouse();
        void clear();
        void end();
};
    
#endif       
