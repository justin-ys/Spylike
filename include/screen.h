#ifndef SPYLIKE_SCREENRENDERING_H
#define SPYLIKE_SCREENRENDERING_H

#include <ncurses.h>
#include <string>

class TerminalScreen {
    public:
        int width;
        int height;
        virtual void write(int x, int y, char c) = 0;
        virtual void write(int x, int y, std::string s) = 0;
        //virtual char* read(int x, int y) = 0; 
        virtual void clear() = 0;
        virtual void end() = 0;
};

class NcursesTerminalScreen : public TerminalScreen {
    WINDOW* win;
    public:
        NcursesTerminalScreen(int w, int h);
        void write(int x, int y, char c);
        void write(int x, int y, std::string s);
        //char* read(int x, int y);
        char getInput();
        void clear();
        void end();
};
    
#endif       
