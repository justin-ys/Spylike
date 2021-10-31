#include "sprites.h"
#include <stdexcept>
#include <string>
#include <vector>

Sprite::Sprite(std::vector<SpriteFrame> frames, int size) : frames{frames}, spriteSize{size} {
	if (frames[0].size() != 1) {
		throw std::invalid_argument("First frame should only have one delta");
	}
	
	if (frames[0][0].data.length() > spriteSize) {
		throw std::invalid_argument("Invalid delta size");
	}
	currentFrameIndex = 0;
	firstFrame = frames[0][0].data;
	currentFrame = firstFrame;
}

void Sprite::nextFrame() {
	currentFrameIndex += 1;
	if (currentFrameIndex == frames.size()) {
		currentFrame = firstFrame;
		currentFrameIndex = 0;
	}
	else {
		prevFrameCache = currentFrame;
		for (SpriteDelta delta : frames[currentFrameIndex]) {
			if (delta.startPos + delta.data.length() > spriteSize) {
				throw std::invalid_argument("Invalid delta size");
			}
			for (int i=0; i<delta.data.length(); i++) {
				if (delta.data[i] == '\b') {
					currentFrame.erase(delta.startPos+i);
					break;
				}
				if (delta.startPos+i >= currentFrame.length()) {
					currentFrame += delta.data[i];
				}
				else {
					currentFrame[delta.startPos+i] = delta.data[i];
				}
			}
		}
	}
}

void Sprite::prevFrame() {
	if (!(currentFrameIndex == 0)) {
		if (currentFrame == prevFrameCache) {
			currentFrame = firstFrame;
			currentFrameIndex = 0;
			int prevFrameIdx = currentFrameIndex - 1;
			for (int i=0; i==prevFrameIdx; i++) {
				nextFrame();
			}
		}
		else {
			currentFrameIndex = 0;
			currentFrame = prevFrameCache;
		}
	}
}

std::string Sprite::getCurrentFrame() {
	return currentFrame;
}	
