#include "ChoiceGroupUIElement.h"

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


}