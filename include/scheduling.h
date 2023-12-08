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
	std::vector<std::shared_ptr<ScheduledTask>> tasks;
	bool runningSignal;
	public:
		FrameScheduler(std::vector<std::shared_ptr<ScheduledTask>> tasks, int maxFPS);
		void run();
		void pause();
		void resume();
		// no more IDs
		void pauseTask(std::string taskID);
		// should be starttask
		void resumeTask(std::string taskID);
};
		
#endif
