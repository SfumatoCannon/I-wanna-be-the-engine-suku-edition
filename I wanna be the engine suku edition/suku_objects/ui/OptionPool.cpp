#include "OptionPool.h"

namespace suku
{
	OptionPool::OptionPool(OptionElement&& _element, Room* _inRoom)
	{
		//_inRoom->create(_element);
	}

	OptionPool::OptionPool(std::initializer_list<OptionElement> _elements, Room* _inRoom)
	{
		//optionElements_ = _elements;
	}
}