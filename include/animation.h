#ifndef SPYLIKE_ANIMATION_H
#define SPYLIKE_ANIMATION_H

#include "rendering.h"
#include "objects.h"
#include "timer.h"
#include <string>

class Animation : public SpritedObject {
	std::vector<std::string> frames;
	int currentFrame = -1;
	int delay; // frames
	bool finished = false;
	std::string layer;
	Timer playTimer;
	public:
		Animation(std::vector<std::string> frames, int delay, std::string layer) : frames{frames}, delay{delay}, layer{layer} {}
		void draw(TextRenderManager& painter);
		void reset();
		bool isFinished() { return finished; }
};
		

#endif
