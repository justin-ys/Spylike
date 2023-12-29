#ifndef SPYLIKE_ANIMATION_H
#define SPYLIKE_ANIMATION_H

#include "rendering.h"
#include "objects.h"
#include "timer.h"
#include "logger.h"
#include <string>

extern SpylikeLogger LOGGER;

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

inline Animation load_anim_from_file(std::string path, int delay, std::string layer) {
	LOGGER.log("Reading animation from " + path, DEBUG);
	std::ifstream input(path);
	std::string heightStr;	
	getline(input, heightStr);
	int height = std::stoi(heightStr);
	std::vector<std::string> frames;
	std::string entLine;
	std::string frame;
	std::string line;
	int rows=0;
	while (getline(input, line)) {
		frame += line + "\n";
		rows++;
		if (rows >= height) {
			frames.push_back(frame);
			frame = "";
			rows = 0;
			getline(input, line); // blank line delimeter;
		}
	}
	return Animation(frames, delay, layer);
}

#endif
