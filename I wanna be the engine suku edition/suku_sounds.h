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
        SoundController(Sound* sound);
        ~SoundController();

        void stop();
        void replay();
        void pause();
        void resume();
        void setVolume(float volume);
        void seek(float seconds); // ��ת��ָ��ʱ���

    private:
        IXAudio2SourceVoice* sourceVoice_ = nullptr;
        Sound* sound_ = nullptr;
        bool isPaused_ = false;
        float currentVolume_ = 1.0f;

        void submitBufferFromOffset(UINT32 byteOffset);
    };

	void soundInit();
    void soundUninit();
}