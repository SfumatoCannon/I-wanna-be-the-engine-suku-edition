#pragma once
#include "framework.h"
#include "resource.h"
#include "suku_drawing.h"
#include "game_saving.h"
#include <string>
#include <vector>
#include <xaudio2.h>

namespace suku
{
    class SoundController;

    class Sound {
    public:
        Sound(String _url);
        ~Sound();

        SoundController* play();

    private:
        WAVEFORMATEX* format_ = nullptr;
        std::vector<BYTE> pcmData_;

        friend class SoundController;
    };

    class SoundController {
    public:
        SoundController(Sound* _sound, bool _isLoop = false);
        ~SoundController();

        void stop();
        void replay();
        void pause();
        void resume();
        void setVolume(float _volume);
        void seek(float _seconds); // 跳转到指定时间点

        void setLooping(bool _isEnable);
        void setLoopRegion(float _startSeconds, float _endSeconds);
        void clearLoopRegion();

    private:
        IXAudio2SourceVoice* sourceVoice_ = nullptr;
        Sound* sound_ = nullptr;
        bool isPaused_ = false;
        float currentVolume_ = 1.0f;

        bool loopEnabled_ = false;
		bool useLoopRegion_ = false;
        float loopStartSec_ = 0.0f;
        float loopEndSec_ = 0.0f;

        void submitBufferFromOffset(UINT32 byteOffset);
        void submitLoopBuffer();
    };

	void soundInit();
    void soundUninit();
}