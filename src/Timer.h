#pragma once
#include <memory>
#include <chrono>

class Timer
{
public:
	using timePoint = std::chrono::high_resolution_clock::time_point;
	using second_t = std::chrono::duration<double, std::ratio<1>>;

	static std::shared_ptr<Timer> GetInstance();
	
	void ResetTime();
	double ElapsedTime() const;

	Timer(Timer const&) = delete;
	Timer& operator=(Timer const&) = delete;

private:
	Timer() = default;
	timePoint startTime;
	int rejectedTasksCount = 0;
};