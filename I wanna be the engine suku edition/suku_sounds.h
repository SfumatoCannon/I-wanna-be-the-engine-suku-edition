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
		MCIDEVICEID deviceId_;
		DWORD totalTime_;
	public:
		Sound() : deviceId_(-1), totalTime_(-1) {}
		Sound(String _url);
		Sound(MCIDEVICEID _deviceId);
		Sound(const Sound& _other) = delete;

		MCIDEVICEID open(String _url);
		void openInAbsolutePath(String _url);
		MCIDEVICEID close();

		void setVolume(double _volume);
		void setSpeed(double _speed);
		void play(bool _isLoop = false);
		void pause();
		void stop();

		MCIDEVICEID getDeviceId();
		DWORD getLength();
		DWORD getCurrentTime();
		//void play(double _startVolume, bool _isLoop = false);
		//void play(double _startVolume, double _startSpeed, bool _isLoop = false);

	};
}