#include "pch.h"
#include "text.h"
#include "draw_core.h"

namespace suku
{
	Text::Text(String _fontName, float _size,
		TextAlign _textAlign, TextWrapOption _wrapOption) 
		: fontName_(_fontName), size_(_size)
	{
		pBrush_ = graphics::createSolidColorBrush({ 0, 0, 0 });
		graphics::TextFactoryGlobal::getDWriteFactory()->CreateTextFormat(
			_fontName.content,
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			_size,
			L"",
			pTextFormat_.GetAddressOf()
		);
		setTextAlign(_textAlign);
		setTextWrapOption(_wrapOption);
	}

	Text::Text(String _content, String _fontName, float _size, TextAlign _textAlign, TextWrapOption _wrapOption)
		:Text(_fontName, _size, _textAlign, _wrapOption)
	{
		textContent = _content;
	}

	Text::Text(String _fontName, float _size, 
		DWRITE_FONT_WEIGHT _fontWeight, DWRITE_FONT_STYLE _fontStyle, DWRITE_FONT_STRETCH _fontStretch, 
		TextAlign _textAlign, TextWrapOption _wrapOption)
	{
		pBrush_ = graphics::createSolidColorBrush({ 0, 0, 0 });
		graphics::TextFactoryGlobal::getDWriteFactory()->CreateTextFormat(
			_fontName.content,
			nullptr,
			_fontWeight,
			_fontStyle,
			_fontStretch,
			_size,
			L"",
			pTextFormat_.GetAddressOf()
		);
		setTextAlign(_textAlign);
		setTextWrapOption(_wrapOption);
	}

	Text::Text(String _content, String _fontName, float _size, 
		DWRITE_FONT_WEIGHT _fontWeight, DWRITE_FONT_STYLE _fontStyle, DWRITE_FONT_STRETCH _fontStretch, 
		TextAlign _textAlign, TextWrapOption _wrapOption)
		:Text(_fontName, _size, _fontWeight, _fontStyle, _fontStretch, _textAlign, _wrapOption)
	{
		textContent = _content;
	}

	void Text::setTextAlign(TextAlign _textAlign)
	{
		if (!pTextFormat_)
			return;
		textAlign_ = _textAlign;
		// Horizonal
		switch (_textAlign)
		{
		case TextAlign::TopLeft: case TextAlign::MiddleLeft: case TextAlign::BottomLeft:
			pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
			break;
		case TextAlign::TopCenter: case TextAlign::MiddleCenter: case TextAlign::BottomCenter:
			pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			break;
		case TextAlign::TopRight: case TextAlign::MiddleRight: case TextAlign::BottomRight:
			pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
			break;
		case TextAlign::TopFill: case TextAlign::MiddleFill: case TextAlign::BottomFill:
			pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
			break;
		default:
			break;
		}
		// Vertical
		switch (_textAlign)
		{
		case TextAlign::TopLeft: case TextAlign::TopCenter: case TextAlign::TopRight: case TextAlign::TopFill:
			pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
			break;
		case TextAlign::MiddleLeft: case TextAlign::MiddleCenter: case TextAlign::MiddleRight: case TextAlign::MiddleFill:
			pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
			break;
		case TextAlign::BottomLeft: case TextAlign::BottomCenter: case TextAlign::BottomRight: case TextAlign::BottomFill:
			pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
			break;
		default:
			break;
		}
	}

	void Text::setTextWrapOption(TextWrapOption _option)
	{
		textWrapOption_ = _option;
		if (_option == TextWrapOption::NoWrap)
			pTextFormat_->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
		else if (_option == TextWrapOption::Wrap)
			pTextFormat_->SetWordWrapping(DWRITE_WORD_WRAPPING_CHARACTER);
		else if (_option == TextWrapOption::WrapWord)
			pTextFormat_->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
	}

	void Text::paint(float _x, float _y)
	{
		if (pBrush_ == nullptr)
		{
			WARNINGWINDOW("Paint brush empty. Program is trying to generate black brush instead.");
			pBrush_ = graphics::createSolidColorBrush({ 0, 0, 0 });
		}
		paint(_x, _y, pBrush_);
	}

	void Text::paint(float _x, float _y, const ComPtr<ID2D1Brush>& _brush)
	{
		if (textAlign_ == TextAlign::TopFill || textAlign_ == TextAlign::MiddleFill || textAlign_ == TextAlign::BottomFill)
		{
			ERRORWINDOW("A fill property was set in the textAlign. You should assign the size of the text box.");
			return;
		}
		D2D1_RECT_F textBoxArea;
		float wideLength = 4096;
		switch (textAlign_)
		{
		case suku::TextAlign::TopLeft:
			textBoxArea = D2D1::RectF(_x, _y, _x + wideLength, _y + wideLength);
			break;
		case suku::TextAlign::TopCenter:
			textBoxArea = D2D1::RectF(_x - wideLength, _y, _x + wideLength, _y + wideLength);
			break;
		case suku::TextAlign::TopRight:
			textBoxArea = D2D1::RectF(_x - wideLength, _y, _x, _y + wideLength);
			break;
		case suku::TextAlign::MiddleLeft:
			textBoxArea = D2D1::RectF(_x, _y - wideLength, _x + wideLength, _y + wideLength);
			break;
		case suku::TextAlign::MiddleCenter:
			textBoxArea = D2D1::RectF(_x - wideLength, _y - wideLength, _x + wideLength, _y + wideLength);
			break;
		case suku::TextAlign::MiddleRight:
			textBoxArea = D2D1::RectF(_x - wideLength, _y - wideLength, _x, _y + wideLength);
			break;
		case suku::TextAlign::BottomLeft:
			textBoxArea = D2D1::RectF(_x, _y - wideLength, _x + wideLength, _y);
			break;
		case suku::TextAlign::BottomCenter:
			textBoxArea = D2D1::RectF(_x - wideLength, _y - wideLength, _x + wideLength, _y);
			break;
		case suku::TextAlign::BottomRight:
			textBoxArea = D2D1::RectF(_x - wideLength, _y - wideLength, _x, _y);
			break;
		default:
			break;
		}
		graphics::pD2DContext->DrawTextW(
			textContent.content,
			wcslen(textContent.content),
			pTextFormat_.Get(),
			textBoxArea,
			_brush.Get()
		);
	}

	void Text::paint(float _x, float _y, float _width, float _height)
	{
		if (pBrush_ == nullptr)
		{
			WARNINGWINDOW("Paint brush empty. Program is trying to generate black brush instead.");
			pBrush_ = graphics::createSolidColorBrush({ 0, 0, 0 });
		}
		paint(_x, _y, _width, _height, pBrush_);
	}

	void Text::paint(float _x, float _y, float _width, float _height, const ComPtr<ID2D1Brush>& _brush)
	{
		graphics::pD2DContext->DrawTextW(
			textContent.content,
			wcslen(textContent.content),
			pTextFormat_.Get(),
			D2D1::RectF(_x, _y, _x + _width, _y + _height),
			_brush.Get()
		);
	}

	void Text::setBrush(ComPtr<ID2D1Brush> _brush)
	{
		pBrush_ = _brush;
	}

	namespace graphics
	{
		TextFactoryGlobal::TextFactoryGlobal()
		{
			DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory),
				reinterpret_cast<IUnknown**>(pDWriteFactory_.GetAddressOf())
			);
		}
	}
}