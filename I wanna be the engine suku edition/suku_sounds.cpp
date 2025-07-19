#include "suku_sounds.h"
#include "framework.h"
#include <Digitalv.h>

namespace suku
{
	int globalVolume = 1000;
	std::map<MCIDEVICEID, SoundSource*> devicePool_;

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

	Sound::Sound(String _url) : deviceId_(-1), fileType_(), isAvailable_(false),
		isPlaying_(false), isFinished_(false), isPaused_(false)
	{
		deviceId_ = open(_url);
	}

	Sound::~Sound()
	{
		auto iter = devicePool_.find(deviceId_);
		if (iter != devicePool_.end())
		{
			devicePool_.erase(iter);
		}
	}

	MCIDEVICEID Sound::open(String _url)
	{
		if (deviceId_ != -1)
			close();
		this->deviceId_ = -1;
		SendMessage(GameWindow::hWnd, WM_SUKUAUDIOOPEN, (WPARAM)this, (LPARAM)_url.content);
		return this->deviceId_;
	}

	MCIDEVICEID Sound::close()
	{
		//if (deviceId_ == -1)
		//{
		//	WARNINGWINDOW("Trying to close a device with id == -1.");
		//	return -1;
		//}
		//auto iter = devicePool_.find(deviceId_);
		//if (iter == devicePool_.end())
		//{
		//	WARNINGWINDOW("There is no device with given id (== " + std::to_wstring(deviceId_)
		//		+ ") in device pool.");
		//	return -1;
		//}
		//devicePool_.erase(iter);

		PostMessage(GameWindow::hWnd, WM_SUKUAUDIOCLOSE, deviceId_, 0);
		MCIDEVICEID temp = deviceId_;
		deviceId_ = -1;
		return temp;
	}

	void Sound::onWindowMessageCallback(WPARAM _flags, LPARAM _deviceId)
	{
		if (_flags == MCI_NOTIFY_SUCCESSFUL)
		{
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

	void Sound::onWindowMessage(UINT _message, WPARAM _wParam, LPARAM _lParam)
	{

		if (_message == WM_SUKUAUDIOOPEN)
		{
			Sound* sound = reinterpret_cast<Sound*>(_wParam);
			String url(reinterpret_cast<const wchar_t*>(_lParam));
			MCI_OPEN_PARMS openParms = { 0 };
			openParms.lpstrDeviceType = nullptr;
			MCIDEVICEID newDeviceId = openAudio(url);
			sound->deviceId_ = newDeviceId;
			sound->fileType_ = getFileTypeFromURL(url);
			sound->sourceUrl_ = url;
			return;
		}
		WPARAM deviceId = _wParam;
		switch (_message)
		{
		case WM_SUKUAUDIOCLOSE:
			closeDevice(deviceId);
			// deviceId = -1;
			break;
		case WM_SUKUAUDIOPLAY:
			playDevice(deviceId, _lParam);
			break;
		case WM_SUKUAUDIOPAUSE:
			pauseDevice(deviceId);
			break;
		case WM_SUKUAUDIOSTOP:
			stopDevice(deviceId);
			break;
		case WM_SUKUAUDIOSET_VOLUME:
			setDeviceVolume(deviceId, _lParam);
			break;
		case WM_SUKUAUDIOSET_SPEED:
			setDeviceSpeed(deviceId, _lParam);
			break;
		case WM_SUKUAUDIOGET_STATUS:
			break;
		case WM_SUKUAUDIOGET_TIME:
			break;
		case WM_SUKUAUDIOGET_ISAVAILABLE:
			break;
		case WM_SUKUAUDIOGET_ISPLAYING:
			break;
		case WM_SUKUAUDIOGET_ISFINISHED:
			break;
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
		PostMessage(GameWindow::hWnd, WM_SUKUAUDIOPAUSE, deviceId_, 0);
	}

	void Sound::resume(bool _isLoop)
	{
		if (isFinished())
		{
			WARNINGWINDOW("The audio is finished playing. Use Sound::play() instead.");
			return;
		}
		PostMessage(GameWindow::hWnd, WM_SUKUAUDIOPLAY, deviceId_, _isLoop ? 1 : 0);
	}

	void Sound::stop()
	{
		PostMessage(GameWindow::hWnd, WM_SUKUAUDIOSTOP, deviceId_, 0);
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

	void pauseDevice(WPARAM _deviceId)
	{
		MCI_GENERIC_PARMS genericParms = { 0 };

		DWORD dwReturn = mciSendCommand(_deviceId, MCI_PAUSE,
			MCI_NOTIFY | MCI_WAIT, (DWORD_PTR)&genericParms);
		if (dwReturn != 0)
		{
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW_GLOBAL("MCI - Failed to pause audio: " + errorString);
		}
	}

	void stopDevice(WPARAM _deviceId)
	{
		MCI_SEEK_PARMS seekParms = { 0 };

		DWORD dwReturn = mciSendCommand(_deviceId, MCI_SEEK,
			MCI_SEEK_TO_START, (DWORD_PTR)&seekParms);
		if (dwReturn != 0)
		{
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW_GLOBAL("MCI - Failed to stop audio: " + errorString);
		}
	}

	void closeDevice(WPARAM _deviceId)
	{
		DWORD dwReturn = mciSendCommand(_deviceId, MCI_CLOSE, 0, 0);
		if (dwReturn != 0)
		{
			wchar_t errorMessage[256];
			mciGetErrorStringW(dwReturn, errorMessage, sizeof(errorMessage) / 2);
			String errorString(errorMessage);
			ERRORWINDOW_GLOBAL("MCI - Failed to close audio: " + errorString);
		}
	}
}