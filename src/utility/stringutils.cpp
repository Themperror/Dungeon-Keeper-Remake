#include "stringUtils.h"

std::string Utility::ToLowerCase(const std::string& string)
{
	std::string newString = string;
	for (size_t i = 0; i < newString.size(); i++)
	{
		newString[i] = std::tolower(newString[i]);
	}
	return newString;
}

std::wstring Utility::ToWideString(const std::string& string)
{
	std::wstring newString(string.size()*2, '\0');
	size_t numCharsConverted = 0;
	mbstowcs_s(&numCharsConverted, newString.data(), newString.size(), string.data(), string.size());
	newString.resize(numCharsConverted-1);
	return newString;
}

bool Utility::Contains(const std::string& sourceString, const std::string& searchString, bool caseSensitive)
{
	const std::string& source = caseSensitive ? sourceString : ToLowerCase(sourceString);
	const std::string& search = caseSensitive ? searchString : ToLowerCase(searchString);
	
	size_t offset = source.find(search);
	return offset != std::string::npos;
}
bool Utility::Contains(const std::string_view& sourceString, const std::string_view& searchString, bool caseSensitive)
{
	const std::string_view& source = caseSensitive ? sourceString : ToLowerCase(std::string(sourceString.data(),sourceString.size()));
	const std::string_view& search = caseSensitive ? searchString : ToLowerCase(std::string(searchString.data(),searchString.size()));
	
	size_t offset = source.find(search);
	return offset != std::string::npos;
}


std::string Utility::ReplaceChar(std::string input, char target, char newChar)
{
	for (int i = 0; i < input.size(); i++)
	{
		if (input[i] == target)
		{
			input[i] = newChar;
		}
	}

	return input;
}