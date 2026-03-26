#pragma once
#include <suku_foundation/string.h>

namespace suku
{
	using Microsoft::WRL::ComPtr;

	class Text
	{
	public:
		String content;
		Text(String _fontName, float _size);
	private:
		ComPtr<IDWriteTextFormat> pTextFormat_;
	};

	namespace graphics
	{
		class TextFactoryGlobal
		{
		public:
			static IDWriteFactory* getDWriteFactory()
			{
				static TextFactoryGlobal instance;
				return instance.pDWriteFactory_.Get();
			}
			TextFactoryGlobal(const TextFactoryGlobal&) = delete;
			TextFactoryGlobal& operator=(const TextFactoryGlobal&) = delete;
		private:
			TextFactoryGlobal();
			ComPtr<IDWriteFactory> pDWriteFactory_ = nullptr;
		};
	}
}