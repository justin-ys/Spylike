CXX=g++
CPPFLAGS=-std=c++2a -Iinclude -Igame/include -Ilib/include -DMA_NO_PULSEAUDIO
LDLIBS=-Llib/bin
SOURCES=$(wildcard graphics/*.cpp logging/*.cpp models/*.cpp level/*.cpp audio/*.cpp game/*.cpp game/entities/*.cpp game/UI/*.cpp util/*.cpp main.cpp)
OBJS=$(patsubst %.cpp, build/%.o, $(SOURCES))
VER=vA1
USE_DISCORD=1

ifndef PDCURSES_BACKEND
	ifeq ($(OS),Windows_NT)
		PDCURSES_BACKEND=wincon
	else
		UNAME_S := $(shell uname -s)
		ifeq ($(UNAME_S),Linux)
			ifndef USE_NCURSES
				USE_NCURSES=1
				CPPFLAGS+= -DUSE_NCURSESW
			endif
			PDCURSES_BACKEND=x11
			LDLIBS+= -lpthread -lm -ldl
		endif
	endif
endif

ifeq ($(USE_NCURSES), 1)
	LDLIBS+= -lncursesw
else
	LDLIBS+= -lpdcurses
endif

ifndef PDCURSES_BACKEND
	$(error  "OS not detected or supported - please specify PDCurses backend (wincon, x11, sdl2))
endif

ifeq ($(USE_DISCORD), 1)
    $(shell mkdir -p lib/bin)
    $(shell mkdir -p lib/include)
    ifeq ($(UNAME_S),Linux)
        $(shell cp -u lib/discord-rpc-linux/linux-dynamic/include/*.h lib/include)
        $(shell cp -u lib/discord-rpc-linux/linux-dynamic/lib/libdiscord-rpc.so lib/bin)
        LDLIBS+= -ldiscord-rpc
        CPPFLAGS+= -DUSE_DISCORD
    else
        USE_DISCORD=0
    endif
endif

$(shell mkdir -p lib/include)
$(shell cp lib/miniaudio/miniaudio.h lib/include/miniaudio.h)
		
build: build-pdcurses
build: $(OBJS)
	$(CXX) $(CPPFLAGS) -o Spylike-$(VER) $(OBJS) $(LDLIBS) -Wl,-rpath=lib/bin
	
build/%.o: %.cpp
	mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) -c -o $@ $(LDLIBS) $^

debug: CPPFLAGS+= -g -O0 -v
debug: build

clean:
	rm -rf lib/bin/*
	rm -rf lib/include/*
	rm -rf build/*
	rm -rf lib/src/*

build-pdcurses:
	cd lib && mkdir -p bin
	cd lib && mkdir -p include
ifeq ($(USE_NCURSES), 1)
	echo "Building with ncurses - skipping PDCurses build"
else # BEGIN PDCURSES BUILD BLOCK
	cp lib/PDCurses/curses.h lib/include/curses.h
	cp lib/PDCurses/panel.h lib/include/panel.h
    ifeq ($(PDCURSES_BACKEND), wincon)
	    cd lib/PDCurses/wincon && $(MAKE) UTF8=Y WIDE=Y
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
endif # END PDCURSES BUILD BLOCK
