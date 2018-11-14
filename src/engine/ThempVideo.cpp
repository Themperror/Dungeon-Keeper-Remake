#include "ThempSystem.h"
#include "ThempVideo.h"
#include "ThempD3D.h"
#include "ThempMaterial.h"
#include "ThempAudio.h"
#include <chrono>

#include "SmackerDecoder.h"

#pragma warning( disable : 4996) //disables warning unsafe function: freopen() fopen() .. etc

//game include
#include "ThempFileManager.h"

namespace Themp
{
	using namespace Smacker;
	SmackerDecoder* decoder;
	Video::Video()
	{
		decoder = new SmackerDecoder();
	}

	//override for game
	Video::Video(FileData* file)
	{
		decoder = new SmackerDecoder();
		if (Load(file->data, (uint32_t)file->size))
		{
			isLoaded = true;
		}
	}

	bool Video::Load(BYTE* data, uint32_t size)
	{
		if (decoder->loadStream(new MemoryReadStream(data, size, false)))
		{
			//sAudio = smk_open_memory(data, size);
			decoder->readNextPacket();
			m_Fps = decoder->video->getFrameRate();
			m_NumFrames = decoder->video->getFrameCount();
			m_Width = decoder->video->getWidth();
			m_Height = decoder->video->getHeight();
			//System::Print("Opened file %s\nWidth: %d\nHeight: %d\nFrames: %d\nFPS: %f", path.c_str(), w, h, f, m_Fps);
			m_DePalettizedImage = new unsigned char[m_Width*m_Height * 4];
			if (decoder->_header.audioInfo[0].hasAudio)
			{
				const unsigned char *audio_data = decoder->audio[0]->_buffer;
				WAVEFORMATEX format;
				format.nSamplesPerSec = decoder->audio[0]->_audioInfo.sampleRate;
				format.nChannels = decoder->audio[0]->_audioInfo.isStereo ? 2 : 1;
				format.wBitsPerSample = decoder->audio[0]->_audioInfo.is16Bits ? 16 : 24;
				format.wFormatTag = WAVE_FORMAT_PCM;
				format.nBlockAlign = (format.nChannels * format.wBitsPerSample) / 8;
				format.nAvgBytesPerSec = format.nBlockAlign * format.nSamplesPerSec;
				sound = Themp::System::tSys->m_Audio->MakeSoundBuffer(format);
			}
			videoTimer.StartTime();
			isLoaded = true;

			m_Tex = new Texture();
			m_Tex->Create(m_Width, m_Height, DXGI_FORMAT_R8G8B8A8_UNORM, true, m_DePalettizedImage);
			return true;
		}
		else
		{
			System::Print("ThempVideo::Load - Failed to open video file");
			return false;
		}

		return false;
	}
	
	//0 failed, 1 success,2 end reached
	int Video::Play()
	{
		if (!isLoaded)return 0;
		double dif = videoTimer.GetDeltaTimeReset();
		m_TotalTime += (float)dif;
		const float frameMS = 1.0f / m_Fps;
		if (m_TotalTime >= frameMS)
		{
			m_ExtraTime = m_TotalTime - frameMS; //amount time we waited too much
			m_TimeInFrames = (int)(m_ExtraTime / frameMS); //converted to frames
			m_TotalTime = m_ExtraTime;
		}
		else
		{
			return 1;
		}
		if(decoder->video->getCurFrame() != decoder->video->getFrameCount()-1) //we haven't reached the end of the video
		{
			SmackerVideoTrack* vid = decoder->video;
			m_CurrentFrame = vid->getCurFrame();

			/* Retrieve the palette and image */
			const unsigned char *palette_data = vid->getPalette();
			const byte* image_data = (byte*)vid->_surface->getPixels();

			for (size_t i = 0; i < m_Width*m_Height; i++)
			{
				m_DePalettizedImage[(i * 4    )] = palette_data[image_data[i] * 3];
				m_DePalettizedImage[(i * 4 + 1)] = palette_data[image_data[i] * 3+1];
				m_DePalettizedImage[(i * 4 + 2)] = palette_data[image_data[i]* 3 +2];
				m_DePalettizedImage[(i * 4 + 3)] = 255;
			}

			m_Tex->Load((void*)m_DePalettizedImage, m_Width * m_Height * 4);
			
			if (decoder->_header.audioInfo[0].hasAudio)
			{
				Themp::System::tSys->m_Audio->AddSoundData(sound, (void*)decoder->audio[0]->_buffer, decoder->audio[0]->_bufferSize,(m_CurrentFrame < decoder->video->getFrameCount()-2));
				if (!sound->isPlaying)
				{
					Themp::System::tSys->m_Audio->Play(sound);
				}
			}
			/* Advance to next frame */
			decoder->readNextPacket();
			return 1;
		}
		else
		{
			//finally done playing
			Themp::System::tSys->m_Audio->MarkEnd(sound);
			return 2;
		}
		//something went wrong
		return 0;
	}
	void Video::Stop()
	{
		Themp::System::tSys->m_Audio->Stop(sound);
	}
	Video::~Video()
	{
		delete m_Tex;
	}
}