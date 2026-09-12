#include "brush.h"

#include "color.h"
#include "bitmap.h"
#include "draw_core.h"

namespace suku
{
	Brush::Brush(Color _color)
	{
		pBrush_ = graphics::createSolidColorBrush(_color);
	}

	Brush::Brush(Bitmap _bitmap)
	{
		pBrush_ = graphics::createBitmapBrush(_bitmap.getD2DBitmap());
	}

	Brush Brush::solidColorBrush(Color _color)
	{
		return Brush(_color);
	}

	Brush Brush::bitmapBrush(Bitmap _bitmap)
	{
		return Brush(_bitmap);
	}
}