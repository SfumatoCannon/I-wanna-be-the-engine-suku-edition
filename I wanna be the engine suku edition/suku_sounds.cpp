#include "suku_sounds.h"
#include "framework.h"

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
				throw std::runtime_error("Sound file not found: " + pszSound_.contentInString());
			else
			{
				throw std::runtime_error("Failed to play sound: " + pszSound_.contentInString() + ", Error code: " + std::to_string(errorCode));
			}
		}
		return result;
	}
}