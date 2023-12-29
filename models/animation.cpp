#include "animation.h"
#include "rendering.h"
#include "logger.h"
#include <vector>
#include <string>

extern SpylikeLogger LOGGER;

void Animation::draw(TextRenderManager& painter) {
	int size = frames.size(); // i have to store this in a variable first because otherwise... the comparison doesn't work. like, it will think 2 >= -1. beats me.
	if (currentFrame >= size) return;
	if (currentFrame == -1 || playTimer.getElapsed() >= delay) {
		playTimer.reset();
		currentFrame++;
		if (currentFrame >= frames.size()) {
			finished = true;
			return;
		}
		LOGGER.log(frames[currentFrame], DEBUG);
		painter.drawString(Coordinate(0, 0), frames[currentFrame], layer);
	}
	playTimer.tick();
}

void Animation::reset() {
	playTimer.reset();
	currentFrame = -1;
}
