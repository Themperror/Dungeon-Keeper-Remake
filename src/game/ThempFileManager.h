#pragma once
#include <vector>
namespace Themp
{
	struct Texture;
	struct FileData
	{
		BYTE* data;
		size_t size;
	};
	struct Sprite
	{
		Texture* textures;
		int numAnim;
	};
	struct CreatureTab 
	{ 
		unsigned long foffset;
		unsigned char width; //width
		unsigned char height; //height
		unsigned char src_dx ; //src_dx 
		unsigned char src_dy; //src_dy 
		unsigned char rotable;
		unsigned char frames;
		unsigned char xOff; //X
		unsigned char yOff; //Y
		short unscaledW; //unscaled W
		short unscaledH; //unscaled H
	};// sizeof = 16
	struct GUITab
	{
		uint32_t offset;
		uint8_t width, height;
	};
	struct GUITexture
	{
		Texture* texture;
		uint16_t width, height;
	};
	class FileManager
	{
	public:
		~FileManager();
		FileManager();
		static FileManager* fileManager;
		static FileData GetFileData(std::wstring path);
		static Sprite* GetCreatureSprite(int index);
		static GUITexture* GetLevelMiscGUITexture(int index, bool hiRes);
		static GUITexture* GetLevelPaneGUITexture(int index, bool hiRes);
		static GUITexture* GetMenuGUITexture(int index);
		static GUITexture * GetMenuCursorTexture(int index);
		static GUITexture * GetLevelFlagTexture(int index);
		static Texture* GetBlockTexture(int index);
		static std::vector<GUITexture>* GetFont(int source);
	private:
		void LoadFilesFromDirectory(std::wstring dir);
		FileData LoadFileData(std::wstring& path);
		void LoadCreatures();
		void LoadGUITextures(std::wstring datFile, std::wstring tabFile, std::vector<GUITexture>& guiTexVector, bool keepCPUData = false);
		void LoadBlockTextures(std::wstring datFile, std::vector<Texture*>& TexVector);
		void LoadStrings(std::wstring datFile, std::wstring language);
	};
};
