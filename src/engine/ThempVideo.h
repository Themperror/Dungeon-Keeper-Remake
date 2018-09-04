#pragma once

#include <d3d11.h>

namespace Themp
{
	struct Texture;
	struct Sound;
	class Video
	{
	public:
		Video();
		bool Load(BYTE * data, size_t size);
		int Play();
		~Video();

		int m_CurrentFrame = 0;
		int m_AudioFrame = 0;
		float m_Fps = 0;
		unsigned char* m_DePalettizedImage = nullptr;
		Texture* m_Tex = nullptr;

		/* file meta-info */
		unsigned long m_Width, m_Height, m_NumFrames;
		/* microseconds per frame */
		double m_usf;

		Timer videoTimer;
		Sound* sound;
	};
};