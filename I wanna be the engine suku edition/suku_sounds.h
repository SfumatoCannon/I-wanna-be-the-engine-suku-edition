#pragma once
#include "framework.h"
#include "resource.h"
#include "suku_drawing.h"
#include "game_saving.h"

namespace suku
{
	extern int globalVolume; // range: 0 ~ 1000
	
	class Sound
	{
	private:
		String url_;
		BYTE* pSoundData_;
	public:
		Sound() : url_(), pSoundData_(nullptr) {}
		Sound(String _url);
		void play(bool _isLoop = false);
		static void stop(); // stop all sounds regardless of the instance
	};

	class Music
	{
	private:
		MCIDEVICEID deviceId_;
		DWORD totalTime_;
		String fileType_;
	public:
		Music() : deviceId_(-1), totalTime_(-1), fileType_() {}
		Music(String _url, double _volume = 1.0);
		Music(MCIDEVICEID _deviceId);
		Music(const Music& _other) = delete;

		MCIDEVICEID open(String _url, double _volume = 1.0);
		MCIDEVICEID openInAbsolutePath(String _url, double _volume = 1.0);
		MCIDEVICEID close();

		void setVolume(double _volume);
		void setSpeed(double _speed);
		void play(bool _isLoop = false);
		void pause();
		void resume(bool _isLoop = false);
		void stop();

		const String& getFileType();
		MCIDEVICEID getDeviceId();
		DWORD getLength();
		DWORD getCurrentTime();
		bool isAvailable();
		bool isPlaying();
		bool isFinished();
		bool isPaused();
		DWORD getCurrentStatus();
		//void play(double _startVolume, bool _isLoop = false);
		//void play(double _startVolume, double _startSpeed, bool _isLoop = false);

	};
}