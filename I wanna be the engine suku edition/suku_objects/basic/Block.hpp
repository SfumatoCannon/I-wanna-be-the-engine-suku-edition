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
		virtual void onRoomEnter() override
		{
			// autotile the block
			if (!isInRoom())
				return;
			if (sprite_->getFlipTime() != 0)
				return;
			auto [width, height] = sprite_->getSize();
			auto [row, col] = sprite_->getRowColCount();
			if (row == 1 && col == 1)
			{
				return;
			}
			else if ((row == 1 && col == 2) || (row == 2 && col == 1))
			{
				if (getCrashedObject<Block>(x, y - height) != nullptr)
				{
					this->spriteBasicIndex = 1;
				}
				else
				{
					this->spriteBasicIndex = 0;
				}
			}
		}
	};
}