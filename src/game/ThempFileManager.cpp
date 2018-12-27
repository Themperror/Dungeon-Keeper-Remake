#include "ThempSystem.h"
#include "ThempFileManager.h"
#include "ThempGame.h"
#include "ThempResources.h"
#include "ThempAudio.h"
#include "../Engine/ThempObject3D.h"
#include "../Engine/ThempMesh.h"
#include "../Engine/ThempMaterial.h"
#include "../Engine/ThempD3D.h"
#include "../Engine/ThempFunctions.h"

#define DR_WAV_IMPLEMENTATION

#include <WavLoader.h>


#include <DirectXMath.h>
#include <lbrncbase.h>

#define MKTAG(a0,a1,a2,a3) ((uint32_t)((a3) | ((a2) << 8) | ((a1) << 16) | ((a0) << 24)))

using namespace Themp;
FileManager* FileManager::fileManager = nullptr;

std::unordered_map<std::wstring, FileData> FileDataList;

//Creatures
std::vector<CreatureTab> SpriteFileData;
std::vector<Sprite*> CreatureSprites;

////////Level UI

//Esc menu + creature stuff
std::vector<GUITexture> Level_MiscLowGUITextures;
std::vector<GUITexture> Level_MiscHiGUITextures;

//Side pane + Possession elements
std::vector<GUITexture> Level_PaneLowGUITextures;
std::vector<GUITexture> Level_PaneHiGUITextures;

std::vector<GUITexture> Level_HandTextures;
////////

////////Level Block Textures
std::vector<Texture*> Level_BlockTextures;

//Menu UI
std::vector<GUITexture> Menu_GUITextures;
std::vector<GUITexture> Menu_CursorTextures;
std::vector<GUITexture> Menu_LevelFlagTextures;
///////

//Fonts

std::vector<GUITexture> Font_IngameLowTextures;
std::vector<GUITexture> Font_IngameHiTextures;
std::vector<GUITexture> Font_Menu0Textures;
std::vector<GUITexture> Font_Menu1Textures;
std::vector<GUITexture> Font_Menu2Textures;
std::vector<GUITexture> Font_Menu3Textures;

//Localized Text
std::unordered_map<std::wstring, std::vector<std::string>> Localized_Strings; //Key == language, vector contains every string from that language

//Sounds
std::unordered_map<std::string, Sound*> Sounds;
std::vector<Sound*> AtlasGoodSounds;
std::vector<Sound*> AtlasBadSounds;

std::wstring GetFileExtension(std::wstring const& file);

FileManager::~FileManager()
{
	for (auto i : FileDataList)
	{
		_aligned_free(i.second.data);
	}

	for (auto i : CreatureSprites) { delete i->texture;	delete i;}
	for (auto i : Level_MiscLowGUITextures)	delete i.texture;
	for (auto i : Level_MiscHiGUITextures)	delete i.texture;
	for (auto i : Level_PaneLowGUITextures)	delete i.texture;
	for (auto i : Level_PaneHiGUITextures)	delete i.texture;
	for (auto i : Level_HandTextures)		delete i.texture;
	for (auto i : Menu_GUITextures)			delete i.texture;
	for (auto i : Menu_CursorTextures)		delete i.texture;
	for (auto i : Font_IngameLowTextures)	delete i.texture;
	for (auto i : Font_IngameHiTextures)	delete i.texture;
	for (auto i : Font_Menu0Textures)		delete i.texture;
	for (auto i : Font_Menu1Textures)		delete i.texture;
	for (auto i : Font_Menu2Textures)		delete i.texture;
	for (auto i : Font_Menu3Textures)		delete i.texture;
	for (auto i : Menu_LevelFlagTextures)	delete i.texture;
	for (auto i : Level_BlockTextures)	delete i;
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
	System::Print("Loading language specific data files, These might not exist depending on installation, any errors here are fine!");
	LoadFilesFromDirectory(L"DATA\\DUTCH\\");
	LoadFilesFromDirectory(L"DATA\\ENGLISH\\");
	LoadFilesFromDirectory(L"DATA\\FRENCH\\");
	LoadFilesFromDirectory(L"DATA\\GERMAN\\");
	LoadFilesFromDirectory(L"DATA\\ITALIAN\\");
	LoadFilesFromDirectory(L"DATA\\POLISH\\");
	LoadFilesFromDirectory(L"DATA\\SPANISH\\");
	LoadFilesFromDirectory(L"DATA\\SWEDISH\\");
	System::Print("End Loading language specific data files! Errors matter again!");

	LoadFilesFromDirectory(L"LDATA\\");
	LoadFilesFromDirectory(L"SOUND\\");
	LoadFilesFromDirectory(L"SOUND\\ATLAS\\ENGLISH\\");
	LoadFilesFromDirectory(L"LEVELS\\");
	LoadFilesFromDirectory(L"SAVE\\");

	usedPalFile = GetFileData(L"DATA\\MAIN.PAL");

	//Load specific data from previous files
	LoadCreatures();
	LoadGUITextures(L"DATA\\GUI.DAT", L"DATA\\GUI.TAB", Level_MiscLowGUITextures);
	LoadGUITextures(L"DATA\\GUIHI.DAT", L"DATA\\GUIHI.TAB" , Level_MiscHiGUITextures);
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

	usedPalFile = GetFileData(L"DATA\\PALETTE.DAT");
	LoadBlockTextures(L"DATA\\TMAPA000.DAT", Level_BlockTextures);
	LoadBlockTextures(L"DATA\\TMAPA001.DAT", Level_BlockTextures);
	LoadBlockTextures(L"DATA\\TMAPA002.DAT", Level_BlockTextures);
	LoadBlockTextures(L"DATA\\TMAPA003.DAT", Level_BlockTextures);
	LoadBlockTextures(L"DATA\\TMAPA004.DAT", Level_BlockTextures);
	LoadBlockTextures(L"DATA\\TMAPA005.DAT", Level_BlockTextures);
	LoadBlockTextures(L"DATA\\TMAPA006.DAT", Level_BlockTextures);
	LoadBlockTextures(L"DATA\\TMAPA007.DAT", Level_BlockTextures);
	//LoadBlockTextures(L"DATA\\TMAPA000.DAT", );


	//////LDATA stuff now

	//TORTURE screen door animations, uses TORTURE.PAL for palette
	//usedPalFile = GetFileData(L"LDATA\\TORTURE.PAL");
	//LoadGUITextures(L"LDATA\\DOOR01.DAT", L"LDATA\\DOOR01.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);
	//LoadGUITextures(L"LDATA\\DOOR02.DAT", L"LDATA\\DOOR02.TAB", Pane_HiGUITextureData, Pane_HiGUITextures);
	//LoadGUITextures(L"LDATA\\DOOR03.DAT", L"LDATA\\DOOR03.TAB", Pane_LowGUITextureData, Pane_LowGUITextures);
	//LoadGUITextures(L"LDATA\\DOOR04.DAT", L"LDATA\\DOOR04.TAB", Pane_HiGUITextureData, Pane_HiGUITextures);

	//Flag thats on the level select screen
	usedPalFile = GetFileData(L"LDATA\\DKMAP00.PAL");
	LoadGUITextures(L"LDATA\\DKFLAG00.DAT", L"LDATA\\DKFLAG00.TAB", Menu_LevelFlagTextures);

	//MAIN MENU TEXTURE FILES!!
	usedPalFile = GetFileData(L"LDATA\\FRONT.PAL");
	
	//main UI textures
	LoadGUITextures(L"LDATA\\FRONTBIT.DAT", L"LDATA\\FRONTBIT.TAB", Menu_GUITextures);
	//menu UI font comes in 4 colors RED, WHITE, RED, DARK RED
	LoadGUITextures(L"LDATA\\FRONTFT1.DAT", L"LDATA\\FRONTFT1.TAB", Font_Menu0Textures,true);
	LoadGUITextures(L"LDATA\\FRONTFT2.DAT", L"LDATA\\FRONTFT2.TAB", Font_Menu1Textures,true);
	LoadGUITextures(L"LDATA\\FRONTFT3.DAT", L"LDATA\\FRONTFT3.TAB", Font_Menu2Textures,true);
	LoadGUITextures(L"LDATA\\FRONTFT4.DAT", L"LDATA\\FRONTFT4.TAB", Font_Menu3Textures,true);

	//"bigger" font, low res completely unreadable, uses a different palette because these are dark/grey with MAIN.PAL
	LoadGUITextures(L"DATA\\LOFONT.DAT", L"DATA\\LOFONT.TAB", Font_IngameLowTextures,true);
	LoadGUITextures(L"DATA\\HIFONT.DAT", L"DATA\\HIFONT.TAB", Font_IngameHiTextures,true);

	usedPalFile = GetFileData(L"LDATA\\TORTURE.PAL");
	//usedPalFile = GetFileData(L"DATA\\MAIN.PAL");
	//LoadGUITextures(L"LDATA\\MAPHAND.DAT", L"LDATA\\MAPHAND.TAB", Menu_CursorTextureData, Menu_CursorTextures);
	//LoadGUITextures(L"LDATA\\FRONTTOR.DAT", L"LDATA\\FRONTTOR.TAB", Menu_CursorTextureData, Menu_CursorTextures);
	usedPalFile = GetFileData(L"DATA\\MAIN.PAL");
	LoadGUITextures(L"DATA\\HPOINTER.DAT", L"DATA\\HPOINTER.TAB", Menu_CursorTextures);

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

	LoadSounds(L"SOUND\\SOUND.DAT");
	LoadAtlasSpeech();

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
		rawData = (BYTE*)_aligned_malloc(rawFileSize,16);
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
			filedata.data = (BYTE*)_aligned_malloc(uncompressed_size,16);
			if (rnc_unpack(rawData, filedata.data, RNC_IGNORE_NONE) < 0)
			{
				System::Print("Failed to unpack %S", path.c_str());
			}
			_aligned_free(rawData);
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
	return CreatureSprites[index];
}
GUITexture* Themp::FileManager::GetLevelMiscGUITexture(int index, bool hiRes)
{
	if (hiRes)
	{
		index = index % Level_MiscHiGUITextures.size();
		return &Level_MiscHiGUITextures[index];
	}
	else
	{
		index = index % Level_MiscLowGUITextures.size();
		return &Level_MiscLowGUITextures[index];
	}
}
GUITexture* Themp::FileManager::GetLevelPaneGUITexture(int index, bool hiRes)
{
	if (hiRes)
	{
		index = index % Level_PaneHiGUITextures.size();
		return &Level_PaneHiGUITextures[index];
	}
	else
	{
		index = index % Level_PaneLowGUITextures.size();
		return &Level_PaneLowGUITextures[index];
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
GUITexture* Themp::FileManager::GetLevelFlagTexture(int index)
{
	index = index % Menu_LevelFlagTextures.size();
	return &Menu_LevelFlagTextures[index];
}
Texture* Themp::FileManager::GetBlockTexture(int index)
{
	index = index % Level_BlockTextures.size();
	return Level_BlockTextures[index];
}
Sound* Themp::FileManager::GetSoundByIndex(int index)
{
	auto& it = Sounds.begin();
	for (size_t i = 0; i < index; i++)
	{
		it++;
		if (it == Sounds.end())
		{
			it = Sounds.begin();
		}
	}
	return it->second;
}
Sound* Themp::FileManager::GetSound(std::string name)
{
	auto&& it = Sounds.find(name);
	if (it == Sounds.end())
	{
		return nullptr;
	}
	return it->second;
}
Sound* Themp::FileManager::GetAtlasGoodSound(int index)
{
	return AtlasGoodSounds[index % AtlasGoodSounds.size()];
}
Sound* Themp::FileManager::GetAtlasBadSound(int index)
{
	return AtlasBadSounds[index % AtlasBadSounds.size()];
}

std::string FileManager::GetText(int i)
{
	return Localized_Strings[L"INSTALLED"][i];
}
std::vector<GUITexture>* Themp::FileManager::GetFont(int source)
{
	switch (source)
	{
		default: return &Font_Menu0Textures; break;
		case 0: return &Font_IngameLowTextures; break;
		case 1: return &Font_IngameHiTextures; break;
		case 2: return &Font_Menu0Textures; break;
		case 3: return &Font_Menu1Textures; break;
		case 4: return &Font_Menu2Textures; break;
		case 5: return &Font_Menu3Textures; break;
	}
}

void FileManager::LoadAtlasSpeech()
{
	Audio* audio = Themp::System::tSys->m_Audio;
	for (size_t i = 1; i <= 20; i++)
	{
		std::wstring textNr = std::to_wstring(i);
		std::wstring fileNameGood = L"SOUND\\ATLAS\\ENGLISH\\GOOD";
		std::wstring fileNameBad = L"SOUND\\ATLAS\\ENGLISH\\BAD";
		if (i < 10)
		{
			fileNameGood += '0';
			fileNameBad += '0';
		};
		fileNameGood.append(textNr);
		fileNameGood.append(L".WAV");
		fileNameBad.append(textNr);
		fileNameBad.append(L".WAV");

		FileData soundGood = GetFileData(fileNameGood);
		FileData soundBad = GetFileData(fileNameBad);

		{
			//uint32_t tag = soundGood.ReadUInt32();
			//if (tag == MKTAG('F', 'F', 'I', 'R'))
			//{ 
			//	WAVEFORMATEX format = { 0 };
			//	int currentOffset = soundGood.currentOffset;
			//	uint32_t chunkSize = soundGood.ReadUInt32();
			//	//skip WAVE and FMT tag
			//	soundGood.SkipBytes(8);
			//	uint32_t subChunkSize = soundGood.ReadUInt32();
			//	uint16_t audioFormat = soundGood.ReadUInt16();
			//	//assert(audioFormat == 1);
			//
			//	uint16_t numChannels = soundGood.ReadUInt16();
			//	uint32_t sampleRate = soundGood.ReadUInt32();
			//	uint32_t byteRate = soundGood.ReadUInt32();
			//	uint16_t blockAlign = soundGood.ReadUInt16();
			//	uint16_t bitsPerSample = soundGood.ReadUInt16();
			//
			//	if (soundGood.ReadUInt32() == MKTAG('a', 't', 'a', 'd'))
			//	{
			//		uint32_t dataSize = soundGood.ReadUInt32();
			//
			//		format.nSamplesPerSec = sampleRate;
			//		format.nChannels = numChannels;
			//		format.wBitsPerSample = 16;
			//		format.wFormatTag = WAVE_FORMAT_ADPCM;
			//		format.nBlockAlign = blockAlign;
			//		format.nAvgBytesPerSec = byteRate;
			//
			//	}
			//	Sound* s = audio->MakeSoundBuffer(format);
			//	audio->AddSoundData(s, soundGood.CurrentAddress(), (sampleRate * numChannels * bitsPerSample) / 8, false);
			//
			//	AtlasGoodSounds.push_back(s);
			//}
		}
		{
			//drwav wav;
			//drwav_init_memory(&wav, soundBad.data, soundBad.size);
			//ADPCMWAVEFORMAT format = {};
			//format.wfx.wFormatTag = WAVE_FORMAT_ADPCM;
			//format.wfx.nChannels = wav.channels;
			//format.wfx.nSamplesPerSec = wav.sampleRate;
			//format.wfx.wBitsPerSample = wav.bitsPerSample;
			//format.aCoef->iCoef1 = wav.msadpcm.delta[0];
			//format.aCoef->iCoef2 = wav.msadpcm.delta[1];
			//format.wNumCoef = 2;
			//format.wSamplesPerBlock = wav.msadpcm.cachedSampleCount;
			//Sound* s = audio->MakeSoundBuffer(format);
			//audio->AddSoundData(s, &soundBad.data[wav.dataChunkDataPos], wav.dataChunkDataSize, false);
			//
			//AtlasBadSounds.push_back(s);
		}
	}
}
void FileManager::LoadCreatures()
{
	FileData creatureJTY = GetFileData(L"DATA\\CREATURE.JTY");
	FileData creatureTAB = GetFileData(L"DATA\\CREATURE.TAB");

	//create sprite table
	CreatureTab spriteData = {0};
	while (!creatureTAB.IsEnd())
	{
		int32_t t = 0;
		//read offset
		spriteData.foffset = creatureTAB.ReadUInt32();

		spriteData.width = creatureTAB.ReadUInt8();
		spriteData.height = creatureTAB.ReadUInt8();

		//read anim
		spriteData.src_dx = creatureTAB.ReadUInt8();
		spriteData.src_dy = creatureTAB.ReadUInt8();
		spriteData.rotable = creatureTAB.ReadUInt8();
		spriteData.frames = creatureTAB.ReadUInt8();

		spriteData.xOff = creatureTAB.ReadUInt8();
		spriteData.yOff = creatureTAB.ReadUInt8();

		spriteData.unscaledW =  (int16_t)creatureTAB.ReadSInt16();
		spriteData.unscaledH =  (int16_t)creatureTAB.ReadSInt16();

		SpriteFileData.push_back(spriteData);
	}

	//create sprites out of the data we just read
	FileData palData = FileManager::GetFileData(L"DATA\\MAIN.PAL");

	for (size_t i = 0; i < SpriteFileData.size()-1; ( i+= (SpriteFileData[i].frames))) //BUG TODO it.size()-1 might not be correct, I believe it skips the very last frame of the last sprite in the list
	{
		Sprite* sprite = new Sprite();
		sprite->numAnim = SpriteFileData[i].frames;
		sprite->unscaledW = SpriteFileData[i].width;
		sprite->unscaledH = SpriteFileData[i].height;
		int width = SpriteFileData[i].src_dx * sprite->numAnim;
		int singleWidth = SpriteFileData[i].src_dx;
		int height = SpriteFileData[i].src_dy;

		BYTE* textureData = textureData = (BYTE*)_aligned_malloc(width * height * 4,16);
		memset(textureData, 0, width * height * 4);
		
		for (int j = 0; j < sprite->numAnim; j++)
		{
			CreatureTab& sData = SpriteFileData[i + j];
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
							textureData[((x + singleWidth * j) + y * width) * 4] = 0;
							textureData[((x + singleWidth * j) + y * width) * 4 + 1] = 0;
							textureData[((x + singleWidth * j) + y * width) * 4 + 2] = 0;
							textureData[((x + singleWidth * j) + y * width) * 4 + 3] = 0;
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
							textureData[((x + singleWidth * j) + y * width) * 4] = palData.data[startOff[srcIndex] * 3] * 4;
							textureData[((x + singleWidth * j) + y * width) * 4 + 1] = palData.data[startOff[srcIndex] * 3 + 1] * 4;
							textureData[((x + singleWidth * j) + y * width) * 4 + 2] = palData.data[startOff[srcIndex] * 3 + 2] * 4;
							textureData[((x + singleWidth * j) + y * width) * 4 + 3] = 255;
							x++;
						}
						srcIndex++;
						continue;
					}
					if (x - sData.xOff >= sData.width) break;
				}
			}
		}
		sprite->texture = new Texture();
		sprite->texture->Create(width, height, DXGI_FORMAT_R8G8B8A8_UNORM, false, textureData);

		///////TEMPORARY FOR DEV PURPOSES
		//Dump the sprite to disk
//#pragma pack(push,1)
//		struct BITMAPFILEHEADER
//		{
//			uint16_t magic = 19778;
//			uint32_t fileSize = 0;
//			uint16_t reserved0 = 0; //MUST BE 0
//			uint16_t reserved1 = 0; //MUST BE 0
//			uint32_t offsetBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
//		};
//		struct BITMAPINFOHEADER
//		{
//			uint32_t headersize = sizeof(BITMAPINFOHEADER);
//			uint32_t imageWidth;
//			uint32_t imageHeight;
//			uint16_t planes = 0; //must be 0
//			uint16_t bitcount = 32;//bits per pixel
//			uint32_t compression = false;
//			uint32_t sizeImage = 0;
//			uint32_t xPelsMeter = 0;
//			uint32_t yPerlsmeter = 0;
//			uint32_t clrUsed = 0;
//			uint32_t clrImportant = 0;
//		};
//#pragma pack(pop)
//		struct RGBQUAD
//		{
//			RGBQUAD(BYTE* data)
//			{ 
//				//R G B A
//				A = *(data + 3);
//				B = *(data + 2);
//				G = *(data + 1);
//				R = *(data + 0);
//			}
//			uint8_t B, G, R, A; //NOTE THE B G R format
//		};
//		char buf[8] = {0};
//		itoa(CreatureSprites.size(), buf, 10);
//		FILE* creatureSpriteFile = fopen(std::string("Creatures\\Creature").append(buf).append(".bmp").c_str(), "w+b");
//		BITMAPFILEHEADER bmpHeader = BITMAPFILEHEADER();
//		BITMAPINFOHEADER bmpInfoHeader = BITMAPINFOHEADER();
//		bmpHeader.fileSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + width * height * 4;
//		bmpInfoHeader.imageWidth = width;
//		bmpInfoHeader.imageHeight = height;
//
//		fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, creatureSpriteFile);
//		fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, creatureSpriteFile);
//		for (int k = (width * height * 4); k >=0 ; k-=4)
//		{
//			fwrite(&RGBQUAD(&textureData[k - 4]), 4, 1, creatureSpriteFile);
//		}
//		//fwrite(textureData, width * height * 4, 1, creatureSpriteFile);
//		fclose(creatureSpriteFile);
		//////////

		_aligned_free(textureData);
		CreatureSprites.push_back(sprite);
	}
	System::Print("Done Creating Sprites!!");
}
void FileManager::LoadGUITextures(std::wstring datFile, std::wstring tabFile, std::vector<GUITexture>& guiTexVector, bool keepCPUData)
{
	FileData guiData = GetFileData(datFile);
	FileData guiTab = GetFileData(tabFile);
	std::vector<GUITab> tabVector;
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

		BYTE* textureData = textureData = (BYTE*)_aligned_malloc(width * height * 4,16);
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
		guiTex.texture->Create(width, height, DXGI_FORMAT_R8G8B8A8_UNORM, keepCPUData, textureData);

		_aligned_free(textureData);
		guiTexVector.push_back(guiTex);
	}
}
void FileManager::LoadBlockTextures(std::wstring datFile, std::vector<Texture*>& TexVector)
{
	FileData guiData = GetFileData(datFile);
	FileData palData = usedPalFile;

	int width = 256; //each seperate texture is 32x32, there's 8 in 1 row so that leaves 2176 pixels vertically
	int height = 2176; //resulting in 8*68 textures

	Texture* Tex = new Texture();
	BYTE* textureData = textureData = (BYTE*)_aligned_malloc(width * height * 4,16);
	memset(textureData, 0, width * height * 4);
	int xRead = 0;
	int yRead = 0;
	BYTE* startOff = (guiData.data);
	while((xRead + yRead*256) < guiData.size)
	{
		int readPos = xRead + yRead*width;
		textureData[readPos * 4] = palData.data[startOff[readPos] * 3] * 4;
		textureData[readPos * 4 + 1] = palData.data[startOff[readPos] * 3 + 1] * 4;
		textureData[readPos * 4 + 2] = palData.data[startOff[readPos] * 3 + 2] * 4;
		textureData[readPos * 4 + 3] = 255;

		xRead++;
		if (xRead == 256)
		{
			xRead = 0;
			yRead ++;
		}
	}
	Tex->Create(width, height, DXGI_FORMAT_R8G8B8A8_UNORM, false, textureData);
	TexVector.push_back(Tex);
	_aligned_free(textureData);
}

void FileManager::LoadSounds(std::wstring file)
{
	FileData fData = GetFileData(file);
	Audio* audio = Themp::System::tSys->m_Audio;
	struct SoundTab
	{
		//The first thirteen bytes form the internal name of the file (not the one my extractor outputs), followed by five bytes of zeroes, 
		//eight bytes giving the offset of the file in the .dat, then six bytes giving the size of the file.
		std::string name; // 13 chars max
		//char nulls[5];
		uint32_t offset;
		//uint32_t null0;
		uint32_t size;
		//uint16_t null1;
	};
	std::unordered_map<std::string, SoundTab> soundTable;
	fData.SeekTo(fData.size - 4);
	uint32_t tableEnd = fData.ReadUInt32();
	fData.SeekTo(fData.size - 0x3C);
	uint32_t tableStart = fData.ReadUInt32();
	fData.SeekTo(tableStart + 32);

	while (fData.currentOffset < tableEnd)
	{
		//this data line is 32 bytes long
		SoundTab tab;
		tab.name = "";
		tab.name.reserve(13);
		int bytesRead = 0;
		while (bytesRead < 13)
		{
			char byte = fData.ReadSInt8();
			if(byte != 0)
				tab.name.append(1, byte);
			bytesRead++;
		}
		if (tab.name == "NULL.WAV" || soundTable.find(tab.name) != soundTable.end())
		{
			//we read 13 bytes, so skip the next 19 so we land on the next line.
			fData.SkipBytes(19);
			continue;
		}
		fData.SkipBytes(5);
		tab.offset = fData.ReadUInt32();
		fData.SkipBytes(4);
		tab.size = fData.ReadUInt32();
		soundTable[tab.name] = tab;
		fData.SkipBytes(2);
	}

	for(auto&& i : soundTable)
	{
		fData.SeekTo(i.second.offset);
		uint32_t tag = fData.ReadUInt32();
		if (tag == MKTAG('F', 'F', 'I', 'R'))
		{
			WAVEFORMATEX format = { 0 };
			int currentOffset = fData.currentOffset;
			uint32_t chunkSize = fData.ReadUInt32();
			//skip WAVE and FMT tag
			fData.SkipBytes(8);
			uint32_t subChunkSize = fData.ReadUInt32();
			uint16_t audioFormat = fData.ReadUInt16();
			assert(audioFormat == 1);

			uint16_t numChannels = fData.ReadUInt16();
			uint32_t sampleRate = fData.ReadUInt32();
			uint32_t byteRate = fData.ReadUInt32();
			uint16_t blockAlign = fData.ReadUInt16();
			uint16_t bitsPerSample = fData.ReadUInt16();

			//skip 'data' tag
			fData.SkipBytes(4);
			uint32_t dataSize = fData.ReadUInt32();

			format.nSamplesPerSec =sampleRate;
			format.nChannels = numChannels == 2 ? 2 : 1;
			format.wBitsPerSample = bitsPerSample;
			format.wFormatTag = WAVE_FORMAT_PCM;
			format.nBlockAlign = blockAlign;
			format.nAvgBytesPerSec = byteRate;
			Sound* s = audio->MakeSoundBuffer(format);
			audio->AddSoundData(s, fData.CurrentAddress(), dataSize, false);

			Sounds[i.first] = s;
		}
		else
		{
			assert(false);
		}
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