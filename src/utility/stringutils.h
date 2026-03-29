#pragma once

#include "print.h"
#include "break.h"
#include <charconv>
#include <string>
namespace Utility
{
	std::string ToLowerCase(const std::string& string);
	std::wstring ToWideString(const std::string& string);

	std::string ReplaceChar(std::string input, char target, char newChar);
	bool Contains(const std::string& source, const std::string& search, bool caseSensitive = true);
	bool Contains(const std::string_view& sourceString, const std::string_view& searchString, bool caseSensitive);

	template<typename T, typename S, 
		std::enable_if_t<
		std::is_base_of_v<std::basic_string_view<char>, S> ||
		std::is_base_of_v<std::basic_string<char>, S> ||
		std::is_base_of_v<std::basic_string_view<wchar_t>, S> || 
		std::is_base_of_v<std::basic_string<wchar_t>, S>
		>* = nullptr>
	T FromString(const S& s)
	{
		T result{};
		auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), result);
		if (ec == std::errc())
		{
			return result;
		}

		if constexpr (std::is_same_v<S::value_type ,char>)
		{
			Print("Failed to convert %*s", s.size(), s.data());
		}
		else if constexpr (std::is_same_v<S::value_type, wchar_t>)
		{
			Print("Failed to convert %*S", s.size(), s.data());
		}

		Utility::Break();
		return result;
	}

}