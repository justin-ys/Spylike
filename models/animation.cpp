#include "animation.h"
#include "rendering.h"
#include "logger.h"
#include <vector>
#include <string>

extern SpylikeLogger LOGGER;

void Animation::draw(TextRenderManager& painter) {
	int size = frames.size(); // store in int to prevent underflow with size_t
	if (currentFrame >= size) return;
	if (currentFrame == -1 || playTimer.getElapsed() >= delay) {
		playTimer.reset();
		currentFrame++;
		if (currentFrame >= frames.size()) {
			finished = true;
			return;
		}
		painter.drawString(Coordinate(0, 0), frames[currentFrame], layer);
	}
	playTimer.tick();
}

void Animation::reset() {
	playTimer.reset();
	currentFrame = -1;
}
