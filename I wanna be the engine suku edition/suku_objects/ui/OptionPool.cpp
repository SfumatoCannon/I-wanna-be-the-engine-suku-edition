#include "OptionPool.h"

namespace suku
{
	void OptionPool::make(std::initializer_list<OptionElement> _elements, float _x, float _y, int _elementWidth, int _elementHeight)
	{
		int i = 0;
		OptionElement* prevElement = nullptr;
		for (auto& element : _elements)
		{
			OptionElement* newElement = inRoom_->create<OptionElement>(element);
			newElement->x = _x;
			newElement->y = _y + i * _elementHeight;
			newElement->width_ = _elementWidth;
			newElement->height_ = _elementHeight;
			if (prevElement)
			{
				prevElement->next = newElement;
				newElement->prev = prevElement;
			}
			else // prevElement == nullptr
			{
				newElement->isSelected_ = true;
			}
			prevElement = newElement;
			i++;
		}
	}
}