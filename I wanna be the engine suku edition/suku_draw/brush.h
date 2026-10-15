#pragma once

#include <wrl/client.h>
#include <d2d1.h>
#include <suku_foundation/maths.h>
#include <suku_draw/transform.h>

namespace suku
{
	class Color;
	class Bitmap;

	using Microsoft::WRL::ComPtr;

	class Brush
	{
	public:
		Brush(Color _color, real _opacity = 1.0);
		Brush(Bitmap _bitmap, real _opacity = 1.0);
		Brush() = default;
		Brush(const Brush& _other) = default;

		bool isValid()const { return pBrush_ != nullptr; }
		static Brush solidColorBrush(Color _color);
		static Brush bitmapBrush(Bitmap _bitmap, real _opacity = 1.0);
		static Brush emptyBrush() { return Brush(); }

		static const Brush& defaultBrush();

		ComPtr<ID2D1Brush> getD2DBrush()const { return pBrush_; }

		void setOpacity(real _opacity);
		real getOpacity();
		void setTransform(Transform _transform);
		Transform getTransform();
	private:
		ComPtr<ID2D1Brush> pBrush_ = nullptr;
	};
}