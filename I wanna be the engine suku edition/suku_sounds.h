#pragma once
#include "framework.h"
#include "resource.h"
#include "suku_drawing.h"
#include "game_saving.h"

namespace suku
{
#define WM_SUKUAUDIOOPEN 0x5001
#define WM_SUKUAUDIOCLOSE 0x5002
#define WM_SUKUAUDIOPLAY 0x5003
#define WM_SUKUAUDIOPAUSE 0x5004
#define WM_SUKUAUDIOSTOP 0x5005
#define WM_SUKUAUDIOSET_VOLUME 0x5006
#define WM_SUKUAUDIOSET_SPEED 0x5007

	extern int globalVolume; // range: 0 ~ 1000

	class Sound;
	class SoundSource;

	class SoundSource
	{
	private:
		size_t totalTime_;
		String sourceUrl_;
		String fileType_;
		bool isAvailable_;
	public:
		SoundSource(String _url);
		size_t totalTime();
		String sourceUrl();
		String fileType();
		bool isAvailable();
		Sound play();
		//void play(Sound& _soundController);
	};

	class Sound
	{
	private:
		MCIDEVICEID deviceId_;
		static std::map<MCIDEVICEID, Sound*> devicePool_;
		String sourceUrl_;
		String fileType_;
		bool isAvailable_;
		bool isPlaying_;
		bool isFinished_;
		bool isPaused_;
	public:
		Sound() : deviceId_(-1), sourceUrl_(), fileType_(), isAvailable_(false) {}
		Sound(String _url, double _volume = 1.0);
		Sound(const Sound& _other) = delete;
		~Sound();

		MCIDEVICEID open(String _url, double _volume = 1.0);
		MCIDEVICEID openInAbsolutePath(String _url, double _volume = 1.0);
		MCIDEVICEID close();

		static void receiveWindowMessage(WPARAM _flags, LPARAM _devID);

		void setVolume(double _volume);
		void setSpeed(double _speed);
		void play(bool _isLoop = false);
		void pause();
		void resume(bool _isLoop = false);
		void stop();

		const String& getFileType();
		MCIDEVICEID getDeviceId();
		size_t getCurrentTime();
		bool isAvailable();
		bool isPlaying();
		bool isFinished();
		bool isPaused();
		DWORD getCurrentStatus();
		//void play(double _startVolume, bool _isLoop = false);
		//void play(double _startVolume, double _startSpeed, bool _isLoop = false);
	};

	// called by main thread
	MCIDEVICEID openAudio(String _url);
	MCIDEVICEID openAudioInAbsolutePath(String _absoluteUrl);
	void setDeviceVolume(WPARAM _deviceId, LPARAM _volume);
	void setDeviceSpeed(WPARAM _deviceId, LPARAM _speed);
	void playDevice(WPARAM _deviceId, LPARAM _isLoop);
}