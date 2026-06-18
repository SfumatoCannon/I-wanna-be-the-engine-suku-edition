#pragma once
#include "UIElement.h"

namespace suku
{
	class OptionElement : public UIElement
	{
	public:
		OptionElement* prev = nullptr;
		OptionElement* next = nullptr;
		OptionElement(float _x = 0, float _y = 0, int _width = 96, int _height = 32) : UIElement(_x, _y, _width, _height) {}
		virtual void update() override;
		virtual bool onPaint() override;
	private:
		bool isSelected_ = false;
	};
}