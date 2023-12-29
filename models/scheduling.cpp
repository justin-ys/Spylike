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
	for (std::string taskID : deletionSlate) {
		int idx = -1;
		for (int i = 0; i < tasks.size(); i++) {
			auto& task = tasks[i];
			if (task->id == taskID) {
				idx = i;
			}
		}
		if (idx >= 0) {
			tasks.erase(tasks.begin() + idx);
		}
	}
	deletionSlate = {};
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
			if (!elapsedPaused) usElapsed += frameDelay;
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

void FrameScheduler::destroyTask(std::string taskID) {
	pauseTask(taskID);
	deletionSlate.push_back(taskID);
}

void FrameScheduler::resumeTask(std::string taskID) {
	for (auto& task : tasks) {
		if (task->id == taskID) {
			task->running = true;
		}
	}
}

bool FrameScheduler::isRunning(std::string taskID) {
	for (auto& task : tasks) {
		if (task->id == taskID) {
			return task->running;
		}
	}
	return false;
}

int FrameScheduler::timeElapsed() {
	return usElapsed/1000000;
}
