#ifndef SPYLIKE_SCHEDULING_H
#define SPYLIKE_SCHEDULING_H

#include <string>
#include <vector>
#include <memory>

class ScheduledTask {
	public:
		ScheduledTask(std::string id);
		// should be getter/setter
		std::string id;
		// ditto
		bool running;
		virtual void update() = 0;
};

class FrameScheduler {
	int maxFPS;
	std::vector<std::unique_ptr<ScheduledTask>> tasks;
	bool runningSignal;
	public:
		FrameScheduler(int maxFPS=60) : maxFPS{maxFPS} {
			runningSignal = false;
		}
		void addTask(std::unique_ptr<ScheduledTask> t) {
			tasks.push_back(std::move(t));
		}
		void run();
		void pause();
		void resume();
		// no more IDs
		void pauseTask(std::string taskID);
		// should be starttask
		void resumeTask(std::string taskID);
		bool isRunning(std::string taskID);
};
		
#endif
