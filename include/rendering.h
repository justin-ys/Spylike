#ifndef SPYLIKE_RENDER_MANAGER_H
#define SPYLIKE_RENDER_MANAGER_H

#include "screen.h"
#include <map>
#include <string>
#include <vector>
#include <tuple>

struct Coordinate {
    int x; 
    int y;
    Coordinate(int x=0, int y=0): x(x), y(y) {}
};

struct RenderLayer {
    std::string name;
    int priority;
    RenderLayer(std::string name, int priority): name(name), priority(priority) {}
};

struct coordCompare : public std::binary_function<std::string, std::string, bool> {
    bool operator()(const Coordinate& c1, const Coordinate& c2) const {
        if (c1.y < c2.y) {
            return false;
        }
        else if (c1.y > c2.y) {
            return true;
        }
        else if (c1.x < c2.x) {
            return false;
        }
        else {
            return true;
        }
    }
};

class TextRenderManager {
    TerminalScreen& screen;
    typedef std::map<Coordinate, char, coordCompare> TextLayer;
    std::map<std::string, TextLayer> layersCache;
    std::vector<std::string> orderedLayers;
    public:
        TextRenderManager(TerminalScreen& screen, std::vector<RenderLayer> layers);
        virtual void draw(Coordinate coord, char c, std::string layerName);
        void renderToScreen();
		void clearLayer(std::string layerName);
        void clearCache();
        void clearScreen();
        int getScreenWidth();
        int getScreenHeight();
        
};

class GeometryRenderer {
	TextRenderManager& manager;
	public:
		GeometryRenderer(TextRenderManager& renderManager);
		void drawString(Coordinate pos, std::string str, std::string layerName);
		void drawLine(Coordinate p1, Coordinate p2, char c, std::string layerName);
};

#endif
