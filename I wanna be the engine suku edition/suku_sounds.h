#pragma once
#include "framework.h"
#include "resource.h"
#include "suku_drawing.h"
#include "game_saving.h"

namespace suku
{
	class Sound
	{
	private:
		String pszSound_;
	public:
		Sound() = default;
		Sound(String _url) : pszSound_(AbsolutePath(_url.content)) {}
		Sound(const Sound& _other) : pszSound_(_other.pszSound_) {}
		void load(String _url);
		void loadInAbsolutePath(String _url);
		bool play();
	};

	class Music
	{
	private:
		MCIDEVICEID deviceId_;
	public:
		Music() = default;
		Music(String _url);
		Music(const Music& _other) = delete;
		void setVolume(double _volume);
		void setSpeed(double _speed);
		void play(bool _isLoop = false);
		//void play(double _startVolume, bool _isLoop = false);
		//void play(double _startVolume, double _startSpeed, bool _isLoop = false);

	};
}