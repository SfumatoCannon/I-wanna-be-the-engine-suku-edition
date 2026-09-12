#pragma once

#include "draw_core.h"
#include <wrl/client.h>

namespace suku
{
	class Color;
	class Bitmap;

	using Microsoft::WRL::ComPtr;

	class Brush
	{
	public:
		Brush(Color _color);
		Brush(Bitmap _bitmap);

		static Brush solidColorBrush(Color _color);
		static Brush bitmapBrush(Bitmap _bitmap);

		ComPtr<ID2D1Brush> get() { return pBrush_; }
	private:
		ComPtr<ID2D1Brush> pBrush_ = nullptr;
	};
}