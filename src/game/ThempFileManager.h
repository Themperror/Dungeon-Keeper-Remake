#pragma once
#include <vector>
namespace Themp
{
	class FileManager
	{
	public:
		struct FileData
		{
			BYTE* data;
			size_t size;
		};

		~FileManager();
		FileManager();
		static FileManager* fileManager;
		static FileData GetFileData(std::wstring path);
	private:
		void LoadFilesFromDirectory(std::wstring dir);
		FileData LoadFileData(std::wstring& path);
	};
};
