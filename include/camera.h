#ifndef SPYLIKE_CAMERA_H
#define SPYLIKE_CAMERA_H

#include "rendering.h"
#include "levelmap.h"
#include "screen.h"
#include <vector>


class Camera : public TextRenderManager {
	Coordinate origin;
	int width;
	int height;
	public:
		Camera(TerminalScreen& screen, int width, int height, std::vector<RenderLayer> layers);
		void setOrigin(Coordinate pos);
		Coordinate getOrigin();
		// takes a world coordinate, converts to camera coordinates and draws.
		void draw(Coordinate coord, char c, std::string layerName) override;
};

#endif
