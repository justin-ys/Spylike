#include "scheduling.h"
#include "logger.h"
#include <string>
#include <vector>
#include <unistd.h>
#include <cassert>
#include <memory>

extern SpylikeLogger LOGGER;

ScheduledTask::ScheduledTask(std::string id) : id(id), running(true) {}

void FrameScheduler::run() {
	int frameDelay = 1000000/maxFPS;
	if (!runningSignal) { 
		runningSignal = true;
		while (runningSignal) {
			for (auto& task : tasks) { 
					if (task->running) {
						task->update();
					}
			}
			usleep(frameDelay);
		}
	}
	else {
		LOGGER.log("Scheduler already running, can't run in parallel", ERROR);
	}
}

void FrameScheduler::pause() {
	for (auto& task : tasks) { 
		task->running = false;
	}
}

void FrameScheduler::resume() { 
	for (auto& task : tasks) { 
		task->running = true;
	}
}

void FrameScheduler::pauseTask(std::string taskID) {
	for (auto& task : tasks) {
		if (task->id == taskID) {
			task->running = false;
		}
	}
}

void FrameScheduler::resumeTask(std::string taskID) {
	for (auto& task : tasks) {
		if (task->id == taskID) {
			task->running = true;
		}
	}
}
