#pragma once
#include <vector>
#include <string>
#include <unordered_map>
namespace Themp
{
	struct Texture;
	class Sound;
	struct FileData
	{
		BYTE* data;
		size_t size;
		int currentOffset = 0;
		int8_t ReadSInt8()
		{
			return (int8_t)data[currentOffset++];
		}
		int16_t ReadSInt16()
		{
			int16_t v = (int16_t)data[currentOffset++];
			v += ((int16_t)data[currentOffset++]) << 8;
			return v;
		}
		int32_t ReadSInt32()
		{
			int32_t v = (int32_t)data[currentOffset++];
			v += ((int32_t)data[currentOffset++]) << 8;
			v += ((int32_t)data[currentOffset++]) << 16;
			v += ((int32_t)data[currentOffset++]) << 24;
			return v;
		}
		int64_t ReadSInt64()
		{
			int64_t v = (int64_t)data[currentOffset++];
			v += ((int64_t)data[currentOffset++]) << 8;
			v += ((int64_t)data[currentOffset++]) << 16;
			v += ((int64_t)data[currentOffset++]) << 24;
			v += ((int64_t)data[currentOffset++]) << 32;
			v += ((int64_t)data[currentOffset++]) << 40;
			v += ((int64_t)data[currentOffset++]) << 48;
			v += ((int64_t)data[currentOffset++]) << 56;
			return v;
		}
		uint8_t ReadUInt8()
		{
			uint8_t v = (uint8_t)data[currentOffset++];
			return v;
		}
		uint16_t ReadUInt16()
		{
			uint16_t v = (uint16_t)data[currentOffset++];
			v += ((uint16_t)data[currentOffset++]) << 8;
			return v;
		}
		uint32_t ReadUInt32()
		{
			uint32_t v = (uint32_t)data[currentOffset++];
			v += ((uint32_t)data[currentOffset++]) << 8;
			v += ((uint32_t)data[currentOffset++]) << 16;
			v += ((uint32_t)data[currentOffset++]) << 24;
			return v;
		}
		uint64_t ReadUInt64()
		{
			uint64_t v = (uint64_t)data[currentOffset++];
			v += ((uint64_t)data[currentOffset++]) << 8;
			v += ((uint64_t)data[currentOffset++]) << 16;
			v += ((uint64_t)data[currentOffset++]) << 24;
			v += ((uint64_t)data[currentOffset++]) << 32;
			v += ((uint64_t)data[currentOffset++]) << 40;
			v += ((uint64_t)data[currentOffset++]) << 48;
			v += ((uint64_t)data[currentOffset++]) << 56;
			return v;
		}
		void SeekTo(int offset)
		{
			currentOffset = offset;
		}
		void SkipBytes(int numBytes)
		{
			currentOffset += numBytes;
		}
		void* CurrentAddress() { return &data[currentOffset]; }
		bool IsEnd(){return currentOffset >= size;}
		void SeekToEnd() { currentOffset = size; }
	};
	struct Sprite
	{
		Texture* texture;
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

		//only used for testing purposes, not ingame
		static Sound * GetSoundByIndex(int index);

		static Sound* GetSound(std::string name);
		static std::vector<GUITexture>* GetFont(int source);



	private:
		void LoadFilesFromDirectory(std::wstring dir);
		FileData LoadFileData(std::wstring& path);
		void LoadCreatures();
		void LoadGUITextures(std::wstring datFile, std::wstring tabFile, std::vector<GUITexture>& guiTexVector, bool keepCPUData = false);
		void LoadBlockTextures(std::wstring datFile, std::vector<Texture*>& TexVector);
		void LoadSounds(std::wstring file);
		void LoadStrings(std::wstring datFile, std::wstring language);
	};
};
