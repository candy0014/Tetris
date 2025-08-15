#ifndef TIMER_H
#define TIMER_H

#include <chrono>

#ifdef _WIN32
#include <windows.h>
#endif

struct Timer {
	std::chrono::time_point<std::chrono::steady_clock> start;

	Timer(): start(std::chrono::steady_clock::now()) {}
	~Timer() = default;

	double get() const {
		auto end = std::chrono::steady_clock::now();	
		auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
		return static_cast<double>(duration.count()) *
			std::chrono::microseconds::period::num / std::chrono::microseconds::period::den; 
	}
	void sleep(double t){
		#ifdef _WIN32
		Sleep((int)(t*1000));
		#endif
		#ifdef __linux__
		usleep((int)(t*1000000));
		#endif
	}
};

static Timer timer;

#endif