#include "fileutils.h"
#include "stringutils.h"
#include "print.h"
#include "break.h"

#include <string>
#include <vector>
#include <algorithm>

#include "d3dincl.h"


namespace Utility
{
	std::string GetPathName(std::string s)
	{
		std::string name = "";
		int64_t size = s.size() - 1;
		for (int64_t i = size; i >= 0; i--)
		{
			if (s.at(i) == '\\' || s[i] == '/')
			{
				i++;
				name = s.substr(0, i);
				break;
			}
		}
		return name;
	}

	std::string GetFileExtension(const std::string& s)
	{
		size_t lastPeriod = s.find_last_of('.');
		return s.substr(lastPeriod, s.size() - lastPeriod);
	}

	std::vector<std::string> GetFilesInDirectoryRecursive(const std::string& dir)
	{
		std::string pathAdjusted = Utility::ReplaceChar(dir, '/', '\\');;
		if (!pathAdjusted.ends_with('\\'))
			pathAdjusted.append("\\");

		pathAdjusted.append("*");

		WIN32_FIND_DATAA findFileData;
		std::vector<std::string> files;
		std::vector<std::string> dirs;

		const auto GetDirectoryData = [&](const std::string& path)
		{
			std::string dirWithoutAsterisk = path;
			dirWithoutAsterisk.erase(dirWithoutAsterisk.begin() + dirWithoutAsterisk.size() - 1);
			HANDLE hFind = FindFirstFileA(path.c_str(), &findFileData);
			while (hFind != INVALID_HANDLE_VALUE)
			{
				if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					dirs.push_back(dirWithoutAsterisk + findFileData.cFileName + "\\*");
				}
				else
				{
					files.push_back(dirWithoutAsterisk + findFileData.cFileName);
				}
				if (!FindNextFileA(hFind, &findFileData))
					break;
			}
			if(hFind != INVALID_HANDLE_VALUE)
				FindClose(hFind);

			std::erase_if(dirs, [](const std::string& x) { return x.ends_with(".\\*"); });
		};

		GetDirectoryData(pathAdjusted);
		for (size_t i = 0; i < dirs.size(); i++)
		{
			GetDirectoryData(dirs[i]);
		}

		return files;
	}
	std::vector<std::string> GetFilesInDirectory(const std::string& dir)
	{
		std::vector<std::string> files;

		DWORD attributes = GetFileAttributesA(dir.c_str());
		if (attributes == INVALID_FILE_ATTRIBUTES)
		{
			Print(L"Unable to find folder at %s", dir.c_str());
			Utility::Break();
			return {};
		}

		WIN32_FIND_DATAA ffd{};
		HANDLE hFind = FindFirstFileA((dir + "*").c_str(), &ffd);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			Print("Something went wrong %s", dir.c_str());
		}
		while (FindNextFileA(hFind, &ffd) != 0)
		{
			// ignore directories
			if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				// create a full path for each file we find, e.g. "c:\indir\foo.txt"
				std::string file_path;
				file_path.reserve(2048);
				file_path = dir + ffd.cFileName;
				std::transform(file_path.begin(), file_path.end(), file_path.begin(), ::tolower);
				files.push_back(file_path);
			}
		}
		FindClose(hFind);
		return files;
	}

	bool FileExists(const std::string& filePath)
	{
		HANDLE file = CreateFileA(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (file == INVALID_HANDLE_VALUE)
		{
			return false;
		}
		CloseHandle(file);
		return true;
	}

	std::string ReadFileToString(const std::string& filePath)
	{
		HANDLE file = CreateFileA(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (file == INVALID_HANDLE_VALUE)
		{
			Print("Was unable to open the file: [%s]", filePath.c_str());
			Utility::Break();
			return "";
		}

		DWORD fileSize = GetFileSize(file, NULL);
		std::string data(fileSize, '\0');
		DWORD readBytes = 0;
		if (!ReadFile(file, data.data(), fileSize, &readBytes, NULL))
		{
			Print("Was unable to read the file: [%s]", filePath.c_str());
			Utility::Break();
		}
		if (readBytes != fileSize)
		{
			Print("Was unable to read the entire file: [%s]", filePath.c_str());
			Utility::Break();
		}
		CloseHandle(file);

		return data;
	}
	std::vector<uint8_t> ReadFileToVector(const std::string& filePath)
	{
		HANDLE file = CreateFileA(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (file == INVALID_HANDLE_VALUE)
		{
			Print("Was unable to open the file: [%s]", filePath.c_str());
			Utility::Break();
			return {};
		}

		DWORD fileSize = GetFileSize(file, NULL);
		std::vector<uint8_t> data(fileSize, 0);
		DWORD readBytes = 0;
		if (!ReadFile(file, data.data(), fileSize, &readBytes, NULL))
		{
			Print("Was unable to read the file: [%s]", filePath.c_str());
			Utility::Break();
		}
		if (readBytes != fileSize)
		{
			Print("Was unable to read the entire file: [%s]", filePath.c_str());
			Utility::Break();
		}
		CloseHandle(file);

		return data;
	}


	bool WriteFile(const void* data, size_t size, const std::string& filePath)
	{
		EnsureFileTreeExists(filePath);

		HANDLE file = CreateFileA(filePath.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (file == INVALID_HANDLE_VALUE)
		{
			Print("Was unable to open the file: [%s]", filePath.c_str());
			Utility::Break();
			return false;
		}
		DWORD bytesWritten = 0;
		if (size > std::numeric_limits<DWORD>::max())
		{
			Print("trying to write more than %llu amount of bytes [%llu], this isn't supported");
			Utility::Break();
		}
		if (::WriteFile(file, data, static_cast<DWORD>(size), &bytesWritten, NULL) == false)
		{
			Print("Was unable to write the file: [%s]", filePath.c_str());
			Utility::Break();
			CloseHandle(file);
			return false;
		}
		if (bytesWritten != size)
		{
			Print("Was unable to write the entire file: [%s]", filePath.c_str());
			Utility::Break();
			CloseHandle(file);
			return false;
		}
		CloseHandle(file);

		return true;
	}

	bool WriteFile(const std::vector<uint8_t>& data, const std::string& filePath)
	{
		return WriteFile(data.data(), data.size(), filePath);
	}

	void EnsureFileTreeExists(const std::string& filePath)
	{
		size_t slashPos = filePath.find('\\', 0);
		while (slashPos != std::string::npos)
		{
			if (slashPos + 1 < filePath.size())
			{
				size_t nextPos = filePath.find('\\', slashPos+1);

				std::string pathSub = std::string(filePath.begin(), filePath.begin() + slashPos);
				CreateDirectoryA(pathSub.c_str(), nullptr);

				slashPos = nextPos;
			}
			else
			{
				slashPos = std::string::npos;
			}
		}
	}
	void EnsureFileTreeExists(const std::wstring& filePath)
	{
		size_t slashPos = filePath.find(L'\\', 0);
		while (slashPos != std::wstring::npos)
		{
			if (slashPos + 1 < filePath.size())
			{
				size_t nextPos = filePath.find(L'\\', slashPos+1);

				std::wstring pathSub = std::wstring(filePath.begin(), filePath.begin() + slashPos);
				CreateDirectoryW(pathSub.c_str(), nullptr);

				slashPos = nextPos;
			}
			else
			{
				slashPos = std::wstring::npos;
			}
		}
	}


	std::string GetFileName(std::string_view path)
	{
		std::string filename = std::string(path.begin(), path.end());
		for (size_t i = 0; i < filename.size(); i++)
		{
			if (filename[i] == '/')
			{
				filename[i] = '\\';
			}
		}
		size_t lastBackSlash = filename.find_last_of('\\');
		if (lastBackSlash != std::string::npos && lastBackSlash + 1 < filename.size())
		{
			filename = filename.substr(lastBackSlash + 1, filename.size() - lastBackSlash - 1);
		}

		size_t lastPeriod = filename.find_last_of('.');
		if (lastPeriod != std::string::npos)
		{
			filename = filename.substr(0, lastPeriod);
		}

		return filename;
	}


	std::string ReplaceExtensionWith(std::string str, const std::string& extension)
	{
		int offsetLeadingPeriods = 0;
		for (int i = 0; i < str.size(); i++)
		{
			if (str[i] != '.')
			{
				offsetLeadingPeriods = i;
				break;
			}
		}
		size_t period = str.find_last_of('.');
		if (period != std::string::npos && period > offsetLeadingPeriods)
		{
			str = str.substr(0, period);
		}
		str.append(extension);
		return str;
	}

	std::string RemoveFileFromPathName(std::string str)
	{
		for (size_t i = 0; i < str.size(); i++)
		{
			if (str[i] == '/')
			{
				str[i] = '\\';
			}
		}
		size_t lastBackSlash = str.find_last_of('\\');
		if (lastBackSlash != std::string::npos)
		{
			str = str.substr(0, lastBackSlash + 1);
		}
		return str;
	}
}