#include "OptionPool.h"

namespace suku
{
	void OptionPool::make(std::initializer_list<OptionElement> _elements, float _x, float _y, int _elementWidth, int _elementHeight)
	{
		int i = 0;
		for (auto& element : _elements)
		{
			OptionElement* newElement = inRoom_->create<OptionElement>(element);
			newElement->x = _x;
			newElement->y = _y + i * _elementHeight;
			newElement->width_ = _elementWidth;
			newElement->height_ = _elementHeight;
			i++;
		}
	}

	void OptionPool::make(std::initializer_list<OptionElement> _elements, float _x, float _y)
	{
		int i = 0;
		for (auto& element : _elements)
		{
			OptionElement* newElement = inRoom_->create<OptionElement>(element);
			newElement->x = x;
			newElement->y = y + i * elementHeight_;
			newElement->width_ = elementWidth_;
			newElement->height_ = elementHeight_;
			i++;
		}
	}
}