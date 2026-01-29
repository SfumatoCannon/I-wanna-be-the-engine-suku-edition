#pragma once
#include <d2d1.h>

namespace suku
{
	enum BitmapScaleMode
	{
		NearestNeighbor = D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		Linear = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
	};
}