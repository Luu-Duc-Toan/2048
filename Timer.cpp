#include "Timer.h"

void Timer::Reset() {
	time = 0;
	SetBegTime();
}
void Timer::SetBegTime() {
	beg = chrono::high_resolution_clock::now();
}
void Timer::TimeElipsed() {
	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration<double>(end - beg);
	SetBegTime();
	time += duration.count();
}