#include "UILayout.h"

namespace suku
{
	template<typename T>
		requires std::is_base_of_v<UIElement, T>
	void UILayoutVertical<T>::addElement(T&& _element)
	{
		width_ = std::max(width_, _element.getWidth());
		height_ += _element.getHeight();
		elements_.push_back(std::move(_element));
	}

	template<typename T>
		requires std::is_base_of_v<UIElement, T>
	void UILayoutVertical<T>::make(std::initializer_list<T> _elements, int _elementWidth, int _elementHeight)
	{
		clear();
		for (auto& element : _elements)
		{
			T* newElement = inRoom_->create<T>(element);
			if (newElement == nullptr)
			{
				WARNINGWINDOW_GLOBAL("Failed to create UIElement in UILayoutVertical::make()");
				continue;
			}
			elements_.push_back(newElement);
			newElement.width_ = _elementWidth;
			newElement.height_ = _elementHeight;
			addElement(std::move(newElement));
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

	}
}