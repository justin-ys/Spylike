#ifndef SPYLIKE_CAMERA_H
#define SPYLIKE_CAMERA_H

#include "rendering.h"
#include "levelmap.h"
#include "screen.h"
#include <vector>

class Camera : public TextRenderManager {
	Coordinate origin;
	LevelMap& map;
	int width;
	int height;
	public:
		Camera(TerminalScreen& screen, LevelMap& map, int width, int height, std::vector<RenderLayer> layers);
		void setOrigin(Coordinate pos);
		Coordinate getOrigin();
		virtual void draw(Coordinate coord, char c, std::string layerName);
};

#endif
