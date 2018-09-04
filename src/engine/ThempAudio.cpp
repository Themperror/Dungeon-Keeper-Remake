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
	void OnBufferEnd(void * pBufferContext) { numBuffers--; if (numBuffers == 0)Themp::System::Print("Number of Audio buffers is 0, consider feeding more data!"); }
	void OnBufferStart(void * pBufferContext) {    }
	void OnLoopEnd(void * pBufferContext) {    }
	void OnVoiceError(void * pBufferContext, HRESULT Error) { Themp::System::Print("Some voice error occurred!"); }
	int numBuffers = 0;
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
		m_Initialized = true;
	}
	VoiceCallback vCall;
	Sound* Audio::MakeSoundBuffer(WAVEFORMATEX format)
	{
		HRESULT hr;
		Sound* sound = new Sound();
		sound->format.Format = format;
		sound->format.Samples.wSamplesPerBlock = format.nSamplesPerSec;
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
		bit.buffer.AudioBytes = size;
		bit.data = malloc(size);
		bit.dataSize = size;
		memcpy(bit.data, data, size);
		bit.buffer.pAudioData = (BYTE*)bit.data;
		s->buffers.push_back(bit);
		vCall.numBuffers++;
		s->sound->SubmitSourceBuffer(&s->buffers[s->buffers.size()-1].buffer);
		return S_OK;
	}
	void Audio::Play(Sound* s)
	{
		if (!s->isPlaying)
		{
			s->sound->Start();
			s->isPlaying = true;
		}
	}

	Audio::~Audio()
	{

	}
}