#include "OptionPool.h"

namespace suku
{
	OptionPool::OptionPool(OptionElement _element)
	{
		optionElements_.push_back(_element);
	}

	OptionPool::OptionPool(std::initializer_list<OptionElement> _elements)
	{
		optionElements_ = _elements;
	}

	void OptionPool::onRoomEntered()
	{
		for (auto& optionElement : optionElements_)
		{
			inRoom_->create(optionElement);
		}
	}
}