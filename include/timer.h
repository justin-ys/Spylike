#ifndef SPYLIKE_TIMER_H
#define SPYLIKE_TIMER_H

class Timer {
	int elapsed=0; // in ticks
	bool paused=false;
	public:
		void tick();
		void reset();
		void pause();
		void unpause();
		int getElapsed();
};

#endif
