CXX=g++
CPPFLAGS=-Iinclude
LDLIBS=-lncurses
OBJS=graphics/*.cpp logging/*.cpp models/*.cpp level/*.cpp game/*.cpp game/UI/*.cpp navier/*.cpp main.cpp
VER=vA1

build: $(OBJS)
	$(CXX) $(CPPFLAGS) -o Spylike-$(VER) $(OBJS) $(LDLIBS)
	
debug: $(OBJS)
	$(CXX) $(CPPFLAGS) -o Spylike-$(VER) $(OBJS) $(LDLIBS) -g
