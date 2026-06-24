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
		OptionPool(OptionElement _element);
		OptionPool(std::initializer_list<OptionElement> _elements);

		template<typename... OptionElementType>
			requires (std::is_same_v<OptionElementType, OptionElement> && ...)
		OptionPool(OptionElementType... _elements) : Object() { optionElements_ = { _elements... }; }

		virtual void onRoomEntered() override;
	private:
		std::list<OptionElement> optionElements_;
	};
}