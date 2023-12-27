#include "Timer.h"

std::shared_ptr<Timer> Timer::GetInstance()
{
    static std::shared_ptr<Timer> statisticCollector{ new Timer };
    return statisticCollector;
}

void Timer::ResetTime()
{
    startTime = std::chrono::high_resolution_clock::now();
}

double Timer::ElapsedTime() const
{
    return std::chrono::duration_cast<second_t>(std::chrono::high_resolution_clock::now() - startTime).count();
}
