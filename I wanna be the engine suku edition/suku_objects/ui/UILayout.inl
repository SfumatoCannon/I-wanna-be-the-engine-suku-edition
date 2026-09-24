#include "UILayout.h"
#include "../../suku_foundation/input.h"

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
			T* newElement = inRoom_->create<T>(element).get();
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
	void UILayoutVertical<T>::onPostUpdate()
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

	template<typename T>
		requires std::is_base_of_v<UIElement, T> && std::is_base_of_v<IFocusable, T>
	void UILayoutVerticalSelectable<T>::focus()
	{
		isSelected_ = true;
		if (this->elements_.size() == 0)
			return;
		if (input::isKeyDown(VK_DOWN))
		{
			selectedElementIndex = 0;
		}
		else if (input::isKeyDown(VK_UP))
		{
			selectedElementIndex = (int)this->elements_.size() - 1;
		}
		this->elements_[selectedElementIndex]->focus();
		}

	template<typename T>
		requires std::is_base_of_v<UIElement, T> && std::is_base_of_v<IFocusable, T>
	inline bool UILayoutVerticalSelectable<T>::unfocus()
	{
		if (!isFocused())
			return false;
		if (!this->elements_[selectedElementIndex]->unfocus())
			return false;
		if (input::isKeyDown(VK_DOWN))
		{
			if (selectedElementIndex == this->elements_.size() - 1)
			{
				isSelected_ = false;
				return true;
			}
			else
			{
				selectedElementIndex++;
				this->elements_[selectedElementIndex]->focus();
				return false;
			}
		}
		else if (input::isKeyDown(VK_UP))
		{
			if (selectedElementIndex == 0)
			{
				isSelected_ = false;
				return true;
			}
			else
			{
				selectedElementIndex--;
				this->elements_[selectedElementIndex]->focus();
				return false;
			}
		}
	}

	template<typename T>
		requires std::is_base_of_v<UIElement, T>&& std::is_base_of_v<IFocusable, T>
	inline void UILayoutVerticalSelectable<T>::onPostUpdate()
	{
		if (isFocused())
		{
			if (input::isKeyDown(VK_DOWN))
			{
				if (selectedElementIndex < this->elements_.size() - 1)
				{
					this->elements_[selectedElementIndex]->unfocus();
					selectedElementIndex++;
					this->elements_[selectedElementIndex]->focus();
				}

			}
			else if (input::isKeyDown(VK_UP))
			{
				if (selectedElementIndex > 0)
				{
					this->elements_[selectedElementIndex]->unfocus();
					selectedElementIndex--;
					this->elements_[selectedElementIndex]->focus();
				}
			}
		}
		UILayoutVertical<T>::onPostUpdate();
	}
}