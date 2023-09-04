#pragma once

//#define fourccRIFF 'FFIR'
//#define fourccDATA 'atad'
//#define fourccFMT  ' tmf'
//#define fourccWAVE 'EVAW'
//#define fourccXWMA 'AMWX'
//#define fourccDPDS 'sdpd'
//
//class Sound final : public SingletonBase<Sound>
//{
//public:
//	friend SingletonBase<Sound>;
//
//	void Play();
//	void Stop();
//	void Pause();
//
//	void Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParam, LPARAM const lParam);
//
//	const char* GetContent() { return content; }
//	
//	void SetVolume(float value) { volume = value; }
//	void SetLoop(bool value) { loop = value; }
//	void SetPuase(bool value) { bPause = value; }
//
//	const float GetVolume() { return volume; }
//	const bool IsLoop() { return loop; }
//	const bool IsPause() { return bPause; }
//
//	struct Handle final
//	{
//		IXAudio2SourceVoice* SourceVoice = nullptr;
//		XAUDIO2_BUFFER Buffer = { 0 };
//	};
//
//public:
//	static HRESULT FindAudio(HANDLE hAudio, DWORD fourcc, DWORD& dwAudioSize, DWORD& dwAudioDataPosition);
//	static HRESULT ReadAudioData(HANDLE hFile, void* buffer, DWORD bufferSize, DWORD bufferOffset);
//	static void Import(string const& file);
//
//private:
//	Sound();
//	~Sound();
//
//private:
//	char const* content = nullptr;
//	
//	float volume = 0.0f;
//	bool loop = false;
//
//	bool bPause = false;
//
//	static unordered_map<string, Handle*> storage;
//
//	static IXAudio2* XAudio2;
//	static IXAudio2MasteringVoice* MasteringVoice;
//	static WAVEFORMATEXTENSIBLE wfx;
//};

using namespace FMOD;

class Sounds : public SingletonBase<Sounds>
{
public:
	friend SingletonBase<Sounds>;

	void Update();

	void AddSound(string name, wstring file, bool bLoop = false);
	void Play(string name, float volume = 1.0f);
	void Stop(string name);
	void Pause(string name);
	void Resume(string name);

	bool IsPlaying(string name);
	bool IsPaused(string name);

	void SetVolume(string name, float volume);
	float GetVolume(string name);

private:
	Sounds();
	~Sounds();

private:
	System* system;
	Sound** sound;
	Channel** channel;

	UINT count;

	map<string, Sound**> sounds;
	map<string, Sound**>::iterator iter;

	float volume;

};