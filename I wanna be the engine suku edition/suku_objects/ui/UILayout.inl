#include "UILayout.h"

namespace suku
{
	template<typename T>
		requires std::is_base_of_v<UIElement, T>
	void UILayoutVertical<T>::addElement(T* _element)
	{
		if (_element->getWidth() > width_)
			width_ = _element->getWidth();
		height_ += _element->getHeight();
		elements_.push_back(_element);
	}

	template<typename T>
		requires std::is_base_of_v<UIElement, T>
	void UILayoutVertical<T>::make(std::initializer_list<T> _elements, int _elementWidth, int _elementHeight)
	{
		clear();
		int i = 0;
		for (auto& element : _elements)
		{
			T* newElement = inRoom_->create<T>(element);
			if (newElement == nullptr)
			{
				WARNINGWINDOW_GLOBAL("Failed to create UIElement in UILayoutVertical::make()");
				continue;
			}
			newElement->x = x;
			newElement->y = y + i * _elementHeight;
			newElement->setWidth(_elementWidth);
			newElement->setHeight(_elementHeight);
			addElement(newElement);
			i++;
		}
	}

	template<typename T>
		requires std::is_base_of_v<UIElement, T>
	void UILayoutVertical<T>::clear()
	{
		for (auto& element : elements_)
		{
			if (element)
				element->remove();
		}
		elements_.clear();
		width_ = 0;
		height_ = 0;
	}

	template<typename T>
		requires std::is_base_of_v<UIElement, T>
	void UILayoutVertical<T>::onRemove()
	{
		clear();
	}

	template<typename T>
		requires std::is_base_of_v<UIElement, T>
	void UILayoutVertical<T>::postUpdate()
	{
		bool isFirstElement = true;
		int totalY = this->y;
		for (T* element : elements_)
		{
			if (isFirstElement)
			{
				isFirstElement = false;
			}
			else
			{
				element->y = totalY;
			}
			totalY += element->getHeight();
		}
		setHeight(totalY - y);
	}
}