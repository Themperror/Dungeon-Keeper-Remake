#include "timer.h"
using namespace Utility;

void Timer::StartTime()
{
	oldT = std::chrono::high_resolution_clock::now();
	newT = std::chrono::high_resolution_clock::now();
}
//microseconds: Retrieves the delta time since StartTime or Init was called.
int64_t Timer::GetDeltaTimeMicro()
{
	auto t = std::chrono::high_resolution_clock::now() - oldT;
	return std::chrono::duration_cast<std::chrono::microseconds>(t).count();
}
//milliseconds: Retrieves the delta time since StartTime or Init was called.
int64_t Timer::GetDeltaTimeMS()
{
	auto t = std::chrono::high_resolution_clock::now() - oldT;
	return std::chrono::duration_cast<std::chrono::milliseconds>(t).count();
}
//microseconds: Resets the timer to the current time, Repeatedly calling this the same frame will yield low to 0 values
int64_t Timer::GetDeltaTimeMicroReset()
{
	newT = std::chrono::high_resolution_clock::now();
	auto t = newT - oldT;
	oldT = newT;
	long long delta = std::chrono::duration_cast<std::chrono::microseconds>(t).count();
	return delta;
}
//microseconds: Resets the timer to the current time, Repeatedly calling this the same frame will yield low to 0 values
int64_t Timer::GetDeltaTimeNanoReset()
{
	newT = std::chrono::high_resolution_clock::now();
	auto t = newT - oldT;
	oldT = newT;
	long long delta = std::chrono::duration_cast<std::chrono::nanoseconds>(t).count();
	return delta;
}
//milliseconds: Resets the timer to the current time, Repeatedly calling this the same frame will yield low to 0 values
int64_t Timer::GetDeltaTimeMSReset()
{
	newT = std::chrono::high_resolution_clock::now();
	auto t = newT - oldT;
	oldT = newT;
	long long delta = std::chrono::duration_cast<std::chrono::milliseconds>(t).count();
	return delta;
}
//Seconds (nanosecond precision): Resets the timer to the current time, Repeatedly calling this the same frame will yield low to 0 values
double Timer::GetDeltaTimeReset()
{
	return static_cast<double>(GetDeltaTimeNanoReset()) / 1000000000.0;
}
//Seconds (Microsecond precision): Retrieves the delta time since StartTime or Init was called.
double Timer::GetDeltaTime()
{
	return static_cast<double>(GetDeltaTimeMicro()) / 1000000.0;
}