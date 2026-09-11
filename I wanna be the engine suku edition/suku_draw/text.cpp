#include "pch.h"
#include "text.h"
#include "draw_core.h"

namespace suku
{
	TextStyle::TextStyle(String _fontName, float _size, TextAlign _textAlign, TextWrapOption _wrapOption)
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

	TextStyle::TextStyle(String _fontName, float _size, DWRITE_FONT_WEIGHT _fontWeight, DWRITE_FONT_STYLE _fontStyle, DWRITE_FONT_STRETCH _fontStretch, TextAlign _textAlign, TextWrapOption _wrapOption)
		: fontName_(_fontName), size_(_size)
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

	void TextStyle::setTextAlign(TextAlign _textAlign)
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

	void TextStyle::setTextWrapOption(TextWrapOption _option)
	{
		textWrapOption_ = _option;
		if (_option == TextWrapOption::NoWrap)
			pTextFormat_->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
		else if (_option == TextWrapOption::Wrap)
			pTextFormat_->SetWordWrapping(DWRITE_WORD_WRAPPING_CHARACTER);
		else if (_option == TextWrapOption::WrapWord)
			pTextFormat_->SetWordWrapping(DWRITE_WORD_WRAPPING_WRAP);
	}

	int TextStyle::getContentLineCount(String _text, float _width)
	{
		IDWriteTextLayout* pTextLayout = nullptr;
		graphics::TextFactoryGlobal::getDWriteFactory()->CreateTextLayout(
			_text.content,
			static_cast<UINT32>(wcslen(_text.content)),
			pTextFormat_.Get(),
			_width,
			FLT_MAX,
			&pTextLayout
		);
		DWRITE_TEXT_METRICS textMetrics;
		pTextLayout->GetMetrics(&textMetrics);
		return textMetrics.lineCount;
	}

	float TextStyle::getContentHeight(String _text, float _width)
	{
		IDWriteTextLayout* pTextLayout = nullptr;
		graphics::TextFactoryGlobal::getDWriteFactory()->CreateTextLayout(
			_text.content,
			static_cast<UINT32>(wcslen(_text.content)),
			pTextFormat_.Get(),
			_width,
			FLT_MAX,
			&pTextLayout
		);
		DWRITE_TEXT_METRICS textMetrics;
		pTextLayout->GetMetrics(&textMetrics);
		return textMetrics.height;
	}

	void TextStyle::paint(String _text, Transform _transform)
	{
		paint(_text, 0, 0, _transform);
	}

	void TextStyle::paint(String _text, float _x, float _y, Transform _transform)
	{
		if (pBrush_ == nullptr)
		{
			pBrush_ = graphics::createSolidColorBrush({ 0, 0, 0 });
		}
		paint(_text, _x, _y, pBrush_, _transform);
	}

	void TextStyle::paint(String _text, float _x, float _y, const ComPtr<ID2D1Brush>& _brush, Transform _transform)
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
		graphics::setPaintingTransform(_transform);
		graphics::pD2DContext->DrawTextW(
			_text.content,
			static_cast<UINT32>(wcslen(_text.content)),
			pTextFormat_.Get(),
			textBoxArea,
			_brush.Get()
		);
	}

	void TextStyle::paint(String _text, float _x, float _y, float _width, float _height, Transform _transform)
	{
		if (pBrush_ == nullptr)
		{
			pBrush_ = graphics::createSolidColorBrush({ 0, 0, 0 });
		}
		paint(_text, _x, _y, _width, _height, pBrush_, _transform);
	}

	void TextStyle::paint(String _text, float _x, float _y, float _width, float _height, const ComPtr<ID2D1Brush>& _brush, Transform _transform)
	{
		graphics::setPaintingTransform(_transform);
		graphics::pD2DContext->DrawTextW(
			_text.content,
			static_cast<UINT32>(wcslen(_text.content)),
			pTextFormat_.Get(),
			D2D1::RectF(_x, _y, _x + _width, _y + _height),
			_brush.Get()
		);
	}

	void TextStyle::setBrush(ComPtr<ID2D1Brush> _brush)
	{
		pBrush_ = _brush; 
	}

	void TextStyle::setBrush(Color _color)
	{
		setBrush(graphics::createSolidColorBrush(_color));
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
	
	Text::Text(const TextStyle& _textStyle) : style(_textStyle)
	{}

	Text::Text(String _text, const TextStyle& _textStyle) : text(_text), style(_textStyle)
	{}

	Text::Text(String _text) : text(_text), style(TextStyle(L"Arial", 16.0f))
	{}

	void Text::paint(float _x, float _y, Transform _transform)
	{
		style.paint(text, _x, _y, _transform);
	}
	
	void Text::paint(float _x, float _y, float _width, float _height, Transform _transform)
	{
		style.paint(text, _x, _y, _width, _height, _transform);
	}
}