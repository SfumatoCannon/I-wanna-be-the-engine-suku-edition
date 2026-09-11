#pragma once
#include <suku_foundation/suku_string.h>
#include <dwrite.h>
#include <wrl/client.h>
#include "color.h"
#include "transform.h"

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

	class TextStyle
	{
	public:
		TextStyle(String _fontName, float _size,
			TextAlign _textAlign = TextAlign::TopLeft,
			TextWrapOption _wrapOption = TextWrapOption::Wrap);
		TextStyle(String _fontName, float _size,
			DWRITE_FONT_WEIGHT _fontWeight, DWRITE_FONT_STYLE _fontStyle, DWRITE_FONT_STRETCH _fontStretch,
			TextAlign _textAlign = TextAlign::TopLeft,
			TextWrapOption _wrapOption = TextWrapOption::Wrap);


		void setTextAlign(TextAlign _textAlign);
		TextAlign getTextAlign() { return textAlign_; }
		void setTextWrapOption(TextWrapOption _option);
		TextWrapOption getTextWarpOption() { return textWrapOption_; }

		int getContentLineCount(String _text, float _width);
		float getContentHeight(String _text, float _width);

		void paint(String _text, Transform _transform = Transform());
		void paint(String _text, float _x, float _y, Transform _transform = Transform());
		void paint(String _text, float _x, float _y, const ComPtr<ID2D1Brush>& _brush, Transform _transform = Transform());
		void paint(String _text, float _x, float _y, float _width, float _height, Transform _transform = Transform());
		void paint(String _text, float _x, float _y, float _width, float _height, const ComPtr<ID2D1Brush>& _brush, Transform _transform = Transform());

		void setBrush(ComPtr<ID2D1Brush> _brush);
		void setBrush(Color _color);
	private:
		String fontName_;
		float size_;
		ComPtr<IDWriteTextFormat> pTextFormat_;
		ComPtr<ID2D1Brush> pBrush_;
		TextAlign textAlign_;
		TextWrapOption textWrapOption_;
	};

	class Text
	{
	public:
		Text(const TextStyle& _textStyle);
		Text(String _text, const TextStyle& _textStyle);
		Text(String _text);

		TextStyle style;
		String text;

		void setStyle(const TextStyle& _textStyle) { style = _textStyle; }

		int getContentLineCount(float _width) { return style.getContentLineCount(text, _width); }
		float getContentHeight(float _width) { return style.getContentHeight(text, _width); }

		void paint(float _x, float _y, Transform _transform = Transform());
		void paint(float _x, float _y, float _width, float _height, Transform _transform = Transform());

	private:
	};

	class TextArea
	{
	public:
		TextArea() = default;
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