#include "OptionPool.h"

namespace suku
{
	OptionPool::OptionPool(OptionElement&& _element, Room* _inRoom)
	{
		_inRoom->create<OptionElement>(_element);
	}

	OptionPool::OptionPool(std::initializer_list<OptionElement> _elements,
		float _x, float _y, int _elementWidth, int _elementHeight, Room* _inRoom)
	{
		int i = 0;
		for (auto& element : _elements)
		{
			OptionElement* newElement = _inRoom->create<OptionElement>(element);
			newElement->x = _x;
			newElement->y = _y + i * _elementHeight;
			newElement->width_ = _elementWidth;
			newElement->height_ = _elementHeight;
			i++;
		}
	}
}