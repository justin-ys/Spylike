#ifndef SPYLIKE_SPRITES_H
#define SPYLIKE_SPRITES_H

#include <string>
#include <vector>

struct SpriteDelta {
	int startPos;
	std::string data;
	SpriteDelta(int startPos, std::string data) : startPos{startPos}, data{data} {}
};

typedef std::vector<SpriteDelta> SpriteFrame;

class Sprite {
	std::vector<SpriteFrame> frames;
	int spriteSize;
	std::string currentFrame;
	std::string firstFrame;
	std::string prevFrameCache;
	int currentFrameIndex;
	public:
		Sprite(std::vector<SpriteFrame> frames, int size);
		void nextFrame();
		void prevFrame();
		std::string getCurrentFrame();
};
		
#endif
