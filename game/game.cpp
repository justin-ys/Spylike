#include <iostream>
#include <cmath>
#include "rendering.h"
#include "camera.h"
#include "screen.h"
#include "sprites.h"
#include "levelmap.h"
#include "event.h"
#include "logger.h"
#include "menus.h"
#include "input.h"
#include "character.h"
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
			painter.drawString(tile->pos, sprite.getCurrentFrame(), "Entity");
		}
		
		void on_event(Event& e) {
			SpylikeEvents::KeyInputEvent &ke = dynamic_cast<SpylikeEvents::KeyInputEvent&>(e);
			LOGGER.log(std::to_string(ke.c), DEBUG);
		}
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
		
		vector<RenderLayer> layers = {RenderLayer("Entity", 1), RenderLayer("UI", 2)};
		NcursesTerminalScreen screen(60, 20);

		std::shared_ptr<EventManager> manager(new EventManager());
		std::shared_ptr<InputManager> inputManager(new InputManager(manager, screen));
		
		std::shared_ptr<Camera> camera = std::make_shared<Camera>(screen, 60, 20, layers);
		GeometryRenderer renderer(*camera);
		manager->subscribe(camera, "CAMERA_MoveUp");
		manager->subscribe(camera, "CAMERA_MoveDown");
		manager->subscribe(camera, "CAMERA_MoveLeft");
		manager->subscribe(camera, "CAMERA_MoveRight");
		
		std::shared_ptr<epicEntity> ent = std::make_shared<epicEntity>(coolSprite);
		
		std::shared_ptr<MenuButton> button = std::make_shared<MenuButton>(20, 4, "mug moment", "Entity");
		std::shared_ptr<Menu> menu = std::make_shared<Menu>(60, 20);
		std::shared_ptr<Player> player = std::make_shared<Player>();
		manager->subscribe(player, "INPUT_KeyPress");
	
		menu->addChild(button);
		
		IDBlock idAllocation = {0, 1024};
		std::shared_ptr<LevelMap> map = std::make_shared<LevelMap>(60, 20, manager, idAllocation);
		map->registerEntity(ent, Coordinate(5,5));
		map->registerEntity(menu, Coordinate(1,1));
		map->registerEntity(player, Coordinate(10, 10));
		menu->addButton(button, Coordinate(0,0));
		map->moveEntity(menu, Coordinate(3,3));
		menu->click();
		
		bool flag = true;
		while (true) {
			//camera.setOrigin(Coordinate(camera.getOrigin().x + 1, camera.getOrigin().y + 1));
			inputManager->update();
			for (int y=0; y<20; y++) {
				for (int x=0; x<60; x++) {
					map->updateTile(Coordinate(x, y));
					map->drawTile(Coordinate(x, y), renderer);
				}
			}
			usleep(50000);
			camera->clearScreen();
		}

		screen.end();
	}
}
