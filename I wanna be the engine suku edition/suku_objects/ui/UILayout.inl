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

	template<typename T>
		requires std::is_base_of_v<UIElement, T> && std::is_base_of_v<ISelectable, T>
	void UILayoutVerticalSelectable<T>::select()
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
			selectedElementIndex = this->elements_.size() - 1;
		}
		this->elements_[selectedElementIndex]->select();
		}

	template<typename T>
		requires std::is_base_of_v<UIElement, T> && std::is_base_of_v<ISelectable, T>
	inline bool UILayoutVerticalSelectable<T>::deselect()
	{
		if (!isSelected())
			return false;
		if (!this->elements_[selectedElementIndex]->deselect())
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
				this->elements_[selectedElementIndex]->select();
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
				this->elements_[selectedElementIndex]->select();
				return false;
			}
		}
	}

	template<typename T>
		requires std::is_base_of_v<UIElement, T>&& std::is_base_of_v<ISelectable, T>
	inline void UILayoutVerticalSelectable<T>::postUpdate()
	{
		if (isSelected())
		{
			if (input::isKeyDown(VK_DOWN))
			{
				if (selectedElementIndex < this->elements_.size() - 1)
				{
					this->elements_[selectedElementIndex]->deselect();
					selectedElementIndex++;
					this->elements_[selectedElementIndex]->select();
				}

			}
			else if (input::isKeyDown(VK_UP))
			{
				if (selectedElementIndex > 0)
				{
					this->elements_[selectedElementIndex]->deselect();
					selectedElementIndex--;
					this->elements_[selectedElementIndex]->select();
				}
			}
		}
		UILayoutVertical<T>::postUpdate();
	}
}