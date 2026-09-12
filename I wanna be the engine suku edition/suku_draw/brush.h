#pragma once

#include <wrl/client.h>
#include <d2d1.h>

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

		ComPtr<ID2D1Brush> getD2DBrush()const { return pBrush_; }
	private:
		ComPtr<ID2D1Brush> pBrush_ = nullptr;
	};
}