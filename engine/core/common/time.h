#pragma once

#include <cstdint>
#include <chrono>
#include <iostream>

namespace z {

using namespace std::chrono;

class TimePoint {
public:
	TimePoint(time_point<system_clock> && time) {
		time_ = time;
	}

	uint64_t TotalMs() {
		return duration_cast<milliseconds>(time_.time_since_epoch()).count();
	}

	int InSecond();

private:
	time_point<system_clock> time_;

};

class ZTime {
public:
	static TimePoint Now() {
		return TimePoint(system_clock::now());

	
	
	}
};

}
