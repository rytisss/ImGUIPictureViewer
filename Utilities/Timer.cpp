#include "Timer.h"

Timer::Timer() :
	m_startPoint(-1)
	, m_endPoint(-1)
{
}

int64_t Timer::GetTimeStamp()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void Timer::Start()
{
	m_startPoint = Timer::GetTimeStamp();
}

void Timer::Stop()
{
	m_endPoint = Timer::GetTimeStamp();
}

void Timer::Reset()
{
	m_startPoint = -1;
	m_endPoint = -1;
}

int64_t Timer::GetCurrentDurationInMiliseconds()
{
	int64_t currentTimeStamp = GetTimeStamp();
	return currentTimeStamp - m_startPoint;
}

int64_t Timer::GetDurationInMiliseconds()
{
	return m_endPoint - m_startPoint;
}

Timer::~Timer()
{
}
