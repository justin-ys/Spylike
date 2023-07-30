#include "test.h"
#include <vector>
#include <string>
#include <iostream>

#include "logger.h"
extern SpylikeLogger LOGGER("test_log.txt", DEBUG);

int main() {
	std::vector<std::shared_ptr<UnitTestSuite>> suites;
	suites.push_back(std::make_shared<LevelMapTestSuite>());
	for (auto suite : suites) {
		std::cout << "RUNNING SUITE: " + suite->name << "\n";
		std::vector<TestResult> results = suite->runTests();
		for (TestResult result : results) {
			if (result.type == TestResultType::success) std::cout << "TEST PASSED " << result.info << "\n";
			if (result.type == TestResultType::failure) std::cout << "TEST FAILED " << result.info << "\n";
		}
		std::cout << "\n";
	}
}
	