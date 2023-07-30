#ifndef SPYLIKE_TESTING_H
#define SPYLIKE_TESTING_H

#include "levelmap.h"

#include <string>
#include <vector>
#include <memory>

enum TestResultType { success, failure };

class TestResult {
	public:
		TestResult(std::string info, TestResultType type) : info{info}, type{type} {}
		std::string info;
		TestResultType type;
};

class UnitTestSuite {
	public:
		UnitTestSuite(std::string name) : name{name} {}
		std::string name;
		virtual std::vector<TestResult> runTests() = 0;
};

class UnitTest {
	public:
		UnitTest(std::string name) : name{name} {}
		const std::string name;
		virtual int run() = 0;
};

class LevelMapTestSuite : public UnitTestSuite {
	protected:
		std::shared_ptr<LevelMap> map1;
		std::shared_ptr<LevelMap> map2;
		std::shared_ptr<LevelMap> map3;
		std::shared_ptr<LevelMap> map4;
	public:
		LevelMapTestSuite();
		std::vector<TestResult> runTests();
};


#endif