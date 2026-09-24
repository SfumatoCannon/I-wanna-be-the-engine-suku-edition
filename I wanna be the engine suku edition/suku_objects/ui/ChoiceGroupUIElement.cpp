#include "ChoiceGroupUIElement.h"
#include <definitions/suku_font_pool.h>

namespace suku
{
	ChoiceGroupUIElement::ChoiceGroupUIElement(std::vector<String>&& _options, unsigned int _defaultIndex)
		:options_(std::move(_options)), selectedIndex_(_defaultIndex)
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
		if (input::isKeyDown(VK_LEFT))
		{
			if (selectedIndex_ > 0)
				selectedIndex_--;
		}
		else if (input::isKeyDown(VK_RIGHT))
		{
			if (selectedIndex_ < options_.size() - 1)
				selectedIndex_++;
		}
	}

	void ChoiceGroupUIElement::onPaint()
	{
		// left arrow
		textStyle_.paint(" < ", x, y);
		// right arrow
		textStyle_.setTextAlign(TextStyle::Align::MiddleRight);
		textStyle_.paint(" > ", x + width_, y)
	}
}