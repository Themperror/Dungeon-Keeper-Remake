#pragma once
#include <string>
namespace Utility
{
	void SetLogFile(const char* filePath);
}

void Print(const char* message, ...);
void Print(const wchar_t* message, ...);
void Print(const std::string message, ...);
void Print(const std::wstring message, ...);