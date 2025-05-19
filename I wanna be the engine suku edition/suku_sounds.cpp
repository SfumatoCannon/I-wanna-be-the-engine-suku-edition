#include "suku_sounds.h"
#include "framework.h"
#include <Digitalv.h>

namespace suku
{
	int globalVolume = 1000;

	Sound::Sound(String _url, double _volume) : deviceId_(-1), totalTime_(-1), fileType_()
	{
		open(_url, _volume);
	}

	Sound::Sound(MCIDEVICEID _deviceId)
	{
		deviceId_ = _deviceId;
		totalTime_ = -1;
	}

	MCIDEVICEID Sound::open(String _url, double _volume)
	{
		if (deviceId_ != -1)
			close();

		String fileType = getFileTypeFromURL(_url);

		MCI_OPEN_PARMS openParms = { 0 };
		openParms.dwCallback = NULL;
		openParms.lpstrElementName = AbsolutePath(_url.content);

		DWORD dwFlags = MCI_OPEN_TYPE | MCI_OPEN_ELEMENT;
		if (fileType == "wav")
		{
			openParms.lpstrDeviceType = L"waveaudio";
		}
		else
		{
			openParms.lpstrDeviceType = L"MPEGVideo";
			dwFlags |= MCI_OPEN_SHAREABLE;
		}

		DWORD dwReturn = mciSendCommand(0, MCI_OPEN,
			dwFlags, (DWORD_PTR)&openParms);
		if (dwReturn != 0)
		{
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW("MCI - Failed to open sound file: " + errorString);
			deviceId_ = -1;
			totalTime_ = -1;
			fileType_ = "null";
			return -1;
		}
		deviceId_ = openParms.wDeviceID;
		totalTime_ = -1;
		fileType_ = fileType;
		if (fileType_ == "wav")
		{
			if (_volume != 1.0)
				WARNINGWINDOW("Cannot set volume for a WAV file.");
		}
		else
			setVolume(_volume);
		return deviceId_;
	}

	MCIDEVICEID Sound::openInAbsolutePath(String _url, double _volume)
	{
		if (deviceId_ != -1)
			close();

		String fileType = getFileTypeFromURL(_url);

		MCI_OPEN_PARMS openParms = { 0 };
		openParms.dwCallback = NULL;
		openParms.lpstrElementName = _url.content;

		DWORD dwFlags = MCI_OPEN_TYPE | MCI_OPEN_ELEMENT;
		if (fileType == "wav")
		{
			openParms.lpstrDeviceType = L"waveaudio";
		}
		else
		{
			openParms.lpstrDeviceType = L"MPEGVideo";
			dwFlags |= MCI_OPEN_SHAREABLE;
		}

		DWORD dwReturn = mciSendCommand(0, MCI_OPEN,
			dwFlags, (DWORD_PTR)&openParms);
		if (dwReturn != 0)
		{
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW("MCI - Failed to open sound file: " + errorString);
			deviceId_ = -1;
			totalTime_ = -1;
			fileType_ = "null";
			return -1;
		}
		deviceId_ = openParms.wDeviceID;
		totalTime_ = -1;
		fileType_ = fileType;
		if (fileType_ == "wav")
		{
			if (_volume != 1.0)
				WARNINGWINDOW("Cannot set volume for a WAV file.");
		}
		else
			setVolume(_volume);
		return deviceId_;
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

		if (fileType_ == ".wav") // WAVE device
		{
			WARNINGWINDOW("Cannot set volume for a WAV device.");
		}
		else // DGV device
		{
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
	}

	void Sound::setSpeed(double _speed)
	{
		if (fileType_ == ".wav") // WAVE device
		{
			WARNINGWINDOW("Cannot set volume for a WAV device.");
		}
		else
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
				ERRORWINDOW("MCI - Failed to set audio speed: " + errorString);
			}
		}
	}

	void Sound::play(bool _isLoop)
	{
		if (isFinished() || isPaused() || isPlaying())
			stop();
		DWORD dwFlags = MCI_NOTIFY;
		if (_isLoop)
		{
			if (fileType_ == ".wav")
				WARNINGWINDOW("Cannot set loop-playing for a WAV file.")
			else
				dwFlags |= MCI_DGV_PLAY_REPEAT;
		}
		MCI_PLAY_PARMS playParms = { 0 };
		playParms.dwCallback = NULL;
		DWORD dwReturn = mciSendCommand(deviceId_, MCI_PLAY,
			dwFlags, (DWORD_PTR)&playParms);
		if (dwReturn != 0)
		{
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW("MCI - Failed to play audio: " + errorString);
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
			ERRORWINDOW("MCI - Failed to pause audio: " + errorString);
		}
	}

	void Sound::resume(bool _isLoop)
	{
		if (isFinished())
		{
			WARNINGWINDOW("The audio is finished playing. Use Sound::play() instead.");
			return;
		}
		DWORD dwFlags = MCI_NOTIFY;
		if (_isLoop)
		{
			if (fileType_ == ".wav")
				WARNINGWINDOW("Cannot set loop-playing for a WAV file.")
			else
				dwFlags |= MCI_DGV_PLAY_REPEAT;
		}
		MCI_PLAY_PARMS playParms = { 0 };
		playParms.dwCallback = NULL;
		DWORD dwReturn = mciSendCommand(deviceId_, MCI_PLAY,
			dwFlags, (DWORD_PTR)&playParms);
		if (dwReturn != 0)
		{
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW("MCI - Failed to play audio: " + errorString);
		}
	}

	void Sound::stop()
	{
		MCI_SEEK_PARMS seekParms = { 0 };

		DWORD dwReturn = mciSendCommand(deviceId_, MCI_SEEK,
			MCI_NOTIFY | MCI_WAIT | MCI_SEEK_TO_START, (DWORD_PTR)&seekParms);
		if (dwReturn != 0)
		{
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW("MCI - Failed to stop audio: " + errorString);
		}
	}

	const String& Sound::getFileType()
	{
		return fileType_;
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
				ERRORWINDOW("MCI - Failed to get audio length: " + errorString);
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
			ERRORWINDOW("MCI - Failed to get current audio time: " + errorString);
			return -1;
		}
		return statusParms.dwReturn;
	}

	bool Sound::isAvailable()
	{
		MCI_STATUS_PARMS statusParms = { 0 };
		statusParms.dwCallback = NULL;
		statusParms.dwItem = MCI_STATUS_READY;
		DWORD dwReturn = mciSendCommand(deviceId_, MCI_STATUS,
			MCI_WAIT | MCI_STATUS_ITEM, (DWORD_PTR)&statusParms);
		if (dwReturn != 0)
			return false;
		else
			return (statusParms.dwReturn == 1);
	}

	bool Sound::isPlaying()
	{
		if (!isAvailable())
			return false;
		return (getCurrentStatus() == MCI_MODE_PLAY);
	}

	bool Sound::isFinished()
	{
		if (!isAvailable())
			return false;
		return (getCurrentStatus() == MCI_MODE_STOP);
	}

	bool Sound::isPaused()
	{
		if (!isAvailable())
			return false;
		return (getCurrentStatus() == MCI_MODE_PAUSE);
	}

	DWORD Sound::getCurrentStatus()
	{
		MCI_STATUS_PARMS statusParms = { 0 };
		statusParms.dwCallback = NULL;
		statusParms.dwItem = MCI_STATUS_MODE;
		DWORD dwReturn = mciSendCommand(deviceId_, MCI_STATUS,
			MCI_WAIT | MCI_STATUS_ITEM, (DWORD_PTR)&statusParms);
		if (dwReturn != 0)
		{
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW("MCI - Failed to get audio status: " + errorString);
		}
		return statusParms.dwReturn;
	}
}