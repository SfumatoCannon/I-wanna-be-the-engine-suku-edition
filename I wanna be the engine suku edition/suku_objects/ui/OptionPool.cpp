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
			if (newElement == nullptr)
			{
				WARNINGWINDOW_GLOBAL("Failed to create OptionElement in OptionPool::make()");
				continue;
			}
			elements_.push_back(newElement);
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

	void OptionPool::update()
	{
		if (input::isKeyDown(VK_UP))
		{
			for (OptionElement* element : elements_)
			{
				if (element->isSelected_ && element->prev)
				{
					element->isSelected_ = false;
					element->prev->isSelected_ = true;
					break;
				}
			}
		}
		else if (input::isKeyDown(VK_DOWN))
		{
			for (OptionElement* element : elements_)
			{
				if (element->isSelected_ && element->next)
				{
					element->isSelected_ = false;
					element->next->isSelected_ = true;
					break;
				}
			}
		}
	}
}