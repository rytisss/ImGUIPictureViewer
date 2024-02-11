#ifndef TIMER_H
#define TIMER_H

#include <string>
#include <chrono>

class Timer
{
public:
	//
	Timer();
	// Get current time stamp
	static int64_t GetTimeStamp();
	// Start times
	void Start();
	// Stop timer
	void Stop();
	// Reset timer
	void Reset();
	// Get current duration (from start till the moment when this function is called)
	int64_t GetCurrentDurationInMiliseconds();
	// Return duration in miliseconds from Start to Stop
	int64_t GetDurationInMiliseconds();
	//
	~Timer();
private:
	int64_t m_startPoint;
	int64_t m_endPoint;
};

#endif //TIMER_H