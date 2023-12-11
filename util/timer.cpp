#include "timer.h"

void Timer::tick() { if (!paused) elapsed++; }
void Timer::reset() { elapsed=0; }
void Timer::pause() { paused=true; }
void Timer::unpause() { paused=false; }
int Timer::getElapsed() { return elapsed; }
