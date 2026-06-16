#pragma once
#include "suku_core/object.h"

namespace suku
{
	class UIElement : public Object
	{
	public:
		UIElement(float _x = 0, float _y = 0, int _width = 32, int _height = 32);
		virtual void update() override;
		virtual bool onPaint() override;
	private:
		int width_, height_;
	};
}