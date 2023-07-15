CXX=g++
CPPFLAGS=-Iinclude -Ilib/include
LDLIBS=-Llib/bin -lpdcurses
OBJS=graphics/*.cpp logging/*.cpp models/*.cpp level/*.cpp game/*.cpp game/UI/*.cpp util/*.cpp main.cpp
VER=vA1

ifndef PDCURSES_BACKEND
	ifeq ($(OS),Windows_NT)
		PDCURSES_BACKEND=wincon
	else ifeq ($(UNAME_S),Linux)
		PDCURSES_BACKEND=x11
	endif
endif

ifndef PDCURSES_BACKEND
$(error  "OS not detected or supported - please specify PDCurses backend (wincon, x11, sdl2))
endif

build: build-pdcurses
build: $(OBJS)
	$(CXX) $(CPPFLAGS) -o Spylike-$(VER) $(OBJS) $(LDLIBS)

debug: CPPFLAGS+= -g
debug: build

build-pdcurses:
ifeq ($(PDCURSES_BACKEND), wincon)
	cd lib/PDCurses/wincon && $(MAKE)
	cp lib/PDCurses/wincon/pdcurses.a lib/bin/libpdcurses.a
else ifeq ($(PDCURSES_BACKEND), x11)
	./configure
	cd lib/PDCurses/x11 && $(MAKE)
	cp lib/PDCurses/x11/libXcurses.a lib/bin/libpdcurses.a
else ifeq ($(PDCURSES_BACKEND), sdl2)
	cd lib/PDCurses/sdl2 && $(MAKE)
else
	echo "Invalid PDCurses backend specified, options are wincon, x11, sdl2"
	exit 1
endif
	cp lib/PDCurses/curses.h lib/include/curses.h
	cp lib/PDCurses/panel.h lib/include/panel.h
