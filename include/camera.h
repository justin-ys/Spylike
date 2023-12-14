#ifndef SPYLIKE_CAMERA_H
#define SPYLIKE_CAMERA_H

#include "rendering.h"
#include "levelmap.h"
#include "screen.h"
#include "event.h"
#include <vector>


class Camera : public TextRenderManager, public EventHandler {
	Coordinate origin;
	int width;
	int height;
	int offsetX=0;
	int offsetY=0;
	bool absolute=false;
	public:
		Camera(TerminalScreen& screen, int width, int height, std::vector<RenderLayer> layers);
		void setOrigin(Coordinate pos);
		void setOffset(int x, int y);
		int getXOffset() { return offsetX; }
		int getYOffset() { return offsetY; }
		Coordinate getOrigin();
		// takes a world coordinate, converts to camera coordinates and draws.
		void draw(Coordinate coord, char c, std::string layerName) override;
		void drawAbsolute(Coordinate coord, char c, std::string layerName);
		void toggleAbsolute();
		void on_event(Event& e) override;
};

#endif
