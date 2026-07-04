#pragma once

#include "OptionElement.h"
#include "suku_core/object.h"
#include <list>
#include <initializer_list>

namespace suku
{
	class OptionPool : public Object
	{
	public:
		OptionPool(OptionElement&& _element, Room* _inRoom);
		OptionPool(std::initializer_list<OptionElement> _elements, Room* _inRoom);

		//template<typename... OptionElementType>
		//	requires (std::is_same_v<OptionElementType, OptionElement> && ...)
		//OptionPool(OptionElementType... _elements) : Object() { optionElements_ = { _elements... }; }
	};
}