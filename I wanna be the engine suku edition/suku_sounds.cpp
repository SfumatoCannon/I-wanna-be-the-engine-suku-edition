#include "suku_sounds.h"
#include "framework.h"
#include <Digitalv.h>

namespace suku
{
	int globalVolume = 1000;
	std::map<MCIDEVICEID, Sound*> Sound::devicePool_;

	SoundSource::SoundSource(String _url) : sourceUrl_(_url)
	{
		fileType_ = getFileTypeFromURL(_url);

		MCI_OPEN_PARMS openParms = { 0 };
		openParms.dwCallback = NULL;

		openParms.lpstrElementName = AbsolutePath(_url.content);

		DWORD dwFlags = MCI_OPEN_TYPE | MCI_OPEN_ELEMENT;
		if (fileType_ == "wav")
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
			if (openParms.wDeviceID != 0)
				mciSendCommand(openParms.wDeviceID, MCI_CLOSE, 0, 0);
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW("MCI - Failed to open sound file: " + errorString);
			totalTime_ = -1;
			fileType_ = "null";
			isAvailable_ = false;
			return;
		}
		MCIDEVICEID deviceId = openParms.wDeviceID;
		MCI_STATUS_PARMS statusParms = { 0 };
		statusParms.dwItem = MCI_STATUS_LENGTH;
		dwReturn = mciSendCommand(deviceId, MCI_STATUS,
			MCI_STATUS_ITEM | MCI_WAIT, (DWORD_PTR)(&statusParms));
		if (dwReturn != 0)
		{
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			WARNINGWINDOW("MCI - Failed to get sound length: " + errorString);
			totalTime_ = -1;
		}
		else
		{
			totalTime_ = statusParms.dwReturn;
		}
		isAvailable_ = true;

		mciSendCommand(deviceId, MCI_CLOSE, 0, 0);
	}

	size_t SoundSource::totalTime()
	{
		return totalTime_;
	}

	String SoundSource::sourceUrl()
	{
		return sourceUrl_;
	}

	String SoundSource::fileType()
	{
		return fileType_;
	}

	bool SoundSource::isAvailable()
	{
		return isAvailable_;
	}

	Sound SoundSource::play()
	{
		MCI_OPEN_PARMS openParms = { 0 };
		openParms.dwCallback = NULL;
		openParms.lpstrElementName = AbsolutePath(sourceUrl_.content);

		DWORD dwFlags = MCI_OPEN_TYPE | MCI_OPEN_ELEMENT;
		if (fileType_ == "wav")
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
			if (openParms.wDeviceID != 0)
				mciSendCommand(openParms.wDeviceID, MCI_CLOSE, 0, 0);
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW("MCI - Failed to open sound file: " + errorString);
			return Sound();
		}
		return Sound();
	}

	Sound::Sound(String _url, double _volume) : deviceId_(-1), fileType_()
	{
		open(_url, _volume);
	}

	Sound::~Sound()
	{
		auto iter = devicePool_.find(deviceId_);
		if (iter != devicePool_.end())
		{
			devicePool_.erase(iter);
		}
	}

	MCIDEVICEID Sound::open(String _url, double _volume)
	{
		if (deviceId_ != -1)
			close();

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
			if (openParms.wDeviceID != 0)
				mciSendCommand(openParms.wDeviceID, MCI_CLOSE, 0, 0);
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW("MCI - Failed to open sound file: " + errorString);
			deviceId_ = -1;
			fileType_ = "null";
			return -1;
		}
		deviceId_ = openParms.wDeviceID;
		fileType_ = fileType;
		devicePool_.insert({ deviceId_, this });
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
		{
			WARNINGWINDOW("Trying to close a device with id == -1.");
			return -1;
		}
		auto iter = devicePool_.find(deviceId_);
		if (iter == devicePool_.end())
		{
			WARNINGWINDOW("There is no device with given id (== " + std::to_wstring(deviceId_)
				+ ") in device pool.");
			return -1;
		}
		devicePool_.erase(iter);

		DWORD dwReturn = mciSendCommand(deviceId_, MCI_CLOSE, 0, 0);
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

	void Sound::receiveWindowMessage(WPARAM _flags, LPARAM _deviceId)
	{
		if (_flags == MCI_NOTIFY_SUCCESSFUL)
		{
			INFOWINDOW_GLOBAL("breakpoint");
			//try to stop
			DWORD dwReturn = mciSendCommand(_deviceId, MCI_STOP, 0, 0);
			if (dwReturn != 0)
			{
				wchar_t errorMessage[256];
				{
					mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
					String errorString(errorMessage);
					ERRORWINDOW_GLOBAL("MCI - Failed to stop audio: " + errorString);
				}
			}
			MCI_SEEK_PARMS seekParms = { 0 };

			dwReturn = mciSendCommand(_deviceId, MCI_SEEK,
				MCI_SEEK_TO_START, (DWORD_PTR)&seekParms);
			if (dwReturn != 0)
			{
				wchar_t errorMessage[256];
				{
					mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
					String errorString(errorMessage);
					ERRORWINDOW_GLOBAL("MCI - Failed to seek audio: " + errorString);
				}
			}
		}
	}

	void Sound::setVolume(double _volume)
	{
		if (fileType_ == ".wav") // WAVE device
		{
			WARNINGWINDOW_GLOBAL("Cannot set volume for a WAV device.");
		}
		else
		{
			PostMessage(GameWindow::hWnd, WM_SUKUAUDIOSET_VOLUME, deviceId_, (LPARAM)_volume);
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
			PostMessage(GameWindow::hWnd, WM_SUKUAUDIOSET_SPEED, deviceId_, (LPARAM)_speed);
		}
	}

	void Sound::play(bool _isLoop)
	{
		if (_isLoop)
		{
			if (fileType_ == ".wav")
			{
				WARNINGWINDOW("Cannot set loop-playing for a WAV file.");
				PostMessage(GameWindow::hWnd, WM_SUKUAUDIOPLAY, deviceId_, 0);
			}
			else
				PostMessage(GameWindow::hWnd, WM_SUKUAUDIOPLAY, deviceId_, 1);
		}
		else
			PostMessage(GameWindow::hWnd, WM_SUKUAUDIOPLAY, deviceId_, 0);
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
			MCI_SEEK_TO_START, (DWORD_PTR)&seekParms);
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

	size_t Sound::getCurrentTime()
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
		{
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW("MCI - Failed to get current audio status: " + errorString);
			return false;
		}
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
			MCI_STATUS_ITEM, (DWORD_PTR)&statusParms);
		if (dwReturn != 0)
		{
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW("MCI - Failed to get audio status: " + errorString);
			return -1;
		}
		return (DWORD)statusParms.dwReturn;
	}

	MCIDEVICEID openAudio(String _url)
	{
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
			if (openParms.wDeviceID != 0)
				mciSendCommand(openParms.wDeviceID, MCI_CLOSE, 0, 0);
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW_GLOBAL("MCI - Failed to open sound fie: " + errorString);
			return -1;
		}
		return openParms.wDeviceID;
	}

	MCIDEVICEID openAudioInAbsolutePath(String _absoluteUrl)
	{
		String fileType = getFileTypeFromURL(_absoluteUrl);

		MCI_OPEN_PARMS openParms = { 0 };
		openParms.dwCallback = NULL;
		openParms.lpstrElementName = _absoluteUrl.content;

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
			if (openParms.wDeviceID != 0)
				mciSendCommand(openParms.wDeviceID, MCI_CLOSE, 0, 0);
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW_GLOBAL("MCI - Failed to open sound fie: " + errorString);
			return -1;
		}
		return openParms.wDeviceID;
	}

	void setDeviceVolume(WPARAM _deviceId, LPARAM _volume)
	{
		if (_volume < 0.0) _volume = 0;
		if (_volume > 1.0) _volume = 1.0;
		else // DGV device
		{
			MCI_DGV_SETAUDIO_PARMS setParms = { 0 };
			setParms.dwCallback = NULL;
			setParms.dwItem = MCI_DGV_SETAUDIO_VOLUME;
			setParms.dwValue = (DWORD)(_volume * globalVolume);
			DWORD dwReturn = mciSendCommand(_deviceId, MCI_SETAUDIO,
				MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD_PTR)&setParms);
			if (dwReturn != 0)
			{
				wchar_t errorMessage[256];
				mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
				String errorString(errorMessage);
				ERRORWINDOW_GLOBAL("MCI - Failed to set volume: " + errorString);
			}
		}
	}

	void setDeviceSpeed(WPARAM _deviceId, LPARAM _speed)
	{
		MCI_DGV_SET_PARMS setParms = { 0 };
		setParms.dwCallback = NULL;
		setParms.dwSpeed = (DWORD)((double)_speed * 1000);
		DWORD dwReturn = mciSendCommand(_deviceId, MCI_SET,
			MCI_DGV_SET_SPEED, (DWORD_PTR)&setParms);
		if (dwReturn != 0)
		{
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW_GLOBAL("MCI - Failed to set audio speed: " + errorString);
		}
	}

	void playDevice(WPARAM _deviceId, LPARAM _isLoop)
	{
		DWORD dwFlags = MCI_NOTIFY;
		if (_isLoop)
			dwFlags |= MCI_DGV_PLAY_REPEAT;
		MCI_PLAY_PARMS playParms = { 0 };
		playParms.dwCallback = (DWORD_PTR)GameWindow::hWnd;
		DWORD dwReturn = mciSendCommand(_deviceId, MCI_PLAY,
			dwFlags, (DWORD_PTR)&playParms);
		if (dwReturn != 0)
		{
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW_GLOBAL("MCI - Failed to play audio: " + errorString);
		}
	}
}