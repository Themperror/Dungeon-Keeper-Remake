#include "ThempSystem.h"
#include "ThempD3D.h"
#include "ThempAudio.h"


#pragma warning( disable : 4996) //disables warning unsafe function: freopen() fopen() .. etc
class VoiceCallback : public IXAudio2VoiceCallback
{
public:
	HANDLE hBufferEndEvent;
	VoiceCallback() : hBufferEndEvent(CreateEvent(NULL, FALSE, FALSE, NULL)) {}
	~VoiceCallback() { CloseHandle(hBufferEndEvent); }

	//Called when the voice has just finished playing a contiguous audio stream.
	void OnStreamEnd() { SetEvent(hBufferEndEvent); }

	//Unused methods are stubs
	void OnVoiceProcessingPassEnd() { }
	void OnVoiceProcessingPassStart(UINT32 SamplesRequired) {    }
	void OnBufferEnd(void * pBufferContext) 
	{ 
		Themp::Sound* s = (Themp::Sound*)pBufferContext;
		Themp::SoundBit& bit = s->buffers.front();
		if (bit.buffer.Flags != XAUDIO2_END_OF_STREAM)
		{
			free(bit.data);
			s->buffers.pop();
			if (s->buffers.size() == 0)
			{
				Themp::System::Print("Number of Audio buffers is 0, consider feeding more data!");
			}
			return;
		}
		s->isPlaying = false;
		if(s->isLooping)
		{
			Themp::System::tSys->m_Audio->Play(s, true);
		}
		
	}
	void OnBufferStart(void * pBufferContext) {    }
	void OnLoopEnd(void * pBufferContext) {    }
	void OnVoiceError(void * pBufferContext, HRESULT Error) { Themp::System::Print("Some voice error occurred!"); }
};
namespace Themp
{
	Audio::Audio()
	{
		HRESULT res = XAudio2Create(&m_Audio, 0, XAUDIO2_DEFAULT_PROCESSOR);
#ifdef _DEBUG
		XAUDIO2_DEBUG_CONFIGURATION dbg = { 0 };
		dbg.TraceMask = XAUDIO2_LOG_ERRORS;
		m_Audio->SetDebugConfiguration(&dbg);
#endif
		if (res != S_OK)
		{
			System::Print("Failed to initialize Audio subsystem");
			return;
		}
		res = m_Audio->CreateMasteringVoice(&m_MasterVoice);
		if (res != S_OK)
		{
			System::Print("Failed to initialize Audio subsystem");
			return;
		}
		m_OneShots.reserve(128);
		m_Sounds.reserve(256);
		m_Initialized = true;
	}
	VoiceCallback vCall;
	Sound* Audio::MakeSoundBuffer(WAVEFORMATEX format)
	{
		HRESULT hr = S_OK;
		Sound* sound = new Sound();
		sound->format.Format = format;
		assert(format.nSamplesPerSec < 65536);
		//sound->format.Samples.wSamplesPerBlock = (WORD)format.nSamplesPerSec;
		hr = m_Audio->CreateSourceVoice(&sound->sound, (WAVEFORMATEX*)&sound->format,0,2.0f, &vCall);
		if (hr != S_OK)
		{
			delete sound;
			return nullptr;
		}
		m_Sounds.push_back(sound);
		return sound;
	}
	
	HRESULT Audio::AddSoundData(Sound* s, void* data, size_t size, bool streaming)
	{
		SoundBit bit;
		bit.buffer = { 0 };
		bit.buffer.Flags = streaming ? 0 : XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer
		bit.buffer.AudioBytes = (uint32_t)size;
		bit.data = malloc(size);
		bit.dataSize = size;
		memcpy(bit.data, data, size);
		bit.buffer.pAudioData = (BYTE*)bit.data;
		bit.buffer.pContext = s;
		s->buffers.push(bit);
		s->sound->SubmitSourceBuffer(&bit.buffer);
		return S_OK;
	}
	void Audio::Play(Sound* s, bool loop)
	{
		if (!s->isPlaying)
		{
			XAUDIO2_VOICE_STATE state;
			s->sound->GetState(&state);
			if (state.BuffersQueued == 0 && s->isLooping)
			{
				if(s->buffers.size() > 0)
					s->sound->SubmitSourceBuffer(&s->buffers.front().buffer);
			}
			s->sound->Start();
			s->isPlaying = true;
			s->isLooping = loop;
		}
	}
	void Audio::PlayOneShot(Sound* original)
	{
		HRESULT hr = S_OK;
		Sound* sound = new Sound();
		sound->isLooping = false;
		sound->format.Format = original->format.Format;
		hr = m_Audio->CreateSourceVoice(&sound->sound, (WAVEFORMATEX*)&sound->format, 0, 2.0f, &vCall);
		if (hr != S_OK)
		{
			delete sound;
			return;
		}
		SoundBit bit = original->buffers.front();
		bit.buffer.Flags = XAUDIO2_END_OF_STREAM; // tell the source voice not to expect any data after this buffer;
		bit.buffer.pContext = sound;
		sound->isOneShot = true;
		m_OneShots.push_back(sound);
		sound->buffers.push(bit);
		sound->sound->SubmitSourceBuffer(&bit.buffer);
		Play(sound);
	}
	void Audio::Stop(Sound* s)
	{
		s->sound->Discontinuity();
		s->sound->Stop(0, 0);
		s->isPlaying = false;
		s->isLooping = false;
	}
	void Audio::MarkEnd(Sound* s)
	{
		s->sound->Discontinuity();
	}

	void Audio::Update()
	{
		for (int i = m_OneShots.size()-1; i >=0 ; i--)
		{
			if(!m_OneShots[i]->isPlaying)
			{
				m_OneShots[i]->sound->DestroyVoice();
				delete m_OneShots[i];
				m_OneShots.erase(m_OneShots.begin() + i);
			}
		}
	}

	Audio::~Audio()
	{

	}
}