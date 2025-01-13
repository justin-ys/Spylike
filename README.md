# Retrograde (codename "Spylike")

A C++ roguelike/platformer game and engine. Demo available at https://jm-5.itch.io/retrograde
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
- Level editor (through Tile) & level loading

## Compiling & Running
Spylike currently runs on Windows (using PDcurses) and Linux (through NCurses). The Makefile should auto-detect which backend you need, but you can set the Makefile variable PDCURSES_BACKEND to `wincon`, `sdl2` or `x11` if you wish (or USE_NCURSES=1).
To build, simply run `make` in the root directory. An executable `Spylike-[VERSION]` will be generated. `make debug` will add symbol table information.
Spylike uses the [miniaudio](https://github.com/mackron/miniaudio) library for audio playing, which should work on nearly any system.

## Level Editing
Spylike levels were made in [Tiled](https://www.mapeditor.org/). Load this file as a custom export format into Tiled: https://gist.github.com/justin-ys/8b96e6a9c39bd4ee76a7b4230e60fb8d, then load the tiles from the "Spylike Tiles" tsx file found in the game directory. To transition between levels, add a "LevelTrans" tile, and set its "transition" custom property to the *filename* (WITHOUT extension/full path) of the level you want to transition to when the player reaches that tile. Levels should be placed in game/resource/levels.

## Discord RPC support
Spylike has Discord RPC integreation! Currently, this is only implemented for Linux. Use the USE_DISCORD=1 flag when compiling to enable it.
