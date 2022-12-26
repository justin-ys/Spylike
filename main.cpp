#include "game.h"
//#include "navier.h"

#include "logger.h"
extern SpylikeLogger LOGGER("log.txt", DEBUG);

int main() {
	Game::run();
	//run();
	return 0;
}
