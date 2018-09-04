#include "ThempSystem.h"
#include "ThempFileManager.h"
#include "ThempGame.h"
#include "ThempResources.h"
#include "../Library/imgui.h"
#include "../Engine/ThempCamera.h"
#include "../Engine/ThempObject3D.h"
#include "../Engine/ThempMesh.h"
#include "../Engine/ThempMaterial.h"
#include "../Engine/ThempD3D.h"
#include "../Engine/ThempFunctions.h"
#include "../Engine/ThempDebugDraw.h"
#include "../Engine/ThempVideo.h"
#include <DirectXMath.h>
#include <lbrncbase.h>

using namespace Themp;
FileManager* FileManager::fileManager = nullptr;

std::unordered_map<std::wstring, FileManager::FileData> FileDataList =
{
	//{ std::string("DATA/ALPHA.COL"),{ nullptr } },
	//{ std::string("DATA/ALPHA.DAT"),{ nullptr } },
	//{ std::string("DATA/ALPHA.TAB"),{ nullptr } },
	//{ std::string("DATA/BLOCK.TAB"),{ nullptr } },
	//{ std::string("DATA/BLUEPAL.DAT"),{ nullptr } },
	//{ std::string("DATA/BLUEPALL.DAT"),{ nullptr } },
	//{ std::string("DATA/BORD0-0.DAT"),{ nullptr } },
	//{ std::string("DATA/BORD0-0.TAB"),{ nullptr } },
	//{ std::string("DATA/BORD0-1.DAT"),{ nullptr } },
	//{ std::string("DATA/BORD0-1.TAB"),{ nullptr } },
	//{ std::string("DATA/CEILING.TXT"),{ nullptr } },
	//{ std::string("DATA/COLOURS.COL"),{ nullptr } },
	//{ std::string("DATA/CREAT1.TAB"),{ nullptr } },
	//{ std::string("DATA/CREATURE.JTY"),{ nullptr } },
	//{ std::string("DATA/CREATURE.TAB"),{ nullptr } },
	//{ std::string("DATA/CREATURE.TXT"),{ nullptr } },
	//{ std::string("DATA/CRET0000.DAT"),{ nullptr } },
	//{ std::string("DATA/CUBE.DAT"),{ nullptr } },
	//{ std::string("DATA/CUBEGRP.DAT"),{ nullptr } },
	//{ std::string("DATA/DD1CREAT.TXT"),{ nullptr } },
	//{ std::string("DATA/DD1TEXT.DAT"),{ nullptr } },
	//{ std::string("DATA/DOGPAL.PAL"),{ nullptr } },
	//{ std::string("DATA/DP_PREFS"),{ nullptr } },
	//{ std::string("DATA/EDITICN1.DAT"),{ nullptr } },
	//{ std::string("DATA/EDITICN1.TAB"),{ nullptr } },
	//{ std::string("DATA/EDITICN2.DAT"),{ nullptr } },
	//{ std::string("DATA/EDITICN2.TAB"),{ nullptr } },
	//{ std::string("DATA/FONT0-0.DAT"),{ nullptr } },
	//{ std::string("ldata/FONT0-0.TAB"),{ nullptr } },
	//{ std::string("ldata/TORTURE"),{ nullptr } },
	//{ std::string("ldata/DKMAP00"),{ nullptr } },
	//{ std::string("ldata/DKMAP01"),{ nullptr } },
	//{ std::string("ldata/DKMAP02"),{ nullptr } },
	//{ std::string("ldata/DKMAP03"),{ nullptr } },
	//{ std::string("ldata/DKMAP04"),{ nullptr } },
	//{ std::string("ldata/DKMAP05"),{ nullptr } },
	//{ std::string("ldata/DKMAP06"),{ nullptr } },
	//{ std::string("ldata/DKMAP07"),{ nullptr } },
	//{ std::string("ldata/DKMAP08"),{ nullptr } },
	//{ std::string("ldata/DKMAP09"),{ nullptr } },
	//{ std::string("ldata/DKMAP10"),{ nullptr } },
	//{ std::string("ldata/DKMAP11"),{ nullptr } },
	//{ std::string("ldata/DKMAP12"),{ nullptr } },
	//{ std::string("ldata/DKMAP13"),{ nullptr } },
	//{ std::string("ldata/DKMAP14"),{ nullptr } },
	//{ std::string("ldata/DKMAP15"),{ nullptr } },
	//{ std::string("ldata/DKMAP16"),{ nullptr } },
	//{ std::string("ldata/DKMAP17"),{ nullptr } },
	//{ std::string("ldata/DKMAP18"),{ nullptr } },
	//{ std::string("ldata/DKMAP19"),{ nullptr } },
	//{ std::string("ldata/DKMAP20"),{ nullptr } },
	//{ std::string("ldata/DKMAP21"),{ nullptr } },
};

FileManager::~FileManager()
{
	for (auto i : FileDataList)
	{
		free(i.second.data);
	}
}

FileManager::FileManager()
{
	bool everythingOk = false;
	DWORD dataFType = GetFileAttributesA("data");
	if (dataFType == INVALID_FILE_ATTRIBUTES)
	{
		System::Print("\"\\data\\\" folder not found!");
		MessageBox(Themp::System::tSys->m_Window, L"data\\ folder not found, Please copy over all the Dungeon Keeper assets", L"Missing Assets", MB_OK);
		System::tSys->m_Quitting = true;
	}
	if (dataFType & FILE_ATTRIBUTE_DIRECTORY)
	{
		DWORD ldataFType = GetFileAttributesA("ldata");
		if (ldataFType == INVALID_FILE_ATTRIBUTES)
		{
			System::Print("\"\\ldata\\\" folder not found!");
			MessageBox(Themp::System::tSys->m_Window, L"ldata\\ folder not found, Please copy over all the Dungeon Keeper assets", L"Missing Assets", MB_OK);
			System::tSys->m_Quitting = true;
		}
		if (dataFType & FILE_ATTRIBUTE_DIRECTORY)
		{
			everythingOk = true;
		}
		else
		{
			System::Print("\"\\ldata\\\" is a file not a folder!");
			MessageBox(Themp::System::tSys->m_Window, L"ldata\\ folder not found, ldata is a file not a folder, Please copy over all the Dungeon Keeper assets", L"Missing Assets", MB_OK);
			System::tSys->m_Quitting = true;
		}
	}
	else
	{
		System::Print("\"\\data\\\" is a file not a folder!");
		MessageBox(Themp::System::tSys->m_Window, L"data\\ folder not found, data is a file not a folder, Please copy over all the Dungeon Keeper assets", L"Missing Assets", MB_OK);
		System::tSys->m_Quitting = true;
	}
	if (everythingOk)
	{
		//load all the files
		LoadFilesFromDirectory(L"DATA\\");
		LoadFilesFromDirectory(L"LDATA\\");
		LoadFilesFromDirectory(L"SOUND\\");
		LoadFilesFromDirectory(L"LEVELS\\");
		LoadFilesFromDirectory(L"SAVE\\");
	}
	FileManager::fileManager = this;
}

//dir should end in a "\"
void FileManager::LoadFilesFromDirectory(std::wstring dir)
{
	WIN32_FIND_DATA ffd;
	HANDLE hFind = FindFirstFile((dir + L"*").c_str(), &ffd);
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		System::Print("Something went wrong %S", dir.c_str());
	} 
	while (FindNextFile(hFind, &ffd) != 0)
	{
		// ignore directories
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			// create a full path for each file we find, e.g. "c:\indir\foo.txt"
			std::wstring file_path;
			file_path.reserve(2048);
			file_path = dir + ffd.cFileName;
			std::transform(file_path.begin(), file_path.end(), file_path.begin(), ::towupper);
			FileDataList[file_path] = LoadFileData(file_path);
		}
	}
	FindClose(hFind);
}
//allocates, decompresses (if needed) and returns the file data
FileManager::FileData FileManager::LoadFileData(std::wstring& path)
{
	FileData filedata = { 0 };
	BYTE* rawData = nullptr; 
	int rawFileSize = 0;
	FILE* f = _wfopen(path.c_str(), L"rb");
	bool compressed = false;
	if (f)
	{
		fseek(f, 0, SEEK_END); //go to end of file 
		rawFileSize = ftell(f); //tell the size
		fseek(f, 0, SEEK_SET); //set it back to beginning
		rawData = (BYTE*)malloc(rawFileSize);
		fread(rawData, rawFileSize, 1, f);
		fclose(f);
		if (rawData[0] == 'R' && rawData[1] == 'N' && rawData[2] == 'C' && rawData[3] == 0x01) //RNC magic number
		{
			compressed = true;
		}
	}
	else
	{
		System::Print("Could not open File: %S", path.c_str());
		return filedata;
	}
	if (rawData && rawFileSize != 0)
	{
		if (compressed)
		{
			int uncompressed_size = rnc_ulen(rawData);
			filedata.size = uncompressed_size;
			filedata.data = (BYTE*)malloc(uncompressed_size);
			if (rnc_unpack(rawData, filedata.data, RNC_IGNORE_NONE) < 0)
			{
				System::Print("Failed to unpack %S", path.c_str());
			}
			free(rawData);
		}
		else
		{
			filedata.data = rawData;
			filedata.size = rawFileSize;
		}
	}
	return filedata;
}
FileManager::FileData FileManager::GetFileData(std::wstring path)
{
	FileData f = { 0 };
	auto&& it = FileDataList.find(path);
	if (it != FileDataList.end())
	{
		f = it->second;
	}
	return f;
}