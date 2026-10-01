#include "ChoiceGroupUIElement.h"
#include <definitions/suku_font_pool.h>

namespace suku
{
	ChoiceGroupUIElement::ChoiceGroupUIElement(std::vector<String>&& _options, unsigned int _defaultIndex)
		:UIElement(0, 0, 192, 32), options_(std::move(_options)), isEnabled_(options_.size(), true), selectedIndex_(_defaultIndex)
	{
	}

	ChoiceGroupUIElement::ChoiceGroupUIElement(real _x, real _y, int _width, int _height, std::vector<String>&& _options, unsigned int _defaultIndex)
		:UIElement(_x, _y, _width, _height), options_(std::move(_options)), isEnabled_(options_.size(), true), selectedIndex_(_defaultIndex)
	{
	}

	unsigned int ChoiceGroupUIElement::getCurrentIndex()
	{
		return selectedIndex_;
	}

	const String& ChoiceGroupUIElement::getCurrentChoice()
	{
		return options_[selectedIndex_];
	}

	ChoiceGroupUIElement* ChoiceGroupUIElement::setOptions(std::vector<String>&& _options)
	{
		options_ = _options;
		if (selectedIndex_ >= options_.size())
			selectedIndex_ = options_.size() - 1;
		isEnabled_ = std::vector<bool>(options_.size(), true);
		return this;
	}

	ChoiceGroupUIElement* ChoiceGroupUIElement::setSelectedIndex(unsigned int _index)
	{
		selectedIndex_ = _index;
		if (selectedIndex_ >= options_.size())
			selectedIndex_ = options_.size() - 1;
		return this;
	}

	unsigned int ChoiceGroupUIElement::getOptionIndex(const String& _option)
	{
		unsigned int result = 0;
		for (auto& str : options_)
		{
			if (str == _option)
				return result;
			result++;
		}
		return UINT_MAX;
	}

	const String& ChoiceGroupUIElement::getOption(unsigned int _index)
	{
		if (_index >= options_.size())
			return options_[options_.size() - 1];
		return options_[_index];
	}

	ChoiceGroupUIElement* ChoiceGroupUIElement::enableOption(unsigned int _index)
	{
		if (_index < options_.size())
			isEnabled_[_index] = true;
		return this;
	}

	ChoiceGroupUIElement* ChoiceGroupUIElement::disableOption(unsigned int _index)
	{
		isEnabled_[_index] = false;
		if (selectedIndex_ == _index)
		{
			selectedIndex_++;
			while (selectedIndex_ < options_.size() && isEnabled_[selectedIndex_] == false)
				selectedIndex_++;
			if (selectedIndex_ < options_.size())
				return this;

			selectedIndex_ = _index - 1;
			while (selectedIndex_ >= 0 && selectedIndex_ < options_.size() && isEnabled_[selectedIndex_] == false)
				selectedIndex_--;
			if (selectedIndex_ >= 0 && selectedIndex_ < options_.size())
				return this;
			
			selectedIndex_ = UINT_MAX;
		}
		return this;
	}

	ChoiceGroupUIElement* ChoiceGroupUIElement::renameOption(unsigned int _index, const String& _newOption)
	{
		if (_index < options_.size())
		{
			options_[_index] = _newOption;
		}
		return this;
	}

	void ChoiceGroupUIElement::onUpdate()
	{
		if (!isFocused_)
			return;
		if (selectedIndex_ == UINT_MAX)
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
		if (selectedIndex_ != UINT_MAX)
		{
			textStyle_.paint(options_[selectedIndex_], x + 16, y, width_ - 32, height_, TextStyle::Align::MiddleCenter);
		}
	}
}