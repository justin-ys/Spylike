#include "game.h"
//#include "navier.h"
#include <memory>

#include "logger.h"
extern SpylikeLogger LOGGER("log.txt", DEBUG);

int main() {
	std::shared_ptr<GameManager> manager = std::make_shared<GameManager>();
	manager->run();
	return 0;
}
