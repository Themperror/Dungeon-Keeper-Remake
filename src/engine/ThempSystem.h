#pragma once

#include <Windows.h>
#include <map>
#include <vector>
#include <time.h>
#include <iostream>
#include <chrono>

#pragma warning( disable : 4996) //disables warning unsafe function: freopen() fopen() .. etc

#define ReportLiveObjects 0
#define LOG_MISSING_MATERIALS 0
#define CLEAN(x) if(x){x->Release();x=nullptr;}

namespace Themp
{
	struct Timer
	{
	public:
		Timer()
		{
			StartTime();
		}
		void StartTime()
		{
			oldT = std::chrono::high_resolution_clock::now();
			newT = std::chrono::high_resolution_clock::now();
		}
		//microseconds: Retrieves the delta time since StartTime or Init was called.
		long long GetDeltaTimeMicro()
		{
			auto t = std::chrono::high_resolution_clock::now() - oldT;
			return std::chrono::duration_cast<std::chrono::microseconds>(t).count();
		}
		//milliseconds: Retrieves the delta time since StartTime or Init was called.
		long long GetDeltaTimeMS()
		{
			auto t = std::chrono::high_resolution_clock::now() - oldT;
			return std::chrono::duration_cast<std::chrono::milliseconds>(t).count();
		}
		//microseconds: Resets the timer to the current time, Repeatedly calling this the same frame will yield low to 0 values
		long long GetDeltaTimeMicroReset()
		{
			newT = std::chrono::high_resolution_clock::now();
			auto t = newT - oldT;
			oldT = newT;
			long long delta = std::chrono::duration_cast<std::chrono::microseconds>(t).count();
			return delta;
		}
		//microseconds: Resets the timer to the current time, Repeatedly calling this the same frame will yield low to 0 values
		long long GetDeltaTimeNanoReset()
		{
			newT = std::chrono::high_resolution_clock::now();
			auto t = newT - oldT;
			oldT = newT;
			long long delta = std::chrono::duration_cast<std::chrono::nanoseconds>(t).count();
			return delta;
		}
		//milliseconds: Resets the timer to the current time, Repeatedly calling this the same frame will yield low to 0 values
		long long GetDeltaTimeMSReset()
		{
			newT = std::chrono::high_resolution_clock::now();
			auto t = newT - oldT;
			oldT = newT;
			long long delta = std::chrono::duration_cast<std::chrono::milliseconds>(t).count();
			return delta;
		}
		//Seconds (nanosecond precision): Resets the timer to the current time, Repeatedly calling this the same frame will yield low to 0 values
		double GetDeltaTimeReset()
		{
			return static_cast<double>(GetDeltaTimeNanoReset()) / 1000000000.0;
		}
		//Seconds (Microsecond precision): Retrieves the delta time since StartTime or Init was called.
		double GetDeltaTime()
		{
			return static_cast<double>(GetDeltaTimeMicro()) / 1000000.0;
		}
	private:

		std::chrono::time_point<std::chrono::steady_clock> oldT,newT;
	};
	float lerp(float x, float y, float t);
	class Game;
	class D3D;
	class Resources;
	class GUI;
	class Audio;
	class System
	{
	public:
		static FILE* logFile;
		static void Print(const char* message, ...);

		static Themp::System* tSys;
		System() {}; 
		void Start();
		void Interrupt() {}; // Alt tab, lost focus etc...

		std::string m_BaseDir;
		HWND m_Window = nullptr;
		HINSTANCE m_HInstance = 0;
		bool m_Quitting = false;
		bool m_CursorShown = true;
		std::map<std::string, float> m_SVars;
		Themp::Game* m_Game;
		Themp::D3D* m_D3D;
		Themp::Resources* m_Resources;
		Themp::GUI* m_GUI;
		Themp::Audio* m_Audio;
	};
};