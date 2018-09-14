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

std::unordered_map<std::wstring, FileData> FileDataList;

//Creatures
std::unordered_map<std::wstring, std::vector<CreatureTab>> SpriteFileData;
std::vector<Sprite> CreatureSprites;

////////Level UI

//Esc menu + creature stuff
std::vector<GUITab> Misc_LowGUITextureData; 
std::vector<GUITexture> Misc_LowGUITextures;
std::vector<GUITab> Misc_HiGUITextureData;
std::vector<GUITexture> Misc_HiGUITextures;

//Side pane + Possession elements
std::vector<GUITab> Pane_LowGUITextureData;
std::vector<GUITexture> Pane_LowGUITextures;
std::vector<GUITab> Pane_HiGUITextureData;
std::vector<GUITexture> Pane_HiGUITextures;
////////

//Menu UI
std::vector<GUITab> Menu_GUITextureData;
std::vector<GUITexture> Menu_GUITextures;

std::vector<GUITab> Menu_CursorTextureData;
std::vector<GUITexture> Menu_CursorTextures;

//Localized Text
std::unordered_map<std::wstring, std::vector<std::string>> Localized_Strings; //Key == language, vector contains every string from that language


std::wstring GetFileExtension(std::wstring const& file);

FileManager::~FileManager()
{
	for (auto i : FileDataList)
	{
		free(i.second.data);
	}
	for (auto i : CreatureSprites)
	{
		delete[] i.textures;
	}
	for (auto i : Misc_LowGUITextures)
	{
		delete i.texture;
	}
	for (auto i : Misc_HiGUITextures)
	{
		delete i.texture;
	}
	for (auto i : Pane_LowGUITextures)
	{
		delete i.texture;
	}
	for (auto i : Pane_HiGUITextures)
	{
		delete i.texture;
	}
	for (auto i : Menu_GUITextures)
	{
		delete i.texture;
	}
	for (auto i : Menu_CursorTextures)
	{
		delete i.texture;
	}
	
}
FileData usedPalFile;

FileManager::FileManager()
{
	DWORD dataFType = GetFileAttributesA("data");
	if (dataFType == INVALID_FILE_ATTRIBUTES)
	{
		System::Print("\"\\data\\\" folder not found!");
		MessageBox(Themp::System::tSys->m_Window, L"data\\ folder not found, Please copy over all the Dungeon Keeper assets", L"Missing Assets", MB_OK);
		System::tSys->m_Quitting = true;
		return;
	}
	if (!(dataFType & FILE_ATTRIBUTE_DIRECTORY))
	{
		System::Print("\"\\data\\\" is a file not a folder!");
		MessageBox(Themp::System::tSys->m_Window, L"data\\ folder not found, data is a file not a folder, Please copy over all the Dungeon Keeper assets", L"Missing Assets", MB_OK);
		System::tSys->m_Quitting = true;
		return;
	}
	dataFType = GetFileAttributesA("ldata");
	if (dataFType == INVALID_FILE_ATTRIBUTES)
	{
		System::Print("\"\\ldata\\\" folder not found!");
		MessageBox(Themp::System::tSys->m_Window, L"ldata\\ folder not found, Please copy over all the Dungeon Keeper assets", L"Missing Assets", MB_OK);
		System::tSys->m_Quitting = true;
		return;
	}
	if (!(dataFType & FILE_ATTRIBUTE_DIRECTORY))
	{
		System::Print("\"\\ldata\\\" is a file not a folder!");
		MessageBox(Themp::System::tSys->m_Window, L"ldata\\ folder not found, ldata is a file not a folder, Please copy over all the Dungeon Keeper assets", L"Missing Assets", MB_OK);
		System::tSys->m_Quitting = true;
		return;
	}
	dataFType = GetFileAttributesA("sound");
	if (dataFType == INVALID_FILE_ATTRIBUTES)
	{
		System::Print("\"\\sound\\\" folder not found!");
		MessageBox(Themp::System::tSys->m_Window, L"sound\\ folder not found, Please copy over all the Dungeon Keeper assets", L"Missing Assets", MB_OK);
		System::tSys->m_Quitting = true;
		return;
	}
	if (!(dataFType & FILE_ATTRIBUTE_DIRECTORY))
	{
		System::Print("\"\\sound\\\" is a file not a folder!");
		MessageBox(Themp::System::tSys->m_Window, L"sound\\ folder not found, sound is a file not a folder, Please copy over all the Dungeon Keeper assets", L"Missing Assets", MB_OK);
		System::tSys->m_Quitting = true;
		return;
	}
	dataFType = GetFileAttributesA("levels");
	if (dataFType == INVALID_FILE_ATTRIBUTES)
	{
		System::Print("\"\\levels\\\" folder not found!");
		MessageBox(Themp::System::tSys->m_Window, L"levels\\ folder not found, Please copy over all the Dungeon Keeper assets", L"Missing Assets", MB_OK);
		System::tSys->m_Quitting = true;
		return;
	}
	if (!(dataFType & FILE_ATTRIBUTE_DIRECTORY))
	{
		System::Print("\"\\levels\\\" is a file not a folder!");
		MessageBox(Themp::System::tSys->m_Window, L"levels\\ folder not found, levels is a file not a folder, Please copy over all the Dungeon Keeper assets", L"Missing Assets", MB_OK);
		System::tSys->m_Quitting = true;
		return;
	}

	//load all the files
	LoadFilesFromDirectory(L"DATA\\");
	//These folders might not be present depending on whether the game was properly installed or copied over from disc
	LoadFilesFromDirectory(L"DATA\\DUTCH\\");
	LoadFilesFromDirectory(L"DATA\\ENGLISH\\");
	LoadFilesFromDirectory(L"DATA\\FRENCH\\");
	LoadFilesFromDirectory(L"DATA\\GERMAN\\");
	LoadFilesFromDirectory(L"DATA\\ITALIAN\\");
	LoadFilesFromDirectory(L"DATA\\POLISH\\");
	LoadFilesFromDirectory(L"DATA\\SPANISH\\");
	LoadFilesFromDirectory(L"DATA\\SWEDISH\\");

	LoadFilesFromDirectory(L"LDATA\\");
	LoadFilesFromDirectory(L"SOUND\\");
	LoadFilesFromDirectory(L"LEVELS\\");
	LoadFilesFromDirectory(L"SAVE\\");

	usedPalFile = GetFileData(L"DATA\\MAIN.PAL");

	//Load specific data from previous files
	LoadCreatures();
	LoadGUITextures(L"DATA\\GUI.DAT", L"DATA\\GUI.TAB",		Misc_LowGUITextureData, Misc_LowGUITextures);
	LoadGUITextures(L"DATA\\GUIHI.DAT", L"DATA\\GUIHI.TAB", Misc_HiGUITextureData , Misc_HiGUITextures);
	//LoadGUITextures(L"DATA\\GUI2-0-0.DAT", L"DATA\\GUI2-0-0.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);
	//LoadGUITextures(L"DATA\\GUI2-0-1.DAT", L"DATA\\GUI2-0-1.TAB", Pane_HiGUITextureData, Pane_HiGUITextures);

	//Looks like..Water/Lava heightmaps?
	//LoadGUITextures(L"DATA\\MENUBLK1.DAT", L"DATA\\MENUBLK1.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);

	//developer test assets? Why are these even ingame
	//LoadGUITextures(L"DATA\\FRAME0-0.DAT", L"DATA\\FRAME0-0.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);
	//LoadGUITextures(L"DATA\\FRAME0-1.DAT", L"DATA\\FRAME0-1.TAB", Pane_HiGUITextureData, Pane_HiGUITextures);

	//must be a frame border for something, skulls on the corner, I cannot place this..
	//LoadGUITextures(L"DATA\\BORD0-0.DAT", L"DATA\\BORD0-0.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);
	//LoadGUITextures(L"DATA\\BORD0-1.DAT", L"DATA\\BORD0-1.TAB", Pane_HiGUITextureData, Pane_HiGUITextures);

	//Mix of Testing assets and released creature icons (these exist in GUI.DAT)
	//LoadGUITextures(L"DATA\\PORT0-0.DAT", L"DATA\\PORT0-0.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);
	//LoadGUITextures(L"DATA\\PORT0-1.DAT", L"DATA\\PORT0-1.TAB", Pane_HiGUITextureData, Pane_HiGUITextures);

	//More placeholder developer icons...
	//LoadGUITextures(L"DATA\\ROOM0-0.DAT", L"DATA\\ROOM0-0.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);
	//LoadGUITextures(L"DATA\\ROOM0-1.DAT", L"DATA\\ROOM0-1.TAB", Pane_HiGUITextureData, Pane_HiGUITextures);

	//Map editor icons perhaps? (only a few of them)
	//LoadGUITextures(L"DATA\\EDITICN1.DAT", L"DATA\\EDITICN1.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);
	//LoadGUITextures(L"DATA\\EDITICN2.DAT", L"DATA\\EDITICN2.TAB", Pane_HiGUITextureData, Pane_HiGUITextures);

	//Seems to be "ingame" text font
	//LoadGUITextures(L"DATA\\FONT0-0.DAT", L"DATA\\FONT0-0.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);
	//LoadGUITextures(L"DATA\\FONT0-1.DAT", L"DATA\\FONT0-1.TAB", Pane_HiGUITextureData, Pane_HiGUITextures);
	
	//Might be main menu font, very similar to the "ingame" font, slightly more readable
	//LoadGUITextures(L"DATA\\FONT2-0.DAT", L"DATA\\FONT2-0.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);
	//LoadGUITextures(L"DATA\\FONT2-1.DAT", L"DATA\\FONT2-1.TAB", Pane_HiGUITextureData, Pane_HiGUITextures);

	//"bigger" font, low res completely unreadable, uses a different palette because these are dark/grey with MAIN.PAL
	//LoadGUITextures(L"DATA\\LOFONT.DAT", L"DATA\\LOFONT.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);
	//LoadGUITextures(L"DATA\\HIFONT.DAT", L"DATA\\HIFONT.TAB", Pane_HiGUITextureData, Pane_HiGUITextures);

	//Part of the Info pane when selecting a creature (has a weird full HP/MAGIC bar texture which might be unused as there is no "mana" in this game)
	//LoadGUITextures(L"DATA\\STAT0-0.DAT", L"DATA\\STAT0-0.TAB", Pane_HiGUITextureData, Pane_HiGUITextures);
	
	//Mistress claw attack?
	//LoadGUITextures(L"DATA\\SWIPE1.DAT", L"DATA\\SWIPE1.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);
	//LoadGUITextures(L"DATA\\SWIPE1.DAT", L"DATA\\SWIPE1.TAB", Pane_HiGUITextureData, Pane_HiGUITextures);

	//Warlock Staff melee Frame 1
	//LoadGUITextures(L"DATA\\SWPE00.DAT", L"DATA\\SWPE00.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);

	//Imp pickaxe melee?
	//LoadGUITextures(L"DATA\\SWPE01.DAT", L"DATA\\SWPE01.TAB", Pane_HiGUITextureData, Pane_HiGUITextures);
	//LoadGUITextures(L"DATA\\SWPE02.DAT", L"DATA\\SWPE02.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);
	//LoadGUITextures(L"DATA\\SWPE03.DAT", L"DATA\\SWPE03.TAB", Pane_HiGUITextureData, Pane_HiGUITextures);

	//Warlock Staff Melee Rest of the frames?
	//LoadGUITextures(L"DATA\\SWPE04.DAT", L"DATA\\SWPE04.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);
	//LoadGUITextures(L"DATA\\SWPE05.DAT", L"DATA\\SWPE05.TAB", Pane_HiGUITextureData, Pane_HiGUITextures);

	//Scythe, Horned Reaper perhaps?
	//LoadGUITextures(L"DATA\\SWPE06.DAT", L"DATA\\SWPE06.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);
	//LoadGUITextures(L"DATA\\SWPE07.DAT", L"DATA\\SWPE07.TAB", Pane_HiGUITextureData, Pane_HiGUITextures);

	//No clue, I'll assume the rest of them are also melee attacks
	//LoadGUITextures(L"DATA\\SWPE08.DAT", L"DATA\\SWPE08.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);
	//LoadGUITextures(L"DATA\\SWPE09.DAT", L"DATA\\SWPE09.TAB", Pane_HiGUITextureData, Pane_HiGUITextures);

	//Low res creature icons
	//LoadGUITextures(L"DATA\\TICON0-0.DAT", L"DATA\\TICON0-0.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);

	//These are SO mixed up, real icon together with developer testing art I don't even know if this is useful data or not..
	//LoadGUITextures(L"DATA\\ICONS0-0.DAT", L"DATA\\ICONS0-0.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);
	//LoadGUITextures(L"DATA\\ICONS0-1.DAT", L"DATA\\ICONS0-1.TAB", Pane_HiGUITextureData, Pane_HiGUITextures);

	//circular blob and one divided in 4 donut, no clue, but from filename it must be some alpha mask
	//LoadGUITextures(L"DATA\\ALPHA.DAT", L"DATA\\ALPHA.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);

	//Text frame? Might be legit assets
	//LoadGUITextures(L"DATA\\HBUY0-0.DAT", L"DATA\\HBUY0-0.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);

	//I can't believe it.. REAL assets? 
	//This is the cursor for every part in the entire game, Spells & construction included. Only missing the hand of evil 
	//LoadGUITextures(L"DATA\\LPOINTER.DAT", L"DATA\\LPOINTER.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);
	//LoadGUITextures(L"DATA\\HPOINTER.DAT", L"DATA\\HPOINTER.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);

	//Same icons as above but.. fewer of them?
	//LoadGUITextures(L"DATA\\LPOINTS.DAT", L"DATA\\LPOINTS.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);
	//LoadGUITextures(L"DATA\\HPOINTS.DAT", L"DATA\\HPOINTS.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);


	//////LDATA stuff now

	//TORTURE screen door animations, uses TORTURE.PAL for palette
	//usedPalFile = GetFileData(L"LDATA\\TORTURE.PAL");
	//LoadGUITextures(L"LDATA\\DOOR01.DAT", L"LDATA\\DOOR01.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);
	//LoadGUITextures(L"LDATA\\DOOR02.DAT", L"LDATA\\DOOR02.TAB", Pane_HiGUITextureData, Pane_HiGUITextures);
	//LoadGUITextures(L"LDATA\\DOOR03.DAT", L"LDATA\\DOOR03.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);
	//LoadGUITextures(L"LDATA\\DOOR04.DAT", L"LDATA\\DOOR04.TAB", Pane_HiGUITextureData, Pane_HiGUITextures);

	//Flag thats on the level select screen
	//usedPalFile = GetFileData(L"LDATA\\DKMAP00.PAL");
	//LoadGUITextures(L"LDATA\\DKFLAG00.DAT", L"LDATA\\DKFLAG00.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);
	//LoadGUITextures(L"LDATA\\DKFLAG00.DAT", L"LDATA\\DKFLAG00.TAB", Pane_HiGUITextureData, Pane_HiGUITextures);

	//MAIN MENU TEXTURE FILES!!
	usedPalFile = GetFileData(L"LDATA\\FRONT.PAL");
	
	//main UI textures
	LoadGUITextures(L"LDATA\\FRONTBIT.DAT", L"LDATA\\FRONTBIT.TAB", Menu_GUITextureData, Menu_GUITextures);
	//menu UI font comes in 4 colors RED, WHITE, RED, DARK RED
	//LoadGUITextures(L"LDATA\\FRONTFT1.DAT", L"LDATA\\FRONTFT1.TAB", Pane_HiGUITextureData, Pane_HiGUITextures);
	//LoadGUITextures(L"LDATA\\FRONTFT2.DAT", L"LDATA\\FRONTFT2.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);
	//LoadGUITextures(L"LDATA\\FRONTFT3.DAT", L"LDATA\\FRONTFT3.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);
	//LoadGUITextures(L"LDATA\\FRONTFT4.DAT", L"LDATA\\FRONTFT4.TAB", Pane_HiGUITextureData, Pane_HiGUITextures);

	usedPalFile = GetFileData(L"LDATA\\TORTURE.PAL");
	//usedPalFile = GetFileData(L"DATA\\MAIN.PAL");
	//LoadGUITextures(L"LDATA\\MAPHAND.DAT", L"LDATA\\MAPHAND.TAB", Menu_CursorTextureData, Menu_CursorTextures);
	LoadGUITextures(L"LDATA\\FRONTTOR.DAT", L"LDATA\\FRONTTOR.TAB", Menu_CursorTextureData, Menu_CursorTextures);
	//LoadGUITextures(L"DATA\\HPOINTS.DAT", L"DATA\\HPOINTS.TAB", Menu_CursorTextureData, Menu_CursorTextures);

	//Create INSTALLED entry, this is how the game looks at the "current" language, it overwrites the file with the one you install as language
	LoadStrings(L"DATA\\TEXT.DAT", L"INSTALLED");
	if (Localized_Strings[L"INSTALLED"].size() == 0)
	{
		//The game was not "installed" but just copied over, we'll default to english then..
		LoadStrings(L"DATA\\ENGLISH\\TEXT.DAT", L"INSTALLED");
	}
	LoadStrings(L"DATA\\DUTCH\\TEXT.DAT", L"DUTCH");
	LoadStrings(L"DATA\\ENGLISH\\TEXT.DAT", L"ENGLISH");
	LoadStrings(L"DATA\\FRENCH\\TEXT.DAT", L"FRENCH");
	LoadStrings(L"DATA\\GERMAN\\TEXT.DAT", L"GERMAN");
	LoadStrings(L"DATA\\ITALIAN\\TEXT.DAT", L"ITALIAN");
	LoadStrings(L"DATA\\POLISH\\TEXT.DAT", L"POLISH");
	LoadStrings(L"DATA\\SPANISH\\TEXT.DAT", L"SPANISH");
	LoadStrings(L"DATA\\SWEDISH\\TEXT.DAT", L"SWEDISH");
	if (Localized_Strings.size() == 0)
	{
		System::Print("Could not load any text files!");
		MessageBox(Themp::System::tSys->m_Window, L"Missing text.dat files, Please copy over all the Dungeon Keeper assets", L"Missing Assets", MB_OK);
		System::tSys->m_Quitting = true;
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
FileData FileManager::LoadFileData(std::wstring& path)
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
FileData FileManager::GetFileData(std::wstring path)
{
	FileData f = { 0 };
	for (size_t i = 0; i < path.size(); i++)
	{
		if (!iswupper(path[i]))
		{
			path[i] = towupper(path[i]);
		}
	}
	auto&& it = FileDataList.find(path);
	if (it != FileDataList.end())
	{
		f = it->second;
	}
	return f;
}
Sprite* Themp::FileManager::GetCreatureSprite(int index)
{
	index = index % CreatureSprites.size();
	return &CreatureSprites[index];
}
GUITexture* Themp::FileManager::GetLevelMiscGUITexture(int index, bool hiRes)
{
	if (hiRes)
	{
		index = index % Misc_HiGUITextures.size();
		return &Misc_HiGUITextures[index];
	}
	else
	{
		index = index % Misc_LowGUITextures.size();
		return &Misc_LowGUITextures[index];
	}
}
GUITexture* Themp::FileManager::GetLevelPaneGUITexture(int index, bool hiRes)
{
	if (hiRes)
	{
		index = index % Pane_HiGUITextures.size();
		return &Pane_HiGUITextures[index];
	}
	else
	{
		index = index % Pane_LowGUITextures.size();
		return &Pane_LowGUITextures[index];
	}
}
GUITexture* Themp::FileManager::GetMenuGUITexture(int index)
{
	index = index % Menu_GUITextures.size();
	return &Menu_GUITextures[index];
}
GUITexture* Themp::FileManager::GetMenuCursorTexture(int index)
{
	index = index % Menu_CursorTextures.size();
	return &Menu_CursorTextures[index];
}
void FileManager::LoadCreatures()
{
	FileData creatureJTY = GetFileData(L"DATA\\CREATURE.JTY");
	FileData creatureTAB = GetFileData(L"DATA\\CREATURE.TAB");

	//create sprite table
	auto&& it = SpriteFileData[L"DATA\\CREATURE.TAB"];
	CreatureTab spriteData = {0};
	size_t i = 0;
	while (i < creatureTAB.size)
	{
		int32_t t = 0;
		//read offset
		t =  (int32_t)creatureTAB.data[i++];
		t += (int32_t)creatureTAB.data[i++] << 8;
		t += (int32_t)creatureTAB.data[i++] << 16;
		t += (int32_t)creatureTAB.data[i++] << 24;
		spriteData.foffset = t;

		spriteData.width = creatureTAB.data[i++];
		spriteData.height = creatureTAB.data[i++];

		//read anim
		spriteData.src_dx = creatureTAB.data[i++];
		spriteData.src_dy = creatureTAB.data[i++];
		spriteData.rotable = creatureTAB.data[i++];
		spriteData.frames = creatureTAB.data[i++];

		spriteData.xOff = creatureTAB.data[i++];
		spriteData.yOff = creatureTAB.data[i++];

		spriteData.unscaledW =  (int16_t)creatureTAB.data[i++];
		spriteData.unscaledW += (int16_t)creatureTAB.data[i++] << 8;
		spriteData.unscaledH =  (int16_t)creatureTAB.data[i++];
		spriteData.unscaledH += (int16_t)creatureTAB.data[i++] << 8;

		it.push_back(spriteData);
	}

	//create sprites out of the data we just read
	Sprite sprite = { 0 };
	FileData palData = FileManager::GetFileData(L"DATA\\MAIN.PAL");

	for (size_t i = 0; i < it.size()-1; i+= it[i].frames) //BUG TODO it.size()-1 might not be correct, I believe it skips the very last frame of the last sprite in the list
	{
		sprite.numAnim = it[i].frames;
		sprite.textures = new Texture[sprite.numAnim];
	
		int width = it[i].src_dx;
		int height = it[i].src_dy;

		BYTE* textureData = textureData = (BYTE*)malloc(width * height * 4);
		memset(textureData, 0, width * height * 4);
		
		for (int j = 0; j < sprite.numAnim; j++)
		{
			memset(textureData, 0, width * height * 4);
			CreatureTab& sData = it[i + j];
			BYTE* startOff = (creatureJTY.data + sData.foffset); //palletize the creature data with MAIN.PAL

			int srcIndex = 0;
			//"if positive, number of non-transparent pixels. If negative, number of pixels to leave transparent. If 0, this is the end of the row. "
			for (size_t y = sData.yOff; y < height; y++)
			{
				size_t x = sData.xOff;
				if (y - sData.yOff >= sData.height)continue;
				while(true)
				{ 
					char val = startOff[srcIndex];
					if (val < 0)
					{
						for (size_t z = 0; z < abs(val)-1; z++)
						{
							textureData[(x + y * width) * 4] = 0;
							textureData[(x + y * width) * 4 + 1] = 0;
							textureData[(x + y * width) * 4 + 2] = 0;
							textureData[(x + y * width) * 4 + 3] = 0;
							x++;
						}
						srcIndex++;
						continue;
					}
					else if (val == 0)
					{
						srcIndex++;
						break;
					}
					else if (val > 0)
					{
						for (size_t z = 0; z < val; z++)
						{
							srcIndex++;
							textureData[(x + y * width) * 4] = palData.data[startOff[srcIndex] * 3] * 4;
							textureData[(x + y * width) * 4 + 1] = palData.data[startOff[srcIndex] * 3 + 1] * 4;
							textureData[(x + y * width) * 4 + 2] = palData.data[startOff[srcIndex] * 3 + 2] * 4;
							textureData[(x + y * width) * 4 + 3] = 255;
							x++;
						}
						srcIndex++;
						continue;
					}
					if (x - sData.xOff >= sData.width) break;
				}
			}
			sprite.textures[j].Create(width, height, DXGI_FORMAT_R8G8B8A8_UNORM, false, textureData);
		}
		free(textureData);
		CreatureSprites.push_back(sprite);
	}
	System::Print("Done Creating Sprites!!");
}
void FileManager::LoadGUITextures(std::wstring datFile, std::wstring tabFile, std::vector<GUITab>& tabVector, std::vector<GUITexture>& guiTexVector)
{
	FileData guiData = GetFileData(datFile);
	FileData guiTab = GetFileData(tabFile);

	{
		int i = 0;
		GUITab tab;
		while (i < guiTab.size)
		{
			tab.offset = guiTab.data[i++];
			tab.offset += guiTab.data[i++] << 8;
			tab.offset += guiTab.data[i++] << 16;
			tab.offset += guiTab.data[i++] << 24;
			tab.width = guiTab.data[i++];
			tab.height = guiTab.data[i++];
			tabVector.push_back(tab);
		}
	}

	FileData palData = usedPalFile;
	for (size_t i = 0; i < tabVector.size(); i++)
	{
		GUITab& guiTabData = tabVector[i];
		GUITexture guiTex = { 0 };
		guiTex.texture = new Texture();
		guiTex.width = guiTabData.width;
		guiTex.height = guiTabData.height;
		int width = guiTex.width;
		int height = guiTex.height;
		if (height == 0 || width == 0)
		{
			continue;
		}

		BYTE* textureData = textureData = (BYTE*)malloc(width * height * 4);
		memset(textureData, 0, width * height * 4);

		BYTE* startOff = (guiData.data + guiTabData.offset);

		int srcIndex = 0;
		//"if positive, number of non-transparent pixels. If negative, number of pixels to leave transparent. If 0, this is the end of the row. "
		for (size_t y = 0; y < height; y++)
		{
			size_t x = 0;
			if (y >= height)continue;
			while (true)
			{
				char val = startOff[srcIndex];
				if (val < 0)
				{
					for (size_t z = 0; z < abs(val) - 1; z++)
					{
						textureData[(x + y * width) * 4] = 0;
						textureData[(x + y * width) * 4 + 1] = 0;
						textureData[(x + y * width) * 4 + 2] = 0;
						textureData[(x + y * width) * 4 + 3] = 0;
						x++;
					}
					srcIndex++;
					continue;
				}
				else if (val == 0)
				{
					srcIndex++;
					break;
				}
				else if (val > 0)
				{
					for (size_t z = 0; z < val; z++)
					{
						srcIndex++;
						textureData[(x + y * width) * 4] = palData.data[startOff[srcIndex] * 3] * 4;
						textureData[(x + y * width) * 4 + 1] = palData.data[startOff[srcIndex] * 3 + 1] * 4;
						textureData[(x + y * width) * 4 + 2] = palData.data[startOff[srcIndex] * 3 + 2] * 4;
						textureData[(x + y * width) * 4 + 3] = 255;
						x++;
					}
					srcIndex++;
					continue;
				}
				if (x >= width) break;
			}
		}
		guiTex.texture->Create(width, height, DXGI_FORMAT_R8G8B8A8_UNORM, false, textureData);

		free(textureData);
		guiTexVector.push_back(guiTex);
	}
}
void FileManager::LoadStrings(std::wstring datFile, std::wstring language)
{
	FileData stringFile = GetFileData(datFile);
	std::vector<std::string>& strings = Localized_Strings[language]; 
	std::string buf = "";
	buf.reserve(255);
	for (size_t i = 0; i < stringFile.size; i++)
	{
		if (stringFile.data[i] == 0x0)
		{
			strings.push_back(buf);
			buf.clear();
		}
		else
		{
			buf.append(1,stringFile.data[i]);
		}
	}
	

}
//std::wstring GetFileExtension(std::wstring& file)
//{
//	std::wstring ext = L"";
//	for (int i = file.size()-1; i > 0; i--)
//	{
//		if (file[i] == L'.')
//		{
//			break;
//		}
//		ext.push_back(file[i]);
//	}
//	std::reverse(ext.begin(), ext.end());
//	return ext;
//}

//improved version recommended to me
std::wstring GetFileExtension(std::wstring const& file) 
{
	const auto dot_pos = file.rfind(L'.');
	if (dot_pos == file.npos) return L"";

	return std::wstring(file.begin() + dot_pos + 1, file.end());
}