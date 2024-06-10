#pragma once
#ifndef TIMER_H
#define TIMER_H
#include "Header.h"

struct Timer {
	double time = 0;
	chrono::time_point<chrono::high_resolution_clock> beg;
	void Reset();
	void SetBegTime();
	void TimeElipsed();
};
#endif