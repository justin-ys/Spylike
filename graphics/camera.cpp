#include "camera.h"
#include "rendering.h"
#include "screen.h"
#include "logger.h"

extern SpylikeLogger LOGGER;

Camera::Camera(TerminalScreen& screen, int width, int height, std::vector<RenderLayer> layers) : width{width}, height{height}, TextRenderManager(screen, layers) {
	origin = Coordinate(0,0);
}

void Camera::setOrigin(Coordinate pos) {origin = pos;}

Coordinate Camera::getOrigin() {return origin;}

void Camera::draw(Coordinate coord, char c, std::string layerName) {
	Coordinate cameraMapped(coord.x - origin.x, coord.y - origin.y);
	if ((cameraMapped.x >= 0) && (cameraMapped.y >= 0)) {
		if ((cameraMapped.x <= width) && (cameraMapped.y <= height)) {
			TextRenderManager::draw(cameraMapped, c, layerName);
		}
	}
}
	
