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
		OptionPool(int _elementWidth = 256, int _elementHeight = 32) : Object(0, 0), elementWidth_(_elementWidth), elementHeight_(_elementHeight) {}
		void make(std::initializer_list<OptionElement> _elements,
			float _x, float _y, int _elementWidth, int _elementHeight);
		void make(std::initializer_list<OptionElement> _elements,
			float _x, float _y) { make(_elements, _x, _y, elementWidth_, elementHeight_); }
		virtual void update() override;
	private:
		std::list<OptionElement*> elements_;
		int elementWidth_, elementHeight_;
	};
}