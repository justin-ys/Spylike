#include "rendering.h"
#include "logger.h"
#include <algorithm>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <cuchar>
#include <cwchar>

extern SpylikeLogger LOGGER;

TextRenderManager::TextRenderManager(TerminalScreen& scr, std::vector<RenderLayer> layers) : screen(scr) {
    std::vector<RenderLayer> layers_sorted = layers;
    sort(layers_sorted.begin(), layers_sorted.end(), 
                                [](const RenderLayer x, const RenderLayer y)
                                {return x.priority < y.priority;});
    for (const RenderLayer layer: layers_sorted)
    {
        orderedLayers.insert(orderedLayers.end(), layer.name);
        #if USE_NCURSESW
        TextLayer layercells = {{Coordinate(0,0), L"\0"}};
        #else
        TextLayer layercells = {{Coordinate(0,0), L'\0'}};
        #endif
        layersCache.insert({layer.name, layercells}); // Create blank layer in cache so we don't have to worry about initalizing it later
    }
}

#ifdef USE_NCURSESW
void TextRenderManager::draw(Coordinate coord, std::wstring c, std::string layerName) {
    if (!locked) {
	    if (coord.x <= screen.width && coord.y <= screen.height) {
		    assert(find(orderedLayers.begin(), orderedLayers.end(), layerName) != orderedLayers.end());
		    TextLayer& layer = layersCache[layerName];
		    //LOGGER.log(std::to_string(coord.x), DEBUG);
		    if (layer[coord] != c) {
		    	layer[coord] = c;
			//toUpdate[coord] = true;
		    }
		    //LOGGER.log("\n" + getSnapshot(), DEBUG);
	    }
    }
}

void TextRenderManager::draw(Coordinate coord, char c, std::string layerName) {
    std::wstring converted(1, L'\0');
    mbstowcs(&converted[0], &c, 1);
    TextRenderManager::draw(coord, converted, layerName);
}

#else
void TextRenderManager::draw(Coordinate coord, char c, std::string layerName) {
    if (!locked) {
	    if (coord.x <= screen.width && coord.y <= screen.height) {
		    assert(find(orderedLayers.begin(), orderedLayers.end(), layerName) != orderedLayers.end());
		    TextLayer& layer = layersCache[layerName];
		    //LOGGER.log(std::to_string(coord.x), DEBUG);
		    if (layer[coord] != c) {
		    	layer[coord] = c;
			//toUpdate[coord] = true;
		    }
		    //LOGGER.log("\n" + getSnapshot(), DEBUG);
	    }
    }
}
#endif

void TextRenderManager::clearLayer(std::string layerName) {
	layersCache.erase(layerName);
	#if USE_NCURSESW
	TextLayer layercells = {{Coordinate(0,0), L"\0"}};
	#else
	TextLayer layercells = {{Coordinate(0,0), '\0'}};
	#endif
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
                #ifdef USE_NCURSESW
                if (node.second != L"\0") {
                #else
                if (node.second != '\0') {
                #endif
                	screen.write(coord.x, coord.y, node.second);
		    //toUpdate.erase(coord);
                }
        }
    }
	//std::string canvas = getSnapshot();
	//screen.write(0, 0, canvas);
	screen.update();
}

#ifdef USE_NCURSESW
std::wstring TextRenderManager::getSnapshot() {
#else
std::string TextRenderManager::getSnapshot() {
#endif
	#ifdef USE_NCURSESW
	std::vector<std::wstring> rows;
	std::wstring brow;
	#else
	std::vector<std::string> rows;
	std::string brow;
	#endif
	for (int j=0; j<screen.width; j++) {
		brow.push_back(' ');
	}
	for (int i=0; i<screen.height; i++) {	
		rows.push_back(brow);
	}
	for (const auto layerName : orderedLayers) {
		for (auto node : layersCache[layerName]) {
			#ifdef USE_NCURSESW
            if (node.second != L"\0") {
            	Coordinate coord = node.first;
				//LOGGER.log(std::to_string(coord.x), DEBUG);
				//LOGGER.log(std::to_string(coord.y), DEBUG);
				rows[coord.y][coord.x] = node.second[0];
            #else
            if (node.second != '\0') {
            	Coordinate coord = node.first;
				//LOGGER.log(std::to_string(coord.x), DEBUG);
				//LOGGER.log(std::to_string(coord.y), DEBUG);
				rows[coord.y][coord.x] = node.second;
            #endif
			}
		}
	}
	#ifdef USE_NCURSESW
    std::wstring snapshot;
    #else
    std::string snapshot;
    #endif
    for (auto row : rows) {
    	#ifdef USE_NCURSESW
    	snapshot += (row + L"\n");
    	#else
    	snapshot += (row + '\n');
    	#endif
    }
    return snapshot;
    
}

int TextRenderManager::getScreenWidth() {
	return screen.width;
}

int TextRenderManager::getScreenHeight() {
	return screen.height;
}

void TextRenderManager::drawString(Coordinate pos, std::string str, std::string layerName) {
	Coordinate currentPos = pos;
	for (const char& c : str) {
		if (c == '\n') {
			currentPos.y += 1;
			currentPos.x = pos.x-1;
		}
		draw(currentPos, c, layerName);
		currentPos.x += 1;
	}
}
		

void TextRenderManager::drawLine(Coordinate p1, Coordinate p2, char c, std::string layerName) {
	int deltaX = p2.x - p1.x;
	int deltaY = p2.y - p1.y;
	int slopeY;
	int signX;
	int signY = deltaY/abs(deltaY);
	// multiply slopeY by 10 to ensure no floating point issues
	if (deltaX == 0) {
		slopeY = 10*deltaY;
		signX = 1;
	}
	else {
		slopeY = abs((10*deltaY)/deltaX)*(signY);
		signX = deltaX/abs(deltaX);
	}
	Coordinate currentPos = p1;
	int yCounter = 0;
	for (int x=0; abs(x)<abs(deltaX)+1; x+=signX) {
		draw(currentPos, c, layerName);
		yCounter += slopeY;
		int lastY = currentPos.y;
		currentPos.y = p1.y + ((yCounter + 5*signY)/10);
		int lastDelta = abs(currentPos.y - lastY);
		if (lastDelta > 1) {
			for (int y=0; y<=lastDelta; y++) {
				draw(Coordinate(currentPos.x, currentPos.y-(y*signY)), c, layerName);
			}
		}
		currentPos.x += signX;
	}
}

void TextRenderManager::drawBox(Coordinate p1, Coordinate p2, std::string layerName) {
	Coordinate p3 = Coordinate(p1.x, p2.y);
	Coordinate p4 = Coordinate(p2.x, p1.y);
	drawLine(p1, p3, '|', layerName);
	drawLine(p2, p4, '|', layerName);
	drawLine(p2, p3, '-', layerName);
	drawLine(p1, p4, '-', layerName);
}
