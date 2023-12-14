#include "camera.h"
#include "rendering.h"
#include "screen.h"
#include "logger.h"
#include "event.h"

extern SpylikeLogger LOGGER;

Camera::Camera(TerminalScreen& screen, int width, int height, std::vector<RenderLayer> layers) : width{width}, height{height}, TextRenderManager(screen, layers) {
	origin = Coordinate(0,0);
}

void Camera::setOrigin(Coordinate pos) {origin = pos;}
void Camera::setOffset(int x, int y) {offsetX = x; offsetY = y;}

Coordinate Camera::getOrigin() {return origin;}

void Camera::draw(Coordinate coord, char c, std::string layerName) {
	if (!absolute) {
		Coordinate cameraMapped(coord.x - origin.x + offsetX, coord.y - origin.y + offsetY);
		if ((cameraMapped.x >= 0) && (cameraMapped.y >= 0)) {
			if ((cameraMapped.x <= width) && (cameraMapped.y <= height)) {
				TextRenderManager::draw(cameraMapped, c, layerName);
			}
		}
	}
	else drawAbsolute(coord, c, layerName);
}

void Camera::drawAbsolute(Coordinate coord, char c, std::string layerName) {
	TextRenderManager::draw(coord, c, layerName);
}

void Camera::toggleAbsolute() {
	if (absolute) absolute = false;
	else absolute = true;
}

void Camera::on_event(Event& e) {
	if (e.type == "CAMERA_MoveUp") {
		setOrigin(Coordinate(origin.x, origin.y-1));
	}
	if (e.type == "CAMERA_MoveDown") {
		setOrigin(Coordinate(origin.x, origin.y+1));
	}
	if (e.type == "CAMERA_MoveRight") {
		setOrigin(Coordinate(origin.x+1, origin.y));
	}
	if (e.type == "CAMERA_MoveLeft") {
		setOrigin(Coordinate(origin.x-1, origin.y));
	}
	if (e.type == "CAMERA_Move") {
		SpylikeEvents::CameraEvent& ce = dynamic_cast<SpylikeEvents::CameraEvent&>(e);
		setOrigin(ce.pos);
	}
}
	
