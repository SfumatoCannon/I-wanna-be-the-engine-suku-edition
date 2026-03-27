#pragma once
#include <suku_foundation/string.h>
#include <dwrite.h>
#include <wrl/client.h>

namespace suku
{
	using Microsoft::WRL::ComPtr;

	enum class TextAlign
	{
		TopLeft,
		TopCenter,
		TopRight,
		TopFill,
		MiddleLeft,
		MiddleCenter,
		MiddleRight,
		MiddleFill,
		DownLeft,
		DownCenter,
		DownRight,
		DownFill
	};

	enum TextWrapOption
	{
		NoWrap,
		Wrap,
		WrapWord
	};

	class Text
	{
	public:
		String textContent;
		Text(String _fontName, float _size);

		void setTextAlign(TextAlign _textAlign);
		TextAlign getTextAlign() { return textAlign_; }
		void setTextWarpOption(TextWrapOption _option);
		TextWrapOption getTextWarpOption() { return textWrapOption_; }

		void paint(float _x, float _y, const ComPtr<ID2D1Brush>& _brush);
		void paint(float _x, float _y, float _width, float _height, const ComPtr<ID2D1Brush>& _brush);
	private:
		String fontName_;
		float size_;
		ComPtr<IDWriteTextFormat> pTextFormat_;
		ComPtr<ID2D1Brush> pBrush_;
		TextAlign textAlign_;
		TextWrapOption textWrapOption_;
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