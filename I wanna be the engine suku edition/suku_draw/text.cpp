#include "pch.h"
#include "text.h"
#include "draw_core.h"
#include "color.h"
#include <suku_foundation/file.h>
#include <dwrite_3.h>

namespace suku
{
	TextStyle::TextStyle(String _fontName, float _size, 
		TextStyle::Align _textAlign, TextStyle::WrapOption _wrapOption)
		: fontName_(_fontName), size_(_size), brush_(Color::Black())
	{
		pTextFormat_ = graphics::TextFactoryGlobal::createTextFormat(_fontName, _size);
		setTextAlign(_textAlign);
		setTextWrapOption(_wrapOption);
	}

	TextStyle::TextStyle(String _fontName, float _size, 
		TextStyle::Weight _fontWeight, TextStyle::ItalicType _fontStyle, TextStyle::Stretch _fontStretch, TextStyle::Align _textAlign, TextStyle::WrapOption _wrapOption)
		: fontName_(_fontName), size_(_size), brush_(Color::Black())
	{
		pTextFormat_ = graphics::TextFactoryGlobal::createTextFormat(_fontName, _size,
			static_cast<DWRITE_FONT_WEIGHT>(_fontWeight),
			static_cast<DWRITE_FONT_STYLE>(_fontStyle),
			static_cast<DWRITE_FONT_STRETCH>(_fontStretch));
		setTextAlign(_textAlign);
		setTextWrapOption(_wrapOption);
	}

	TextStyle::TextStyle(String _fontName, String _localUrl, float _size, 
		TextStyle::Align _textAlign, TextStyle::WrapOption _wrapOption)
		: fontName_(_fontName), size_(_size), brush_(Color::Black())
	{
		pTextFormat_ = graphics::TextFactoryGlobal::createTextFormat(_fontName, _localUrl, _size);
		setTextAlign(_textAlign);
		setTextWrapOption(_wrapOption);
	}

	TextStyle::TextStyle(String _fontName, String _localUrl, float _size, 
		TextStyle::Weight _fontWeight, TextStyle::ItalicType _fontStyle, TextStyle::Stretch _fontStretch, TextStyle::Align _textAlign, TextStyle::WrapOption _wrapOption)
		: fontName_(_fontName), size_(_size), brush_(Color::Black())
	{
		pTextFormat_ = graphics::TextFactoryGlobal::createTextFormat(_fontName, _localUrl, _size,
			static_cast<DWRITE_FONT_WEIGHT>(_fontWeight),
			static_cast<DWRITE_FONT_STYLE>(_fontStyle),
			static_cast<DWRITE_FONT_STRETCH>(_fontStretch));
		setTextAlign(_textAlign);
		setTextWrapOption(_wrapOption);
	}

	void TextStyle::setTextAlign(TextStyle::Align _textAlign)
	{
		if (!pTextFormat_)
			return;
		textAlign_ = _textAlign;
		// Horizonal
		switch (_textAlign)
		{
		case TextStyle::Align::TopLeft: case TextStyle::Align::MiddleLeft: case TextStyle::Align::BottomLeft:
			pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
			break;
		case TextStyle::Align::TopCenter: case TextStyle::Align::MiddleCenter: case TextStyle::Align::BottomCenter:
			pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
			break;
		case TextStyle::Align::TopRight: case TextStyle::Align::MiddleRight: case TextStyle::Align::BottomRight:
			pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
			break;
		case TextStyle::Align::TopFill: case TextStyle::Align::MiddleFill: case TextStyle::Align::BottomFill:
			pTextFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
			break;
		default:
			break;
		}
		// Vertical
		switch (_textAlign)
		{
		case TextStyle::Align::TopLeft: case TextStyle::Align::TopCenter: case TextStyle::Align::TopRight: case TextStyle::Align::TopFill:
			pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
			break;
		case TextStyle::Align::MiddleLeft: case TextStyle::Align::MiddleCenter: case TextStyle::Align::MiddleRight: case TextStyle::Align::MiddleFill:
			pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
			break;
		case TextStyle::Align::BottomLeft: case TextStyle::Align::BottomCenter: case TextStyle::Align::BottomRight: case TextStyle::Align::BottomFill:
			pTextFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
			break;
		default:
			break;
		}
	}

	void TextStyle::setTextWrapOption(TextStyle::WrapOption _option)
	{
		textWrapOption_ = _option;
		if (_option == TextStyle::WrapOption::NoWrap)
			pTextFormat_->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
		else if (_option == TextStyle::WrapOption::Wrap)
			pTextFormat_->SetWordWrapping(DWRITE_WORD_WRAPPING_CHARACTER);
		else if (_option == TextStyle::WrapOption::WrapWord)
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
		paint(_text, _x, _y, brush_, _transform);
	}

	void TextStyle::paint(String _text, float _x, float _y, const Brush& _brush, Transform _transform)
	{
		if (textAlign_ == TextStyle::Align::TopFill || textAlign_ == TextStyle::Align::MiddleFill || textAlign_ == TextStyle::Align::BottomFill)
		{
			ERRORWINDOW("A fill property was set in the textAlign. You should assign the size of the text box.");
			return;
		}
		D2D1_RECT_F textBoxArea;
		float wideLength = 4096;
		switch (textAlign_)
		{
		case suku::TextStyle::Align::TopLeft:
			textBoxArea = D2D1::RectF(_x, _y, _x + wideLength, _y + wideLength);
			break;
		case suku::TextStyle::Align::TopCenter:
			textBoxArea = D2D1::RectF(_x - wideLength, _y, _x + wideLength, _y + wideLength);
			break;
		case suku::TextStyle::Align::TopRight:
			textBoxArea = D2D1::RectF(_x - wideLength, _y, _x, _y + wideLength);
			break;
		case suku::TextStyle::Align::MiddleLeft:
			textBoxArea = D2D1::RectF(_x, _y - wideLength, _x + wideLength, _y + wideLength);
			break;
		case suku::TextStyle::Align::MiddleCenter:
			textBoxArea = D2D1::RectF(_x - wideLength, _y - wideLength, _x + wideLength, _y + wideLength);
			break;
		case suku::TextStyle::Align::MiddleRight:
			textBoxArea = D2D1::RectF(_x - wideLength, _y - wideLength, _x, _y + wideLength);
			break;
		case suku::TextStyle::Align::BottomLeft:
			textBoxArea = D2D1::RectF(_x, _y - wideLength, _x + wideLength, _y);
			break;
		case suku::TextStyle::Align::BottomCenter:
			textBoxArea = D2D1::RectF(_x - wideLength, _y - wideLength, _x + wideLength, _y);
			break;
		case suku::TextStyle::Align::BottomRight:
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
			_brush.getD2DBrush().Get()
		);
	}

	void TextStyle::paint(String _text, float _x, float _y, float _width, float _height, Transform _transform)
	{
		paint(_text, _x, _y, _width, _height, brush_, _transform);
	}

	void TextStyle::paint(String _text, float _x, float _y, float _width, float _height, const Brush& _brush, Transform _transform)
	{
		graphics::setPaintingTransform(_transform);
		graphics::pD2DContext->DrawTextW(
			_text.content,
			static_cast<UINT32>(wcslen(_text.content)),
			pTextFormat_.Get(),
			D2D1::RectF(_x, _y, _x + _width, _y + _height),
			_brush.getD2DBrush().Get()
		);
	}

	void TextStyle::setBrush(const Brush& _brush)
	{
		brush_ = _brush;
	}

	namespace graphics
	{
		ComPtr<IDWriteTextFormat> TextFactoryGlobal::createTextFormat(const String& _fontName, float _size, DWRITE_FONT_WEIGHT _fontWeight, DWRITE_FONT_STYLE _fontStyle, DWRITE_FONT_STRETCH _fontStretch)
		{
			ComPtr<IDWriteTextFormat> pTextFormat = nullptr;
			HRESULT hr = getDWriteFactory()->CreateTextFormat(
				_fontName.content,
				nullptr,
				_fontWeight,
				_fontStyle,
				_fontStretch,
				_size,
				L"",
				pTextFormat.GetAddressOf()
			);
			return pTextFormat;
		}

		ComPtr<IDWriteTextFormat> TextFactoryGlobal::createTextFormat(const String& _fontName, const String& _localUrl, float _size, DWRITE_FONT_WEIGHT _fontWeight, DWRITE_FONT_STYLE _fontStyle, DWRITE_FONT_STRETCH _fontStretch)
		{
			String url = filesystem::absolutePath(_localUrl);
			if (localFontCollectionMap_.find(_fontName) == localFontCollectionMap_.end())
			{
				if (!addLocalFontCollection(url, _fontName))
				{
					ERRORWINDOW_GLOBAL("Failed to add local font collection: \"" + _fontName + L"\" (" + _localUrl + L")");
					return nullptr;
				}
			}
			auto& pFontCollection = localFontCollectionMap_[_fontName];

			UINT32 familyCount = pFontCollection->GetFontFamilyCount();
			if (familyCount == 0)
			{
				ERRORWINDOW_GLOBAL("No font families found in local font collection: \"" + _fontName + L"\" (" + _localUrl + L")");
				return nullptr;
			}

			ComPtr<IDWriteFontFamily> pFamily;
			HRESULT hr = pFontCollection->GetFontFamily(0, pFamily.GetAddressOf());
			if (FAILED(hr) || !pFamily)
			{
				ERRORWINDOW_GLOBAL("Failed to get font family from local collection: \"" + _fontName + L"\" (" + _localUrl + L")");
				return nullptr;
			}

			ComPtr<IDWriteLocalizedStrings> familyNames;
			hr = pFamily->GetFamilyNames(familyNames.GetAddressOf());
			if (FAILED(hr) || !familyNames)
			{
				ERRORWINDOW_GLOBAL("Failed to get family names from font family: \"" + _fontName + L"\" (" + _localUrl + L")");
				return nullptr;
			}

			// Try to find an English locale name first, otherwise take the first available name
			UINT32 nameIndex = 0;
			BOOL exists = FALSE;
			familyNames->FindLocaleName(L"en-us", &nameIndex, &exists);
			if (!exists)
			{
				nameIndex = 0;
			}

			UINT32 nameLength = 0;
			familyNames->GetStringLength(nameIndex, &nameLength);
			std::wstring actualFamilyName(nameLength + 1, L'\0');
			familyNames->GetString(nameIndex, &actualFamilyName[0], nameLength + 1);

			ComPtr<IDWriteTextFormat> textFormat;
			hr = getDWriteFactory()->CreateTextFormat(
				actualFamilyName.c_str(),
				pFontCollection.Get(),
				_fontWeight,
				_fontStyle,
				_fontStretch,
				_size,
				L"",
				textFormat.GetAddressOf()
			);

			if (FAILED(hr))
			{
				ERRORWINDOW_GLOBAL("Failed to create text format for local font: \"" + _fontName + L"\" (" + _localUrl + L")");
				return nullptr;
			}

			return textFormat;
		}

		bool TextFactoryGlobal::addLocalFontCollection(const String& _localUrl, const String& _fontName)
		{
			ComPtr<IDWriteFontCollection> pFontCollection = nullptr;
			ComPtr<IDWriteFontFile> pFontFile;
			String url = filesystem::absolutePath(_localUrl);

			IDWriteFactory* pFactory = getDWriteFactory();
			ComPtr<IDWriteFactory5> pFactory5;
			HRESULT hr = pFactory->QueryInterface(__uuidof(IDWriteFactory5),
				reinterpret_cast<void**>(pFactory5.GetAddressOf()));
			if (FAILED(hr))
			{
				ERRORWINDOW_GLOBAL("Failed to query IDWriteFactory5 interface.");
				return false;
			}

			ComPtr<IDWriteFontSetBuilder1> fontSetBuilder;
			hr = pFactory5->CreateFontSetBuilder(
				fontSetBuilder.GetAddressOf()
			);
			if (FAILED(hr))
			{
				ERRORWINDOW_GLOBAL("Failed to create font set builder.");
				return false;
			}

			ComPtr<IDWriteFontFile> fontFile;
			hr = pFactory->CreateFontFileReference(
				_localUrl.content,
				nullptr,
				fontFile.GetAddressOf()
			);
			if (FAILED(hr))
			{
				ERRORWINDOW_GLOBAL("Failed to create font file reference.");
				return false;
			}

			hr = fontSetBuilder->AddFontFile(
				fontFile.Get()
			);
			if (FAILED(hr))
			{
				ERRORWINDOW_GLOBAL("Failed to add font file to font set builder.");
				return false;
			}

			ComPtr<IDWriteFontSet> fontSet;
			hr = fontSetBuilder->CreateFontSet(
				fontSet.GetAddressOf()
			);
			if (FAILED(hr))
			{
				ERRORWINDOW_GLOBAL("Failed to create font set.");
				return false;
			}

			ComPtr<IDWriteFontCollection1> customFontCollection;
			hr = pFactory5->CreateFontCollectionFromFontSet(
				fontSet.Get(),
				customFontCollection.GetAddressOf()
			);
			if (FAILED(hr))
			{
				ERRORWINDOW_GLOBAL("Failed to create font collection from font set.");
				return false;
			}

			localFontCollectionMap_[_fontName] = customFontCollection;
			return true;
		}

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