#include "pch.h"
#include "text.h"
#include "draw_core.h"

namespace suku
{
	Text::Text(String _fontName, float _size) : fontName_(_fontName), size_(_size)
	{
		graphics::TextFactoryGlobal::getDWriteFactory()->CreateTextFormat(
			_fontName.content,
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			_size,
			L"en_us",
			pTextFormat_.GetAddressOf()
		);
	}

	void Text::paint(float _x, float _y, const ComPtr<ID2D1Brush>& _brush)
	{
		graphics::pD2DContext->DrawTextW(
			textContent.content,
			wcslen(textContent.content),
			pTextFormat_.Get(),
			D2D1::RectF(_x, _y, _x + 1024, _y + 1024),
			_brush.Get()
		);
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