#include "audio_core.h"
#include <Windows.h>
#include <vector>
#include <suku_foundation/message.h>
#include <suku_foundation/file.h>
#include <suku_foundation/codec.h>

#include <Digitalv.h>
#include <Shlwapi.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mfobjects.h>
#include <mferror.h>
#include <propvarutil.h>
#include <comdef.h>
#include <xaudio2.h>
#include <wrl/client.h>

namespace
{
	IXAudio2* g_xaudio2 = nullptr;
	IXAudio2MasteringVoice* g_masterVoice = nullptr;

	void soundInit()
	{
		MFStartup(MF_VERSION);
		HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		if (SUCCEEDED(hr))
		{
			XAudio2Create(&g_xaudio2, 0);
			g_xaudio2->CreateMasteringVoice(&g_masterVoice);
		}
		else
		{
			WARNINGWINDOW_GLOBAL("Failed to initialize COM library for sound system.");
		}
	}

	void soundUninit()
	{
		if (g_masterVoice) {
			g_masterVoice->DestroyVoice();
			g_masterVoice = nullptr;
		}
		if (g_xaudio2) {
			g_xaudio2->Release();
			g_xaudio2 = nullptr;
		}
		MFShutdown();
		CoUninitialize();
	}

	void g_loadSoundData(suku::String _url, WAVEFORMATEX** _pFormat, std::vector<BYTE>& _pcmData)
	{
		using Microsoft::WRL::ComPtr;
		using namespace suku;
		using namespace suku::message;
		File file(_url);
		std::vector<char> soundData;
		ComPtr<IMFSourceReader> reader = nullptr;
		if (!file.isExist())
		{
			// read from resource file
			FileCodec::readResource(soundData, _url);

			IStream* stream = SHCreateMemStream(
				(BYTE*)soundData.data(),
				(UINT)soundData.size()
			);
			ComPtr<IMFByteStream> byteStream = nullptr;
			MFCreateMFByteStreamOnStream(stream, &byteStream);
			MFCreateSourceReaderFromByteStream(
				byteStream.Get(),
				nullptr,
				&reader
			);
			if (!reader)
			{
				ERRORWINDOW_GLOBAL("Failed to create source reader for audio file: " + _url);
				return;
			}
		}
		else
		{
			FileCodec::writeResource(_url);

			String path = filesystem::absolutePath(_url);
			MFCreateSourceReaderFromURL(path.content, nullptr, &reader);
			if (!reader) {
				ERRORWINDOW_GLOBAL("Failed to create source reader for audio file: " + _url);
				return;
			}
		}

		ComPtr<IMFMediaType> mediaTypeOut;
		MFCreateMediaType(&mediaTypeOut);
		if (!mediaTypeOut) {
			ERRORWINDOW_GLOBAL("Failed to create media type for audio file: " + _url);
			return;
		}
		mediaTypeOut->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
		mediaTypeOut->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
		reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, mediaTypeOut.Get());

		ComPtr<IMFMediaType> mediaType;
		reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &mediaType);

		UINT32 size = 0;
		MFCreateWaveFormatExFromMFMediaType(mediaType.Get(), _pFormat, &size);

		// read PCM data
		while (true) {
			ComPtr<IMFSample> sample;
			DWORD flags = 0;
			if (FAILED(reader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &flags, nullptr, &sample))) break;
			if (flags & MF_SOURCE_READERF_ENDOFSTREAM) break;
			if (!sample) continue;

			ComPtr<IMFMediaBuffer> buffer;
			sample->ConvertToContiguousBuffer(&buffer);

			BYTE* data = nullptr;
			DWORD cb = 0;
			buffer->Lock(&data, nullptr, &cb);
			_pcmData.insert(_pcmData.end(), data, data + cb);
			buffer->Unlock();
		}
	}
}

namespace suku
{
	void SoundFactoryGlobal::uninitialize()
	{
		soundUninit();
	}

	void SoundFactoryGlobal::loadSoundData(String _url, WAVEFORMATEX** _pFormat, std::vector<BYTE>& _pcmData)
	{
		g_loadSoundData(_url, _pFormat, _pcmData);
	}

	void SoundFactoryGlobal::createSourceVoice(IXAudio2SourceVoice** _pSourceVoice, const WAVEFORMATEX* _format)
	{
		g_xaudio2->CreateSourceVoice(_pSourceVoice, _format);
	}

	SoundFactoryGlobal::SoundFactoryGlobal()
	{
		soundInit();
	}
}