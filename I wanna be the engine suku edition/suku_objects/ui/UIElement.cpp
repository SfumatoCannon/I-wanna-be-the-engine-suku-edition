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
		RectangleShape area(width_, height_);
		Text text("Consolas", 12, TextAlign::MiddleCenter);
		area.setFill(Color(128, 128, 128, 0.5f));
		area.paint(x, y, transform);
		text.setBrush(Color(255, 255, 0));
		text.textContent = L"UI";
		text.paint(x, y, width_, height_, graphics::createSolidColorBrush(Color(255, 255, 255)));
		return false;
	}
}