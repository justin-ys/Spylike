#include <iostream>
#include "rendering.h"
#include "camera.h"
#include "screen.h"
#include "sprites.h"
#include "levelmap.h"
#include "event.h"
#include "logger.h"
using namespace std;
#include <vector>
#include <string>
#include <memory>
#include <unistd.h>
#include <string>

extern SpylikeLogger LOGGER;

class epicEntity : public TileEntity {
	Sprite sprite;
	public:
		epicEntity(Sprite sprite) : sprite(sprite) {}
		void on_update() {
			sprite.nextFrame();
		}
		void draw(GeometryRenderer& painter) {
			painter.drawString(tile->pos, sprite.getCurrentFrame(), "pog");
		}
		
		void on_event(Event e) {}
};

namespace Game {
	void run() {
		SpriteDelta d1 = {0, "lol"};
		SpriteDelta d2 = {1, "e"};
		SpriteDelta d3 = {0, "p"};
		SpriteDelta d4 = {2, "gg"};
		SpriteDelta d5 = {0, "o\b"};
		SpriteFrame f1 = {d1};
		SpriteFrame f2 = {d2};
		SpriteFrame f3 = {d3, d4};
		SpriteFrame f4 = {d5};
		
		vector<SpriteFrame> frames = {f1, f2, f3, f4};
		Sprite coolSprite(frames, 4);
		
		vector<RenderLayer> layers = {RenderLayer("pog", 1)};
		NcursesTerminalScreen screen(60, 20);
		
		Camera camera(screen, 60, 20, layers);
		GeometryRenderer renderer(camera);
		
		std::shared_ptr<EventManager> manager(new EventManager());
		std::shared_ptr<epicEntity> ent = std::make_shared<epicEntity>(coolSprite);
		
		IDBlock idAllocation = {0, 1024};
		std::shared_ptr<LevelMap> map = std::make_shared<LevelMap>(60, 20, manager, idAllocation);
		map->registerEntity(ent, Coordinate(5,5));
		//for (int i=0; i<4; i++) {
		bool flag = true;
		while (true) {
			//camera.setOrigin(Coordinate(camera.getOrigin().x + 1, camera.getOrigin().y + 1));
			camera.clearScreen();
			char c = screen.getInput();
			for (int y=0; y<20; y++) {
				for (int x=0; x<60; x++) {
					map->updateTile(Coordinate(x, y));
					if (c != '\0') {
						map->drawTile(Coordinate(x, y), renderer);
						if (c == 'z' && flag) {
							Coordinate entPos = ent->tile->pos;
							map->moveEntity(ent->getID(), Coordinate(entPos.x+5, entPos.y+4));
							LOGGER.log(std::to_string(entPos.x), DEBUG);
							flag = false;
						}
					}
				}
			}
			usleep(100000);
		}
		screen.end();
	}
}
