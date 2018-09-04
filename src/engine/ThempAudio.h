#pragma once

#include <d3d11.h>
#include <xaudio2.h>
#define STREAMING_BUFFER_SIZE 65536
#define MAX_BUFFER_COUNT 3
namespace Themp
{
	struct SoundBit
	{
		void* data = nullptr;
		size_t dataSize = 0;
		XAUDIO2_BUFFER buffer;
	};
	struct Sound
	{
		Sound()
		{
			format = { 0 };
		}
		bool isPlaying = false;
		IXAudio2SourceVoice* sound = nullptr;
		WAVEFORMATEXTENSIBLE format;
		std::vector<SoundBit> buffers;
	};
	class Audio
	{
	public:
		Audio();
		Sound* MakeSoundBuffer(WAVEFORMATEX format);

		//requires the format to stay the same!
		//HRESULT AddSoundData(Sound * s, void * data, size_t size);
		HRESULT AddSoundData(Sound * s, void * data, size_t size, bool streaming = true);
		void Play(Sound * s);
		~Audio();
		IXAudio2* m_Audio = nullptr;
		IXAudio2MasteringVoice* m_MasterVoice = nullptr;
		bool m_Initialized = false;
		std::vector<Sound*> m_Sounds;
	};
};