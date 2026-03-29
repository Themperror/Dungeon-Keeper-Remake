#pragma once
#include <chrono>

namespace Utility
{
	class Timer
	{
	public:
		void StartTime();
		//microseconds: Retrieves the delta time since StartTime or Init was called.
		int64_t GetDeltaTimeMicro();
		//milliseconds: Retrieves the delta time since StartTime or Init was called.
		int64_t GetDeltaTimeMS();
		//microseconds: Resets the timer to the current time, Repeatedly calling this the same frame will yield low to 0 values
		int64_t GetDeltaTimeMicroReset();
		//microseconds: Resets the timer to the current time, Repeatedly calling this the same frame will yield low to 0 values
		int64_t GetDeltaTimeNanoReset();
		//milliseconds: Resets the timer to the current time, Repeatedly calling this the same frame will yield low to 0 values
		int64_t GetDeltaTimeMSReset();
		//Seconds (nanosecond precision): Resets the timer to the current time, Repeatedly calling this the same frame will yield low to 0 values
		double GetDeltaTimeReset();
		//Seconds (Microsecond precision): Retrieves the delta time since StartTime or Init was called.
		double GetDeltaTime();
	private:
		std::chrono::time_point<std::chrono::steady_clock> oldT, newT;
	};
}