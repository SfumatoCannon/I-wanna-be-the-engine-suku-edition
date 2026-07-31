#pragma once

#include <suku_foundation/suku_string.h>
#include <vector>
#include <xaudio2.h>

namespace suku
{
	class SoundFactoryGlobal
	{
		// 单例模式
	public:
		static SoundFactoryGlobal& getInstance()
		{
			static SoundFactoryGlobal instance;
			return instance;
		}
		static void uninitialize();
		SoundFactoryGlobal(const SoundFactoryGlobal&) = delete;
		SoundFactoryGlobal& operator=(const SoundFactoryGlobal&) = delete;

		void loadSoundData(String _url, WAVEFORMATEX** _pFormat, std::vector<BYTE>& _pcmData);
		void createSourceVoice(IXAudio2SourceVoice** _pSourceVoice, const WAVEFORMATEX* _format);
	private:
		SoundFactoryGlobal();
	};
}