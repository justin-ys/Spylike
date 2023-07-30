#include "test.h"
#include "levelmap.h"
#include "event.h"
#include "rendering.h"
#include <string>
#include <memory>
#include <vector>

class basicEntity : public TileEntity {
	public:
		void on_update() { }
		void draw(GeometryRenderer& painter) {}
		void on_event(Event& e) {}
};


class LevelMapTest : public UnitTest {
	public:
		std::shared_ptr<LevelMap> map;
		LevelMapTest(std::shared_ptr<LevelMap> map, std::string name) : map{map}, UnitTest{name} {}
		virtual int run() = 0;
};

class TestLevelMapPutEntity : public LevelMapTest {
	public:
		TestLevelMapPutEntity(std::shared_ptr<LevelMap> map) : LevelMapTest(map, "Place Entity") {}
		int run() {
			std::vector<Coordinate> coordinates = {	Coordinate(0, 0), Coordinate(map->width-1, map->height-1),
													Coordinate(map->width-1, 0), Coordinate(0, map->height-1),
													Coordinate(map->width/2, map->height/2) };
			std::vector<std::shared_ptr<basicEntity>> entities;
			for (int i=0; i<coordinates.size(); i++) {
				entities.push_back(std::make_shared<basicEntity>());
				map->putEntity(entities[i], coordinates[i]);
			}
			for (int i=0; i<coordinates.size(); i++) {
				if (entities[i]->tile->pos != coordinates[i]) {
					return 0;
				}
			}
			return 1;
		}
};

LevelMapTestSuite::LevelMapTestSuite() : UnitTestSuite("LevelMap tests") {
	std::shared_ptr<EventManager> manager(new EventManager());
	IDBlock idAllocation = {0, 1024};
	map1 = std::shared_ptr<LevelMap>(new LevelMap(100, 100, manager, idAllocation));
	map2 = std::shared_ptr<LevelMap>(new LevelMap(40, 60, manager, idAllocation));
	map3 = std::shared_ptr<LevelMap>(new LevelMap(60, 40, manager, idAllocation));
	map4 = std::shared_ptr<LevelMap>(new LevelMap(1000, 1000, manager, idAllocation));
}

std::string getMapInfo(std::shared_ptr<LevelMap> map) {
	return "Map width " + std::to_string(map->width) + " Map height: " + std::to_string(map->height);
}

std::vector<TestResult> LevelMapTestSuite::runTests() {
	std::vector<TestResult> results;
	std::vector<std::shared_ptr<LevelMap>> maps = {map1, map2, map3, map4};
	std::vector<std::shared_ptr<LevelMapTest>> tests;
	for (auto map : maps) {
		tests.push_back(std::make_shared<TestLevelMapPutEntity>(map));
	}
	for (auto test : tests) {
		int result = test->run();
		std::string resultString = test->name + ": " + getMapInfo(test->map);
		if (result) results.push_back(TestResult(resultString, TestResultType::success));
		else results.push_back(TestResult(resultString, TestResultType::failure));
	}
			
	return results;
}