#include "UIElement.h"

namespace suku
{
	UIElement::UIElement(real _x, real _y, real _width, real _height)
		: Object(_x, _y), width_(_width), height_(_height), area_(RectangleShape(_width, _height))
	{
	}

	void UIElement::onUpdate()
	{
	}

	void UIElement::onPaint()
	{
		// area
		paintArea();
		// text
		Text text({ "Consolas", 12, TextStyle::Align::MiddleCenter });
		text.style.setBrush(Color(255, 255, 255));
		text.text = L"UI";
		text.paint(x, y, width_, height_);
	}

	void UIElement::setTextStyle(const TextStyle& _font)
	{}

	void UIElement::paintArea()const
	{
		area_.paint(x, y, getTransform(), backgroundBrush_, outlineBrush_);
	}

	void UIElement::paintBackground()const
	{
		area_.paintFill(x, y, getTransform(), backgroundBrush_);
	}

	void UIElement::paintOutline()const
	{
		area_.paintOutline(x, y, getTransform(), outlineBrush_);
	}

	void UIElement::updateArea()
	{
		area_ = RectangleShape(width_, height_);
	}
}