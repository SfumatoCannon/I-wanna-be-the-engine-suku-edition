#include "pch.h"
#include "suku_sounds.h"
#include "suku_foundation/codec.h"
#include <wrl/client.h>
#include "suku_foundation/message.h"
#include "suku_foundation/file.h"

#include <Digitalv.h>
#include <Shlwapi.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mfobjects.h>
#include <mferror.h>
#include <propvarutil.h>
#include <comdef.h>
#include <xaudio2.h>
#include "audio_core.h"


using Microsoft::WRL::ComPtr;

namespace suku
{
	Sound::Sound(String _url) : sourceUrl_(_url)
	{
		sourceUrl_ = "ProjectAssets\\" + sourceUrl_;
		SoundFactoryGlobal::getInstance().loadSoundData(sourceUrl_, &format_, pcmData_);
	}

	Sound::Sound(String _url, float _basicVolume)
		: Sound(_url)
	{
		basicVolume_ = _basicVolume;
	}

	Sound::~Sound() {
		CoTaskMemFree(format_);
	}

	SoundController* Sound::play() {
		SoundController* pSoundController = new SoundController(this);
		pSoundController->setVolume(1.0f);
		return pSoundController;
	}

	SoundController* Sound::playWithLoop()
	{
		SoundController* pSoundController = new SoundController(this, true);
		pSoundController->setVolume(1.0f);
		return pSoundController;
	}

	SoundController::SoundController(Sound* _sound, bool _isLoop) : sound_(_sound) {
		SoundFactoryGlobal::getInstance().createSourceVoice(&sourceVoice_, sound_->format_);

		XAUDIO2_BUFFER buffer = {};
		buffer.AudioBytes = static_cast<UINT32>(_sound->pcmData_.size());
		buffer.pAudioData = _sound->pcmData_.data();
		if (!_isLoop)
		{
			buffer.Flags = XAUDIO2_END_OF_STREAM;
			buffer.LoopCount = 0;
		}
		else
		{
			buffer.Flags = 0;
			buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
		}
		sourceVoice_->SubmitSourceBuffer(&buffer);
		sourceVoice_->Start();
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
			sourceVoice_->SetVolume(sound_->getBasicVolume() * volume);
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
}