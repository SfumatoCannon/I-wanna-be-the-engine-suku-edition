#include "suku_sounds.h"
#include "framework.h"
#include <Digitalv.h>

namespace suku
{
	int globalVolume = 1000;

	Sound::Sound(String _url)
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
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW("MCI - Failed to open sound file: " + errorString);
			deviceId_ = -1;
			totalTime_ = -1;
			return;
		}
		deviceId_ = openParms.wDeviceID;
		totalTime_ = -1;
	}

	Sound::Sound(MCIDEVICEID _deviceId)
	{
		deviceId_ = _deviceId;
		totalTime_ = -1;
	}

	MCIDEVICEID Sound::open(String _url)
	{
		if (deviceId_ != -1)
			close();

		MCI_OPEN_PARMS openParms = { 0 };
		openParms.dwCallback = NULL;
		openParms.lpstrDeviceType = L"waveaudio";
		openParms.lpstrElementName = AbsolutePath(_url.content);
		DWORD dwReturn = mciSendCommand(0, MCI_OPEN,
			MCI_OPEN_ELEMENT | MCI_WAIT | MCI_OPEN_SHAREABLE, (DWORD_PTR)&openParms);
		if (dwReturn != 0)
		{
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW("MCI - Failed to open sound file: " + errorString);
			deviceId_ = -1;
			totalTime_ = -1;
			return -1;
		}
		setVolume(1.0);
		deviceId_ = openParms.wDeviceID;
		totalTime_ = -1;
		return deviceId_;
	}

	void Sound::openInAbsolutePath(String _url)
	{
		if (deviceId_ != -1)
			close();

		MCI_OPEN_PARMS openParms = { 0 };
		openParms.dwCallback = NULL;
		openParms.lpstrDeviceType = L"waveaudio";
		openParms.lpstrElementName = AbsolutePath(_url.content);
		DWORD dwReturn = mciSendCommand(0, MCI_OPEN,
			MCI_OPEN_ELEMENT | MCI_WAIT | MCI_OPEN_SHAREABLE, (DWORD_PTR)&openParms);
		if (dwReturn != 0)
		{
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW("MCI - Failed to open sound file: " + errorString);
			deviceId_ = -1;
			totalTime_ = -1;
			return;
		}
		setVolume(1.0);
		deviceId_ = openParms.wDeviceID;
		totalTime_ = -1;
	}

	MCIDEVICEID Sound::close()
	{
		if (deviceId_ == -1)
			return -1;
		MCI_GENERIC_PARMS closeParms = { 0 };
		DWORD dwReturn = mciSendCommand(deviceId_, MCI_CLOSE,
			MCI_NOTIFY | MCI_WAIT, (DWORD_PTR)&closeParms);
		if (dwReturn != 0)
		{
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW("MCI - Failed to close sound file: " + errorString);
		}
		MCIDEVICEID temp = deviceId_;
		deviceId_ = -1;
		return temp;
	}

	void Sound::setVolume(double _volume)
	{
		if (_volume < 0.0) _volume = 0;
		if (_volume > 1.0) _volume = 1.0;

		MCI_DGV_SETAUDIO_PARMS setParms = { 0 };
		setParms.dwCallback = NULL;
		setParms.dwItem = MCI_DGV_SETAUDIO_VOLUME;
		setParms.dwValue = (DWORD)(_volume * globalVolume);
		DWORD dwReturn = mciSendCommand(deviceId_, MCI_SETAUDIO, 
			MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD_PTR)&setParms);
		if (dwReturn != 0)
		{
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW("MCI - Failed to set volume: " + errorString);
		}
	}

	void Sound::setSpeed(double _speed)
	{
		MCI_DGV_SET_PARMS setParms = { 0 };
		setParms.dwCallback = NULL;
		setParms.dwSpeed = (DWORD)(_speed * 1000);
		DWORD dwReturn = mciSendCommand(deviceId_, MCI_SET,
			MCI_DGV_SET_SPEED, (DWORD_PTR)&setParms);
		if (dwReturn != 0)
		{
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW("MCI - Failed to set music speed: " + errorString);
		}
	}

	void Sound::play(bool _isLoop)
	{
		DWORD dwFlags = MCI_NOTIFY;
		if (_isLoop)
			dwFlags |= MCI_DGV_PLAY_REPEAT;
		MCI_PLAY_PARMS playParms = { 0 };
		playParms.dwCallback = NULL;
		DWORD dwReturn = mciSendCommand(deviceId_, MCI_PLAY, 
			dwFlags, (DWORD_PTR)&playParms);
		if (dwReturn != 0)
		{
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW("MCI - Failed to play music: " + errorString);
		}
	}

	void Sound::pause()
	{
		MCI_GENERIC_PARMS genericParms = { 0 };

		DWORD dwReturn = mciSendCommand(deviceId_, MCI_PAUSE, 
			MCI_NOTIFY | MCI_WAIT, (DWORD_PTR)&genericParms);	
		if (dwReturn != 0)
		{
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW("MCI - Failed to pause music: " + errorString);
		}
	}

	void Sound::stop()
	{
		MCI_SEEK_PARMS seekParms = { 0 };

		DWORD dwReturn = mciSendCommand(deviceId_, MCI_PAUSE, 
			MCI_NOTIFY | MCI_WAIT | MCI_SEEK_TO_START, (DWORD_PTR)&seekParms);
		if (dwReturn != 0)
		{
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW("MCI - Failed to stop music: " + errorString);
		}
	}

	MCIDEVICEID Sound::getDeviceId()
	{
		return deviceId_;
	}

	DWORD Sound::getLength()
	{
		if (totalTime_ == -1)
		{
			MCI_STATUS_PARMS statusParms = { 0 };
			statusParms.dwCallback = NULL;
			statusParms.dwItem = MCI_STATUS_LENGTH;
			DWORD dwReturn = mciSendCommand(deviceId_, MCI_STATUS, 
				MCI_WAIT | MCI_STATUS_ITEM, (DWORD_PTR)&statusParms);
			if (dwReturn != 0)
			{
				wchar_t errorMessage[256];
				mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
				String errorString(errorMessage);
				ERRORWINDOW("MCI - Failed to get music length: " + errorString);
			}
			else
			{
				totalTime_ = statusParms.dwReturn;
			}
		}
		return totalTime_;
	}

	DWORD Sound::getCurrentTime()
	{
		MCI_STATUS_PARMS statusParms = { 0 };
		statusParms.dwCallback = NULL;
		statusParms.dwItem = MCI_STATUS_POSITION;
		DWORD dwReturn = mciSendCommand(deviceId_, MCI_STATUS, 
			MCI_WAIT | MCI_STATUS_ITEM, (DWORD_PTR)&statusParms);
		if (dwReturn != 0)
		{
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW("MCI - Failed to get music length: " + errorString);
			return -1;
		}
		return statusParms.dwReturn;
	}
}