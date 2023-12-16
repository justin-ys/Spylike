#ifndef SPYLIKE_RENDER_MANAGER_H
#define SPYLIKE_RENDER_MANAGER_H

#include "screen.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <tuple>
#include <functional>

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
	bool operator==(const Coordinate& c) const {
		return ((x == c.x) && (y == c.y));
	}
	bool operator!=(const Coordinate& c) const {
		return ((x != c.x) || (y != c.y));
	}
};

struct RenderLayer {
	//const?
    std::string name;
    int priority;
    RenderLayer(std::string name, int priority): name(name), priority(priority) {}
};
  
struct coordHash {
    std::size_t operator()(const Coordinate& c) const {
    	return (std::hash<int>{}(c.x) ^ std::hash<int>{}(c.y));
	}	
};
	

class TextRenderManager {
    TerminalScreen& screen;
    typedef std::unordered_map<Coordinate, char, coordHash> TextLayer;
    std::unordered_map<std::string, TextLayer> layersCache;
    //std::unordered_map<Coordinate, bool, coordHash> toUpdate;
    std::vector<std::string> orderedLayers;
    bool locked = false;
    public:
        TextRenderManager(TerminalScreen& screen, std::vector<RenderLayer> layers);
        virtual void draw(Coordinate coord, char c, std::string layerName);
        void renderToScreen();
	void clearLayer(std::string layerName);
        void clearCache();
        void clearScreen();
	void lock() { locked = true; } // prevents any new draws from taking effect - pauses the camera
	void unlock() { locked = false; }
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
		int getScreenWidth();
		int getScreenHeight();
};

#endif
