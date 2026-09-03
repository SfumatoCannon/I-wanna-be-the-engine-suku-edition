#pragma once
#include "object_definer.h"

namespace suku
{
	class Block :public Solid
	{
	public:
		inline static Sprite spr{ BitmapSpriteElement("Image\\wall.png", SquareShape(32), 16, 16) };
		Block(float _x = 0, float _y = 0) : Solid(_x, _y)
		{
			sprite_ = &spr;
			setPreUpdateId(-1);
			setUpdateId(-1);
			setPostUpdateId(-1);
			setPaintId(2);
		}
	};
}