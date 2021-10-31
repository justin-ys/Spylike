#include "rendering.h"
#include <algorithm>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <algorithm>

TextRenderManager::TextRenderManager(TerminalScreen& scr, std::vector<RenderLayer> layers) : screen(scr) {
    std::vector<RenderLayer> layers_sorted = layers;
    sort(layers_sorted.begin(), layers_sorted.end(), 
                                [](const RenderLayer x, const RenderLayer y)
                                {return x.priority < y.priority;});
    for (const RenderLayer layer: layers_sorted)
    {
        orderedLayers.insert(orderedLayers.end(), layer.name);
        TextLayer layercells = {{Coordinate(0,0), '\0'}};
        layersCache.insert({layer.name, layercells}); // Create blank layer in cache so we don't have to worry about initalizing it later
    }
}
    
    
void TextRenderManager::draw(Coordinate coord, char c, std::string layerName) {
    assert(find(orderedLayers.begin(), orderedLayers.end(), layerName) != orderedLayers.end());
    TextLayer& layer = layersCache[layerName];
    layer[coord] = c;
}

void TextRenderManager::clearLayer(std::string layerName) {
	layersCache.erase(layerName);
	TextLayer layercells = {{Coordinate(0,0), '\0'}};
	layersCache.insert({layerName, layercells});
}

void TextRenderManager::clearCache() { 
    for (const std::string layerName : orderedLayers) {
        clearLayer(layerName);
    }
}

void TextRenderManager::clearScreen() {
	clearCache();
	screen.clear();
}

void TextRenderManager::renderToScreen() {
    for (const std::string layerName : orderedLayers) {
        for (const auto node : layersCache[layerName]) {
                Coordinate coord = node.first;
                if (node.second != '\0') {
                    screen.write(coord.x, coord.y, node.second);
                }
        }
    }
}

int TextRenderManager::getScreenWidth() {
	return screen.width;
}

int TextRenderManager::getScreenHeight() {
	return screen.height;
}


GeometryRenderer::GeometryRenderer(TextRenderManager& renderManager) : manager(renderManager) {}

void GeometryRenderer::drawString(Coordinate pos, std::string str, std::string layerName) {
	Coordinate currentPos = pos;
	for (const char& c : str) {
		if (c == '\n' || currentPos.x > manager.getScreenWidth()) {
			currentPos.y += 1;
			currentPos.x = pos.x;
		}
		manager.draw(currentPos, c, layerName);
		currentPos.x += 1;
	}
	manager.renderToScreen();
}
		

void GeometryRenderer::drawLine(Coordinate p1, Coordinate p2, char c, std::string layerName) {
	int deltaX = (p2.x - p1.x);
	int deltaY = (p2.y - p1.y);
	int slopeX = deltaX/std::__gcd(deltaX, deltaY);
	int slopeY = deltaY/std::__gcd(deltaX, deltaY);
	Coordinate currentPos = p1;
	while (true) {
		manager.draw(currentPos, c, layerName);
		currentPos.x += slopeX;
		currentPos.y += slopeY;
		if ((abs(currentPos.x - p1.x) > abs(deltaX)) || (abs(currentPos.y) - abs(p1.y)) > abs(deltaY)) {
			break;
		}
	}
	manager.renderToScreen();
}


