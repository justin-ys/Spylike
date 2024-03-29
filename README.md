# Spylike

**Version**: Alpha 1

A C++ roguelike/platformer game in development using ncurses.
Currently has:
- Rendering interfaces
- Tilemaps
- Events
- Cameras
- Entities
- Drawing & Animation (Sprites)
- Input
- Logging
- 2a03 Audio Engine
- UI/Menu interfaces
- Context Manager for loading levels, pausing and displaying menus
- Level editor & level loading

## Compiling & Running
Spylike currently runs on Windows (using PDcurses) and Linux (through NCurses). The Makefile should auto-detect which backend you need, but you can set the Makefile variable PDCURSES_BACKEND to `wincon`, `sdl2` or `x11` if you wish (or USE_NCURSES=1).
To build, simply run `make` in the root directory. An executable `Spylike-[VERSION]` will be generated. `make debug` will add symbol table information.
