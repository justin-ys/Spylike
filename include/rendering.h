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
    Coordinate operator+(const Coordinate& c) const {
		int nx = x + c.x;
		int ny = y + c.y;
		return Coordinate(nx, ny);
	}
	Coordinate operator-(const Coordinate& c) const {
		int nx = x - c.x;
		int ny = y - c.y;
		return Coordinate(nx, ny);
	}
	Coordinate operator*(const Coordinate& c) const {
		int nx = x * c.x;
		int ny = y * c.y;
		return Coordinate(nx, ny);
	}
};

struct RenderLayer {
	//const?
    std::string name;
    int priority;
    RenderLayer(std::string name, int priority): name(name), priority(priority) {}
};
  
struct coordCompare : public std::binary_function<std::string, std::string, bool> {
	//TODO: literally stolen LMAO
    bool operator()(const Coordinate& c1, const Coordinate& c2) const {
    	return (c1.x < c2.x) || ((c1.x == c2.x) && (c1.y < c2.y));
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
        std::string getSnapshot();
        
};

class GeometryRenderer {
	protected:
		TextRenderManager& manager;
	public:
		GeometryRenderer(TextRenderManager& renderManager);
		void draw(Coordinate coord, char c, std::string layerName);
		void drawString(Coordinate pos, std::string str, std::string layerName);
		void drawLine(Coordinate p1, Coordinate p2, char c, std::string layerName);
		void drawBox(Coordinate p1, Coordinate p2, std::string layerName);
};

#endif
