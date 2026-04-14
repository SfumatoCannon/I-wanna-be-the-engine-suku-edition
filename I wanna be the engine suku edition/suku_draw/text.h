#pragma once
#include <suku_foundation/suku_string.h>
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
		BottomLeft,
		BottomCenter,
		BottomRight,
		BottomFill
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
		Text(String _fontName, float _size, 
			TextAlign _textAlign = TextAlign::TopLeft, 
			TextWrapOption _wrapOption = TextWrapOption::Wrap);
		Text(String _content, String _fontName, float _size,
			TextAlign _textAlign = TextAlign::TopLeft,
			TextWrapOption _wrapOption = TextWrapOption::Wrap);
		Text(String _fontName, float _size,
			DWRITE_FONT_WEIGHT _fontWeight,	DWRITE_FONT_STYLE _fontStyle, DWRITE_FONT_STRETCH _fontStretch,
			TextAlign _textAlign = TextAlign::TopLeft,
			TextWrapOption _wrapOption = TextWrapOption::Wrap);
		Text(String _content, String _fontName, float _size,
			DWRITE_FONT_WEIGHT _fontWeight, DWRITE_FONT_STYLE _fontStyle, DWRITE_FONT_STRETCH _fontStretch,
			TextAlign _textAlign = TextAlign::TopLeft,
			TextWrapOption _wrapOption = TextWrapOption::Wrap);


		void setTextAlign(TextAlign _textAlign);
		TextAlign getTextAlign() { return textAlign_; }
		void setTextWrapOption(TextWrapOption _option);
		TextWrapOption getTextWarpOption() { return textWrapOption_; }

		void paint(float _x, float _y);
		void paint(float _x, float _y, const ComPtr<ID2D1Brush>& _brush);
		void paint(float _x, float _y, float _width, float _height);
		void paint(float _x, float _y, float _width, float _height, const ComPtr<ID2D1Brush>& _brush);

		void setBrush(ComPtr<ID2D1Brush> _brush);
	private:
		String fontName_;
		float size_;
		ComPtr<IDWriteTextFormat> pTextFormat_;
		ComPtr<ID2D1Brush> pBrush_;
		TextAlign textAlign_;
		TextWrapOption textWrapOption_;
	};

	class TextArea
	{
	public:
		TextArea();
	private:
		ComPtr<IDWriteTextLayout> pTextLayout_;
		float maxWidth_;
		float maxHeight_;
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