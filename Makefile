CXX=g++
CPPFLAGS=-std=c++11 -Iinclude
LDLIBS=
OBJS=graphics/*.cpp logging/*.cpp models/*.cpp level/*.cpp game/*.cpp game/UI/*.cpp util/*.cpp
VER=vA1

ifndef PDCURSES_BACKEND
	ifeq ($(OS),Windows_NT)
		PDCURSES_BACKEND=wincon
	else
		UNAME_S := $(shell uname -s)
		ifeq ($(UNAME_S),Linux)
			ifndef USE_NCURSES
				USE_NCURSES=1
			endif
			PDCURSES_BACKEND=x11
		endif
	endif
endif

ifeq ($(USE_NCURSES), 1)
	LDLIBS+= -lncurses
else
	LDLIBS+= -Llib/bin -lpdcurses
    CPPFLAGS+= -Ilib/include
endif

ifndef PDCURSES_BACKEND
	$(error  "OS not detected or supported - please specify PDCurses backend (wincon, x11, sdl2))
endif

build: build-pdcurses
build: $(OBJS)
	$(CXX) $(CPPFLAGS) -o Spylike-$(VER) $(OBJS) main.cpp $(LDLIBS)

test: build-pdcurses
test: $(OBJS)
	$(CXX) $(CPPFLAGS) -Itest/include -o Spylike-TestSuite-$(VER) $(OBJS) test/unit/*.cpp test/test.cpp $(LDLIBS)

debug: CPPFLAGS+= -g
debug: build

build-pdcurses:
	cd lib && mkdir -p bin
	cd lib && mkdir -p include
ifeq ($(USE_NCURSES), 1)
	echo "Building with ncurses - skipping PDCurses build"
else
	cp lib/PDCurses/curses.h lib/include/curses.h
	cp lib/PDCurses/panel.h lib/include/panel.h
    ifeq ($(PDCURSES_BACKEND), wincon)
	    cd lib/PDCurses/wincon && $(MAKE)
	    cp lib/PDCurses/wincon/pdcurses.a lib/bin/libpdcurses.a
    else ifeq ($(PDCURSES_BACKEND), x11)
	    cd lib/PDCurses/x11 && ./configure
	    cd lib/PDCurses/x11 && $(MAKE)
	    cp lib/PDCurses/x11/libXCurses.a lib/bin/libpdcurses.a
        LDLIBS+= -lX11 -lXt -lXaw -lXmu -lXext -lXpm -lstdc++
    else ifeq ($(PDCURSES_BACKEND), sdl2)
	    cd lib/PDCurses/sdl2 && $(MAKE)
    else
	    echo "Invalid PDCurses backend specified, options are wincon, x11, sdl2"
	    exit 1
    endif
endif
