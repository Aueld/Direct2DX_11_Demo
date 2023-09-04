#include "Framework.h"
#include "Sound.h"

//unordered_map<string, Sound::Handle*> Sound::storage;
//
//IXAudio2* Sound::XAudio2 = nullptr;
//IXAudio2MasteringVoice* Sound::MasteringVoice = nullptr;
//WAVEFORMATEXTENSIBLE Sound::wfx = { 0 };
//
//HRESULT Sound::FindAudio(HANDLE hAudio, DWORD fourcc, DWORD& dwAudioSize, DWORD& dwAudioDataPosition)
//{
//	HRESULT hr = S_OK;
//	if (SetFilePointer(hAudio, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
//		return HRESULT_FROM_WIN32(GetLastError());
//
//	DWORD dwAudioType;
//	DWORD dwAudioDataSize;
//	DWORD dwRIFFDataSize = 0;
//	DWORD dwFileType;
//	DWORD bytesRead = 0;
//	DWORD dwOffset = 0;
//
//	while (hr == S_OK)
//	{
//		DWORD dwRead;
//
//		if(ReadFile(hAudio, &dwAudioType, sizeof(DWORD), &dwRead, NULL) == FALSE)
//			hr = HRESULT_FROM_WIN32(GetLastError());
//
//		if(ReadFile(hAudio, &dwAudioDataSize, sizeof(DWORD), &dwRead, NULL) == FALSE)
//			hr = HRESULT_FROM_WIN32(GetLastError());
//
//		if (dwAudioType == fourccRIFF)
//		{
//			dwRIFFDataSize = dwAudioDataSize;
//			dwAudioDataSize = 4;
//
//			if(ReadFile(hAudio, &dwFileType, sizeof(DWORD), &dwRead, NULL) == FALSE)
//				hr = HRESULT_FROM_WIN32(GetLastError());
//		}
//		else
//		{
//			if (SetFilePointer(hAudio, dwAudioDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
//				return HRESULT_FROM_WIN32(GetLastError());
//		}
//
//		dwOffset += sizeof(DWORD) * 2;
//
//		if (dwAudioType == fourcc)
//		{
//			dwAudioSize = dwAudioDataSize;
//			dwAudioDataPosition = dwOffset;
//			return S_OK;
//		}
//
//		dwOffset += dwAudioDataSize;
//		
//		if (bytesRead >= dwRIFFDataSize) return S_FALSE;
//	}
//	return S_OK;
//}
//
//HRESULT Sound::ReadAudioData(HANDLE hFile, void* buffer, DWORD bufferSize, DWORD bufferOffset)
//{
//	HRESULT hr = S_OK;
//	DWORD dwRead;
//
//	if (SetFilePointer(hFile, bufferOffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
//		return HRESULT_FROM_WIN32(GetLastError());
//
//	if (ReadFile(hFile, buffer, bufferSize, &dwRead, NULL) == FALSE)
//		hr = HRESULT_FROM_WIN32(GetLastError());
//
//	return hr;
//}
//
//void Sound::Import(string const& file)
//{
//	HANDLE hFile = CreateFileA
//	(
//		file.c_str(),
//		GENERIC_READ,
//		FILE_SHARE_READ,
//		NULL,
//		OPEN_EXISTING,
//		0,
//		NULL
//	);
//
//	if (INVALID_HANDLE_VALUE == hFile) return;
//	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) return;
//
//	DWORD dwAudioSize;
//	DWORD dwAudioPosition;
//	CHECK(FindAudio(hFile, fourccRIFF, dwAudioSize, dwAudioPosition));
//
//	DWORD filetype;
//	CHECK(ReadAudioData(hFile, &filetype, sizeof(DWORD), dwAudioPosition));
//
//	if (filetype != fourccWAVE) return;
//
//	CHECK(FindAudio(hFile, fourccFMT, dwAudioSize, dwAudioPosition));
//	CHECK(ReadAudioData(hFile, &wfx, dwAudioSize, dwAudioPosition));
//
//	CHECK(FindAudio(hFile, fourccDATA, dwAudioSize, dwAudioPosition));
//
//	BYTE* pDataBuffer = new BYTE[dwAudioSize];
//	CHECK(ReadAudioData(hFile, pDataBuffer, dwAudioSize, dwAudioPosition));
//
//	Handle* handle = new Handle();
//	handle->Buffer.AudioBytes = dwAudioSize;
//	handle->Buffer.pAudioData = pDataBuffer;
//	handle->Buffer.Flags = XAUDIO2_END_OF_STREAM;
//
//	CHECK(XAudio2->CreateSourceVoice(&handle->SourceVoice, (WAVEFORMATEX*)&wfx));
//
//	{
//		size_t const x = file.find_first_of('/') + sizeof(char);
//		size_t const y = file.find_first_of('.');
//
//		storage.try_emplace(file.substr(x, y - x), handle);
//	}
//}
//
//Sound::Sound()
//{
//
//}
//
//Sound::~Sound()
//{
//
//}
//
//void Sound::Play()
//{
//	Handle*& sound = storage.at(content);
//
//	if (loop) sound->Buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
//
//	if (bPause)
//	{
//		bPause = false;
//		CHECK(sound->SourceVoice->Start());
//		return;
//	}
//
//	CHECK(sound->SourceVoice->Stop());
//	CHECK(sound->SourceVoice->FlushSourceBuffers());
//	CHECK(sound->SourceVoice->SubmitSourceBuffer(&sound->Buffer));
//	CHECK(sound->SourceVoice->SetVolume(volume));
//	CHECK(sound->SourceVoice->Start());
//}
//
//void Sound::Stop()
//{
//	Handle*& sound = storage.at(content);
//	CHECK(sound->SourceVoice->Stop());
//	CHECK(sound->SourceVoice->FlushSourceBuffers());
//}
//
//void Sound::Pause()
//{
//	if (bPause) return;
//
//	Handle*& sound = storage.at(content);
//	bPause = true;
//	CHECK(sound->SourceVoice->Stop());
//}
//
//void Sound::Procedure(HWND const hWindow, UINT const uMessage, WPARAM const wParam, LPARAM const lParam)
//{
//	switch (uMessage)
//	{
//		case WM_CREATE:
//		{
//			CHECK(CoInitializeEx(nullptr, COINITBASE_MULTITHREADED));
//			CHECK(XAudio2Create(&XAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR));
//			CHECK(XAudio2->CreateMasteringVoice(&MasteringVoice));
//
//			return;
//		}
//		case WM_DESTROY:
//		{
//			for (pair<string, Handle*> elem : storage)
//				delete elem.second;
//
//			XAudio2->Release();
//
//			return;
//		}
//	}
//}

Sounds::Sounds()
	: count(150), volume(1.0f)
{
	System_Create(&system);

	system->init(count, FMOD_INIT_NORMAL, NULL);

	sound = new Sound* [count];
	channel = new Channel* [count];

	ZeroMemory(sound, sizeof(Sound*) * count);
	ZeroMemory(channel, sizeof(Channel*) * count);
}

Sounds::~Sounds()
{
	if (channel != NULL)
	{
		for (UINT i = 0; i < count; i++)
			if (channel[i]) channel[i]->stop();
	}

	if (sound != NULL)
	{
		for (UINT i = 0; i < count; i++)
			if (sound[i]) sound[i]->release();
	}

	SAFE_DELETE_ARRAY(channel);
	SAFE_DELETE_ARRAY(sound);

	if (system != NULL)
	{
		system->release();
		system->close();
	}

	sounds.clear();
}

void Sounds::Update()
{
	system->update();
}

void Sounds::AddSound(string name, wstring filePath, bool bLoop)
{
	if (bLoop == true)
		system->createStream(String::ToString(filePath).c_str(), FMOD_LOOP_NORMAL, NULL, &sound[sounds.size()]);
	else
		system->createStream(String::ToString(filePath).c_str(), FMOD_DEFAULT, NULL, &sound[sounds.size()]);

	sounds[name] = &sound[sounds.size()];
}

void Sounds::Play(string name, float volume)
{
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			system->playSound(*iter->second, NULL, false, &channel[count]);
			channel[count]->setVolume(volume);
		}
	}
}

void Sounds::Stop(string name)
{
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->stop();
			break;
		}
	}
}

void Sounds::Pause(string name)
{
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->setPaused(true);
			break;
		}
	}
}

void Sounds::Resume(string name)
{
	int count = 0;
	iter = sounds.begin();
	for (iter; iter != sounds.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->setPaused(false);
			break;
		}
	}
}

bool Sounds::IsPlaying(string name)
{
	bool bPlay = false;
	int count = 0;
	iter = sounds.begin();

	for (iter; iter != sounds.end(); iter++, count++)
		if (name == iter->first)
		{
			channel[count]->isPlaying(&bPlay);
			break;
		}

	return bPlay;
}

bool Sounds::IsPaused(string name)
{
	bool bPaused = false;
	int count = 0;
	iter = sounds.begin();

	for (iter; iter != sounds.end(); iter++, count++)
	{
		if (name == iter->first)
		{
			channel[count]->getPaused(&bPaused);
			break;
		}
	}

	return bPaused;
}

void Sounds::SetVolume(string name, float volume)
{
	int count = 0;
	iter = sounds.begin();

	for (iter; iter != sounds.end(); iter++, count++)
		if (name == iter->first)
		{
			channel[count]->setVolume(volume);
			break;
		}
}

float Sounds::GetVolume(string name)
{
	float volume = 0.f;
	int count = 0;
	iter = sounds.begin();

	for (iter; iter != sounds.end(); iter++, count++)
		if (name == iter->first)
		{
			channel[count]->getVolume(&volume);
			break;
		}

	return volume;
}