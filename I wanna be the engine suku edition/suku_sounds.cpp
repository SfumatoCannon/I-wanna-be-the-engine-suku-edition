#include "suku_sounds.h"
#include "framework.h"
#include <Digitalv.h>

namespace suku
{
	void Sound::load(String _url)
	{
		pszSound_ = AbsolutePath(_url.content);
	}

	void Sound::loadInAbsolutePath(String _url)
	{
		pszSound_ = _url;
	}

	bool Sound::play()
	{
		bool result = PlaySound(pszSound_.content, NULL, SND_FILENAME | SND_ASYNC);
		if (result == false)
		{
			unsigned short errorCode = GetLastError();
			if (errorCode == ERROR_FILE_NOT_FOUND)
				ERRORWINDOW("Sound file not found: " + pszSound_);
			else
			{
				ERRORWINDOW("Failed to play sound: " + pszSound_
					+ ", Error code: " + std::to_string(errorCode));
			}
		}
		return result;
	}

	Music::Music(String _url)
	{
		MCI_OPEN_PARMS openParms = { 0 };
		openParms.dwCallback = NULL;
		openParms.lpstrDeviceType = L"waveaudio";
		openParms.lpstrElementName = AbsolutePath(_url.content);
		DWORD dwReturn = mciSendCommand(0, MCI_OPEN, 
			MCI_OPEN_ELEMENT | MCI_WAIT | MCI_OPEN_SHAREABLE, (DWORD_PTR)&openParms);
		if (dwReturn != 0)
		{
			wchar_t errorMessage[256];
			mciGetErrorString(dwReturn, errorMessage, sizeof(errorMessage));
			String errorString(errorMessage);
			ERRORWINDOW("MCI - Failed to open sound file: " + errorString);
			deviceId_ = NULL;
			return;
		}
		deviceId_ = openParms.wDeviceID;
	}

	void Music::setVolume(double _volume)
	{
		if (_volume < 0.0) _volume = 0;
		if (_volume > 1.0) _volume = 1.0;

		MCI_DGV_SETAUDIO_PARMS setParms = { 0 };
		setParms.dwCallback = NULL;
		setParms.dwItem = MCI_DGV_SETAUDIO_VOLUME;
		setParms.dwValue = (DWORD)(_volume * 1000);
		DWORD dwReturn = mciSendCommand(deviceId_, MCI_SETAUDIO, 
			MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD_PTR)&setParms);
		if (dwReturn != 0)
		{
			wchar_t errorMessage[256];
			mciGetErrorString(dwReturn, errorMessage, sizeof(errorMessage));
			String errorString(errorMessage);
			ERRORWINDOW("MCI - Failed to set volume: " + errorString);
		}
	}

	void Music::setSpeed(double _speed)
	{
	}

	void Music::play(bool _isLoop)
	{
		DWORD dwFlags = MCI_NOTIFY;
		if (_isLoop)
			dwFlags |= MCI_DGV_PLAY_REPEAT;
		MCI_PLAY_PARMS playParms = { 0 };
		playParms.dwCallback = NULL;
		DWORD dwReturn = mciSendCommand(deviceId_, MCI_PLAY, dwFlags, (DWORD_PTR)&playParms);
		if (dwReturn != 0)
		{
			wchar_t errorMessage[256];
			mciGetErrorString(dwReturn, errorMessage, sizeof(errorMessage));
			String errorString(errorMessage);
			ERRORWINDOW("MCI - Failed to play music: " + errorString);
		}
	}
}