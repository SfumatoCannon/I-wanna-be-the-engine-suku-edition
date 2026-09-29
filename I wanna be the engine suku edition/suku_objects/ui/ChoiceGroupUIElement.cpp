#include "ChoiceGroupUIElement.h"
#include <definitions/suku_font_pool.h>

namespace suku
{
	ChoiceGroupUIElement::ChoiceGroupUIElement(std::vector<String>&& _options, unsigned int _defaultIndex)
		:UIElement(0, 0, 192, 32), options_(std::move(_options)), selectedIndex_(_defaultIndex)
	{}

	ChoiceGroupUIElement::ChoiceGroupUIElement(real _x, real _y, int _width, int _height, std::vector<String>&& _options, unsigned int _defaultIndex)
		:UIElement(_x, _y, _width, _height), options_(std::move(_options)), selectedIndex_(_defaultIndex)
	{}

	unsigned int ChoiceGroupUIElement::getCurrentIndex()
	{
		return selectedIndex_;
	}

	const String& ChoiceGroupUIElement::getCurrentChoice()
	{
		return options_[selectedIndex_];
	}

	void ChoiceGroupUIElement::onUpdate()
	{
		if (!isFocused_)
			return;
		if (input::isKeyDown(VK_LEFT_ARROW))
		{
			if (selectedIndex_ > 0)
				selectedIndex_--;
		}
		else if (input::isKeyDown(VK_RIGHT_ARROW))
		{
			if (selectedIndex_ < options_.size() - 1)
				selectedIndex_++;
		}
	}

	void ChoiceGroupUIElement::onPaint()
	{
		// left arrow
		textStyle_.paint(" < ", x, y, width_, height_, TextStyle::Align::MiddleLeft);
		// right arrow
		textStyle_.paint(" > ", x, y, width_, height_, TextStyle::Align::MiddleRight);
		// option
		textStyle_.paint(options_[selectedIndex_], x + 16, y, width_ - 32, height_, TextStyle::Align::MiddleCenter);
	}
}