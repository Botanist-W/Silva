#include "timer.h"



baseTimer::baseTimer() {};
nullTimer::nullTimer() {};

void nullTimer::logger(int step) {//nothing
};

activeTimer::activeTimer() {};

void activeTimer::logger(int step) {
	stepper++;
	if (stepper == 10000) {
		LOG_INFO("Time Step (rep 1): {}", step);
		stepper = 0;
	}
};

