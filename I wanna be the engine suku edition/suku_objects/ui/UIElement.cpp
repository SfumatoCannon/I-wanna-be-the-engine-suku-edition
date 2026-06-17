#include "UIElement.h"

namespace suku
{
	UIElement::UIElement(float _x, float _y, int _width, int _height)
		: Object(_x, _y), width_(_width), height_(_height)
	{
	}

	void UIElement::update()
	{
	}

	bool UIElement::onPaint()
	{
		return false;
	}
}