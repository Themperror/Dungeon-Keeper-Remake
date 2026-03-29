#include "print.h"

#include <iostream>

#include <time.h>
#include <sys/timeb.h>
#include <cstdarg>
static FILE* logFile = stdout;

#pragma warning( disable : 4996) //disables warning unsafe function: freopen() fopen() .. etc

void Utility::SetLogFile(const char* filePath)
{
	if (logFile != stdout)
	{
		fclose(logFile);
	}
	if (filePath != nullptr)
	{
		logFile = fopen("log.txt", "w+");
	}
}

void Print(const char* message, ...)
{
	timeb t;
	ftime(&t);
	auto time = localtime(&t.time);
	//print time in format: [18:26:02:270]
	fprintf(logFile, "[%02i:%02i:%02i:%03i]: ", time->tm_hour, time->tm_min, time->tm_sec, t.millitm);
	if (logFile != stdout)printf("[%02i:%02i:%02i:%03i]: ", time->tm_hour, time->tm_min, time->tm_sec, t.millitm);
	va_list args;
	va_start(args, message);
	vfprintf(logFile, message, args);
	if (logFile != stdout) vprintf(message, args);
	va_end(args);
	fprintf(logFile, "\n");
	if (logFile != stdout)printf("\n");
	fflush(logFile);
}

void Print(const wchar_t* message, ...)
{
	timeb t;
	ftime(&t);
	auto time = localtime(&t.time);
	//print time in format: [18:26:02:270]
	fwprintf(logFile, L"[%02i:%02i:%02i:%03i]: ", time->tm_hour, time->tm_min, time->tm_sec, t.millitm);
	if (logFile != stdout)wprintf(L"[%02i:%02i:%02i:%03i]: ", time->tm_hour, time->tm_min, time->tm_sec, t.millitm);
	va_list args;
	va_start(args, message);
	vfwprintf(logFile, message, args);
	if (logFile != stdout) vwprintf(message, args);
	va_end(args);
	fwprintf(logFile, L"\n");
	if (logFile != stdout)wprintf(L"\n");
	fflush(logFile);
}

void Print(const std::string message, ...)
{
	timeb t;
	ftime(&t);
	auto time = localtime(&t.time);
	//print time in format: [18:26:02:270]
	fprintf(logFile, "[%02i:%02i:%02i:%03i]: ", time->tm_hour, time->tm_min, time->tm_sec, t.millitm);
	if (logFile != stdout)printf("[%02i:%02i:%02i:%03i]: ", time->tm_hour, time->tm_min, time->tm_sec, t.millitm);
	va_list args;
	va_start(args, message);
	vfprintf(logFile, message.c_str(), args);
	if (logFile != stdout) vprintf(message.c_str(), args);
	va_end(args);
	fprintf(logFile, "\n");
	if (logFile != stdout)printf("\n");
	fflush(logFile);
}


void Print(const std::wstring message, ...)
{
	timeb t;
	ftime(&t);
	auto time = localtime(&t.time);
	//print time in format: [18:26:02:270]
	fprintf(logFile, "[%02i:%02i:%02i:%03i]: ", time->tm_hour, time->tm_min, time->tm_sec, t.millitm);
	if (logFile != stdout)printf("[%02i:%02i:%02i:%03i]: ", time->tm_hour, time->tm_min, time->tm_sec, t.millitm);
	va_list args;
	va_start(args, message);
	vfwprintf(logFile, message.c_str(), args);
	if (logFile != stdout) vwprintf(message.c_str(), args);
	va_end(args);
	fwprintf(logFile, L"\n");
	if (logFile != stdout)printf("\n");
	fflush(logFile);
}