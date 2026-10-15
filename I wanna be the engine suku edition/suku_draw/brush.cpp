#include "brush.h"

#include "color.h"
#include "bitmap.h"
#include "draw_core.h"

#include <suku_foundation/maths.h>

namespace suku
{
	Brush::Brush(Color _color, real _opacity)
	{
		pBrush_ = graphics_core::createSolidColorBrush(_color);
		pBrush_->SetOpacity(_opacity);
	}

	Brush::Brush(Bitmap _bitmap, real _opacity)
	{
		pBrush_ = graphics_core::createBitmapBrush(_bitmap.getD2DBitmap());
		pBrush_->SetOpacity(_opacity);
	}

	Brush Brush::solidColorBrush(Color _color)
	{
		return Brush(_color);
	}

	Brush Brush::bitmapBrush(Bitmap _bitmap, real _opacity)
	{
		return Brush(_bitmap, _opacity);
	}

	const Brush& Brush::defaultBrush()
	{
		static Brush defaultBrush(solidColorBrush(Color::Black));
		return defaultBrush;
	}

	void Brush::setOpacity(real _opacity)
	{
		if (pBrush_)
			pBrush_->SetOpacity(_opacity);
	}

	real Brush::getOpacity()
	{
		if (!pBrush_)
			return 0.0;
		return pBrush_->GetOpacity();
	}

	void Brush::setTransform(Transform _transform)
	{
		if (pBrush_)
			pBrush_->SetTransform(_transform.getMatrix());
	}

	Transform Brush::getTransform()
	{
		if (!pBrush_)
			return Transform();
		D2D1::Matrix3x2F matrix;
		pBrush_->GetTransform(&matrix);
		return Transform(matrix);
	}
}