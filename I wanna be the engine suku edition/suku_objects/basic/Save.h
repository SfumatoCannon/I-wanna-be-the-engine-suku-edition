#pragma once
#include "object_definer.h"

namespace suku
{
	class Save : public Object
	{
	public:
		const int cooldown = 30;
		inline static Sprite spr{ "Image\\save.png", 2, 0, SquareShape(32), 16, 16 };
		Save(float _x = 0.0f, float _y = 0.0f) : Object(_x, _y)
		{
			sprite_ = &spr;
		}
		virtual void onUpdateEnd() override;
	private:
		long long lastSaved_ = -1;
	};
}