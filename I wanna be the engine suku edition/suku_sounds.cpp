#include "suku_sounds.h"
#include "framework.h"
#include "suku_foundation/includes.h"

#include <Digitalv.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mfobjects.h>
#include <mferror.h>
#include <wrl/client.h>
#include <propvarutil.h>
#include <comdef.h>
#include <xaudio2.h>


using Microsoft::WRL::ComPtr;

namespace suku
{
	IXAudio2* g_xaudio2 = nullptr;
	IXAudio2MasteringVoice* g_masterVoice = nullptr;

	Sound::Sound(String _url) {
		std::wstring wFile(AbsolutePath(_url.content));

		ComPtr<IMFSourceReader> reader;
		MFCreateSourceReaderFromURL(wFile.c_str(), nullptr, &reader);
		if (!reader) {
			ERRORWINDOW("Failed to create source reader for audio file: " + _url);
			return;
		}
		
		ComPtr<IMFMediaType> mediaTypeOut;
		MFCreateMediaType(&mediaTypeOut);
		if (!mediaTypeOut) {
			ERRORWINDOW("Failed to create media type for audio file: " + _url);
			return;
		}
		mediaTypeOut->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
		mediaTypeOut->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
		reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, mediaTypeOut.Get());

		ComPtr<IMFMediaType> mediaType;
		reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &mediaType);

		UINT32 size = 0;
		MFCreateWaveFormatExFromMFMediaType(mediaType.Get(), &format_, &size);

		// read PCM data
		while (true) {
			ComPtr<IMFSample> sample;
			DWORD flags = 0;
			if (FAILED(reader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &flags, nullptr, &sample))) break;
			if (flags & MF_SOURCE_READERF_ENDOFSTREAM) break;
			if (!sample) continue;

			ComPtr<IMFMediaBuffer> buffer;
			sample->ConvertToContiguousBuffer(&buffer);

			BYTE* data = nullptr;
			DWORD cb = 0;
			buffer->Lock(&data, nullptr, &cb);
			pcmData_.insert(pcmData_.end(), data, data + cb);
			buffer->Unlock();
		}
	}

	Sound::~Sound() {
		CoTaskMemFree(format_);
	}

	SoundController* Sound::play() {
		return new SoundController(this);
	}

	extern IXAudio2* g_xaudio2;

	SoundController::SoundController(Sound* _sound, bool _isLoop) : sound_(_sound) {
		g_xaudio2->CreateSourceVoice(&sourceVoice_, _sound->format_);

		if (!_isLoop)
		{
			XAUDIO2_BUFFER buffer = {};
			buffer.AudioBytes = static_cast<UINT32>(_sound->pcmData_.size());
			buffer.pAudioData = _sound->pcmData_.data();
			buffer.Flags = XAUDIO2_END_OF_STREAM;

			sourceVoice_->SubmitSourceBuffer(&buffer);
			sourceVoice_->Start();
		}
		else
		{

		}
	}

	SoundController::~SoundController() {
		if (sourceVoice_) {
			sourceVoice_->Stop();
			sourceVoice_->DestroyVoice();
		}
	}

	void SoundController::stop() {
		if (sourceVoice_) {
			if (sourceVoice_) {
				sourceVoice_->Stop();
				sourceVoice_->FlushSourceBuffers();
				submitBufferFromOffset(0);
				isPaused_ = false;
			}
		}
		else
		{
			ERRORWINDOW("Invalid source voice");
		}
	}

	void SoundController::replay()
	{
		if (sourceVoice_)
		{
			stop();
			sourceVoice_->Start();
			isPaused_ = false;
		}
		else
		{
			ERRORWINDOW("Invalid source voice");
		}
	}

	void SoundController::pause() {
		if (sourceVoice_ && !isPaused_) {
			sourceVoice_->Stop();
			isPaused_ = true;
		}
		else
		{
			if (!sourceVoice_)
			{
				ERRORWINDOW("Invalid source voice");
			}
			else
			{
				WARNINGWINDOW("Sound is already paused");
			}
		}
	}

	void SoundController::resume() {
		if (sourceVoice_ && isPaused_) {
			sourceVoice_->Start();
			isPaused_ = false;
		}
	}

	void SoundController::setVolume(float volume) {
		if (sourceVoice_) {
			currentVolume_ = volume;
			sourceVoice_->SetVolume(volume);
		}
	}

	void SoundController::seek(float seconds) {
		if (!sourceVoice_) return;

		sourceVoice_->Stop();
		sourceVoice_->FlushSourceBuffers();

		// 计算跳转位置的字节偏移
		WAVEFORMATEX* fmt = sound_->format_;
		UINT32 byteRate = fmt->nAvgBytesPerSec; // 每秒字节数
		UINT32 byteOffset = static_cast<UINT32>(seconds * byteRate);

		if (byteOffset > sound_->pcmData_.size()) byteOffset = 0;

		submitBufferFromOffset(byteOffset);
		sourceVoice_->Start();
	}

	void SoundController::setLooping(bool _isEnable)
	{
		if (loopEnabled_ == _isEnable)
		{
			WARNINGWINDOW("Looping is already set to " + std::to_string(_isEnable));
			return;
		}
		loopEnabled_ = _isEnable;

		sourceVoice_->Stop();
		sourceVoice_->FlushSourceBuffers();

		if (loopEnabled_)
			submitLoopBuffer();
		else
			submitBufferFromOffset(0);

		if (!isPaused_)
			sourceVoice_->Start();
	}

	void SoundController::setLoopRegion(float _startSeconds, float _endSeconds)
	{
		useLoopRegion_ = true;
		loopStartSec_ = _startSeconds;
		loopEndSec_ = _endSeconds;

		if (!isPaused_)
			sourceVoice_->Stop();

		sourceVoice_->FlushSourceBuffers();
		submitLoopBuffer();

		if (!isPaused_)
			sourceVoice_->Start();
	}

	void SoundController::clearLoopRegion()
	{
		useLoopRegion_ = false;
		if (!isPaused_)
			sourceVoice_->Stop();
		sourceVoice_->FlushSourceBuffers();

		if (loopEnabled_)
			submitLoopBuffer();
		else
			submitBufferFromOffset(0);

		if (!isPaused_)
			sourceVoice_->Start();
	}

	void SoundController::submitBufferFromOffset(UINT32 byteOffset) {
		XAUDIO2_BUFFER buffer = {};
		buffer.AudioBytes = static_cast<UINT32>(sound_->pcmData_.size() - byteOffset);
		buffer.pAudioData = sound_->pcmData_.data() + byteOffset;
		buffer.Flags = XAUDIO2_END_OF_STREAM;

		sourceVoice_->SubmitSourceBuffer(&buffer);
	}

	void SoundController::submitLoopBuffer()
	{
		XAUDIO2_BUFFER buffer = {};
		buffer.pAudioData = sound_->pcmData_.data();
		buffer.AudioBytes = static_cast<UINT32>(sound_->pcmData_.size());
		buffer.Flags = 0;

		if (useLoopRegion_) {
			WAVEFORMATEX* fmt = sound_->format_;
			DWORD sampleRate = fmt->nSamplesPerSec;

			DWORD loopStartSample = static_cast<DWORD>(loopStartSec_ * sampleRate);
			DWORD loopEndSample = static_cast<DWORD>(loopEndSec_ * sampleRate);
			DWORD loopLengthSample = loopEndSample - loopStartSample;

			buffer.LoopBegin = loopStartSample;
			buffer.LoopLength = loopLengthSample;
		}
		else {
			buffer.LoopBegin = 0;
			buffer.LoopLength = 0; // 整曲
		}

		buffer.LoopCount = XAUDIO2_LOOP_INFINITE;

		sourceVoice_->SubmitSourceBuffer(&buffer);
	}

	void soundInit() {
		MFStartup(MF_VERSION);
		CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		XAudio2Create(&g_xaudio2, 0);
		g_xaudio2->CreateMasteringVoice(&g_masterVoice);
	}

	void soundUninit()
	{
		if (g_masterVoice) {
			g_masterVoice->DestroyVoice();
			g_masterVoice = nullptr;
		}
		if (g_xaudio2) {
			g_xaudio2->Release();
			g_xaudio2 = nullptr;
		}
		MFShutdown();
		CoUninitialize();
	}
}