#pragma once

#include <d3d11.h>

namespace Themp
{
	struct Texture;
	struct Sound;
	struct FileData;
	class Video
	{
	public:
		Video();
		Video(FileData* file);
		bool Load(BYTE * data, uint32_t size);
		int Play();
		void Stop();
		~Video();

		bool isLoaded = false;
		int m_CurrentFrame = 0;
		int m_AudioFrame = 0;

		float m_TotalTime = 0;
		float m_ExtraTime = 0;
		int m_TimeInFrames = 0;
		int m_AudioFrameInBytes = 0;

		float m_Fps = 0;
		unsigned char* m_DePalettizedImage = nullptr;
		Texture* m_Tex = nullptr;

		unsigned long m_Width, m_Height, m_NumFrames;

		Timer videoTimer;
		Sound* sound;
	};
};