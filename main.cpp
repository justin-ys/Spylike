#include "game.h"
//#include "navier.h"

#include "logger.h"
extern SpylikeLogger LOGGER("log.txt", DEBUG);

int main() {
	GameManager manager;
	manager.run();
	return 0;
}
