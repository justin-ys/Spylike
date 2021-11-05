#include "game.h"

#include "logger.h"
extern SpylikeLogger LOGGER("log.txt", DEBUG);

int main() {
	Game::run();
	return 0;
}
