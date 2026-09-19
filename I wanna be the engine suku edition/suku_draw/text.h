#pragma once
#include <suku_foundation/suku_string.h>
#include <dwrite.h>
#include <wrl/client.h>
#include "color.h"
#include "transform.h"
#include "brush.h"
#include <map>
#include <utility>

namespace suku
{
	using Microsoft::WRL::ComPtr;

	class TextStyle
	{
	public:
		enum class Align
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

		enum class WrapOption
		{
			NoWrap = DWRITE_WORD_WRAPPING_NO_WRAP,
			Wrap = DWRITE_WORD_WRAPPING_CHARACTER,
			WrapWord = DWRITE_WORD_WRAPPING_WRAP,
		};

		enum class Weight
		{
			Thin = DWRITE_FONT_WEIGHT_THIN,
			ExtraLight = DWRITE_FONT_WEIGHT_EXTRA_LIGHT,
			Light = DWRITE_FONT_WEIGHT_LIGHT,
			SemiLight = DWRITE_FONT_WEIGHT_SEMI_LIGHT,
			Normal = DWRITE_FONT_WEIGHT_NORMAL,
			Medium = DWRITE_FONT_WEIGHT_MEDIUM,
			SemiBold = DWRITE_FONT_WEIGHT_SEMI_BOLD,
			Bold = DWRITE_FONT_WEIGHT_BOLD,
			ExtraBold = DWRITE_FONT_WEIGHT_EXTRA_BOLD,
			Black = DWRITE_FONT_WEIGHT_BLACK,
			ExtraBlack = DWRITE_FONT_WEIGHT_EXTRA_BLACK
		};

		enum class ItalicType
		{
			None = DWRITE_FONT_STYLE_NORMAL,
			Oblique = DWRITE_FONT_STYLE_OBLIQUE,
			Italic = DWRITE_FONT_STYLE_ITALIC
		};

		enum class Stretch
		{
			UltraCondensed = DWRITE_FONT_STRETCH_ULTRA_CONDENSED,
			ExtraCondensed = DWRITE_FONT_STRETCH_EXTRA_CONDENSED,
			Condensed = DWRITE_FONT_STRETCH_CONDENSED,
			SemiCondensed = DWRITE_FONT_STRETCH_SEMI_CONDENSED,
			Normal = DWRITE_FONT_STRETCH_NORMAL,
			SemiExpanded = DWRITE_FONT_STRETCH_SEMI_EXPANDED,
			Expanded = DWRITE_FONT_STRETCH_EXPANDED,
			ExtraExpanded = DWRITE_FONT_STRETCH_EXTRA_EXPANDED,
			UltraExpanded = DWRITE_FONT_STRETCH_ULTRA_EXPANDED
		};

		TextStyle(String _fontName, float _size,
			TextStyle::Align _textAlign = TextStyle::Align::TopLeft,
			TextStyle::WrapOption _wrapOption = TextStyle::WrapOption::Wrap);
		TextStyle(String _fontName, float _size,
			TextStyle::Weight _fontWeight, TextStyle::ItalicType _fontStyle, TextStyle::Stretch _fontStretch,
			TextStyle::Align _textAlign = TextStyle::Align::TopLeft,
			TextStyle::WrapOption _wrapOption = TextStyle::WrapOption::Wrap);
		TextStyle(String _fontName, String _localUrl, float _size,
			TextStyle::Align _textAlign = TextStyle::Align::TopLeft,
			TextStyle::WrapOption _wrapOption = TextStyle::WrapOption::Wrap);
		TextStyle(String _fontName, String _localUrl, float _size,
			TextStyle::Weight _fontWeight, TextStyle::ItalicType _fontStyle, TextStyle::Stretch _fontStretch,
			TextStyle::Align _textAlign = TextStyle::Align::TopLeft,
			TextStyle::WrapOption _wrapOption = TextStyle::WrapOption::Wrap);		
		TextStyle(String _fontName, String _localUrl, String _localeName, float _size,
				TextStyle::Align _textAlign = TextStyle::Align::TopLeft,
				TextStyle::WrapOption _wrapOption = TextStyle::WrapOption::Wrap);
		TextStyle(String _fontName, String _localUrl, String _localeName, float _size,
			TextStyle::Weight _fontWeight, TextStyle::ItalicType _fontStyle, TextStyle::Stretch _fontStretch,
			TextStyle::Align _textAlign = TextStyle::Align::TopLeft,
			TextStyle::WrapOption _wrapOption = TextStyle::WrapOption::Wrap);

		static void registerLocalFont(String _fontName, String _localUrl, String _localeName = "en-us");

		void setTextAlign(TextStyle::Align _textAlign);
		TextStyle::Align getTextAlign() { return textAlign_; }
		void setTextWrapOption(TextStyle::WrapOption _option);
		TextStyle::WrapOption getTextWrapOption() { return textWrapOption_; }

		int getContentLineCount(String _text, float _width);
		float getContentHeight(String _text, float _width);

		void paint(String _text, Transform _transform = Transform());
		void paint(String _text, float _x, float _y, Transform _transform = Transform());
		void paint(String _text, float _x, float _y, const Brush& _brush, Transform _transform = Transform());
		void paint(String _text, float _x, float _y, float _width, float _height, Transform _transform = Transform());
		void paint(String _text, float _x, float _y, float _width, float _height, const Brush& _brush, Transform _transform = Transform());

		void setBrush(const Brush& _brush);
	private:
		String fontName_;
		float size_;
		ComPtr<IDWriteTextFormat> pTextFormat_;
		Brush brush_;
		TextStyle::Align textAlign_;
		TextStyle::WrapOption textWrapOption_;
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

			static ComPtr<IDWriteTextFormat> createTextFormat(
				const String& _fontName, float _size,
				DWRITE_FONT_WEIGHT _fontWeight = DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE _fontStyle = DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH _fontStretch = DWRITE_FONT_STRETCH_NORMAL
			);
			static ComPtr<IDWriteTextFormat> createTextFormat(
				const String& _fontName, const String& _localUrl, float _size,
				DWRITE_FONT_WEIGHT _fontWeight = DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE _fontStyle = DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH _fontStretch = DWRITE_FONT_STRETCH_NORMAL
			);
			static ComPtr<IDWriteTextFormat> createTextFormat(
				const String& _fontName, const String& _localUrl, const String& _localeName, float _size,
				DWRITE_FONT_WEIGHT _fontWeight = DWRITE_FONT_WEIGHT_NORMAL,
				DWRITE_FONT_STYLE _fontStyle = DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH _fontStretch = DWRITE_FONT_STRETCH_NORMAL
			);

			static bool addLocalFontCollection(const String& _localUrl, const String& _fontName, const String& _localeName = "en-us");

		private:
			TextFactoryGlobal();
			ComPtr<IDWriteFactory> pDWriteFactory_ = nullptr;
			inline static std::map<String, std::pair<String, ComPtr<IDWriteFontCollection>>> localFontCollectionMap_;
		};
	}
}