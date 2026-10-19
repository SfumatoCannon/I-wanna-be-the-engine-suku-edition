#include "UILayout.h"
#include "../../suku_foundation/input.h"

namespace suku
{
	template<typename T>
		requires std::is_base_of_v<UIElement, T>
	void UILayoutVertical<T>::addElement(std::shared_ptr<T> _element)
	{
		if (_element->getWidth() > width_)
			width_ = _element->getWidth();
		height_ += _element->getHeight();
		subElements_.push_back(_element);
	}

	template<typename T>
		requires std::is_base_of_v<UIElement, T>
	void UILayoutVertical<T>::make(std::initializer_list<T> _elements, int _elementWidth, int _elementHeight)
	{
		clear();
		int i = 0;
		if (inRoom_ == nullptr)
		{
			ERRORWINDOW("Failed to create sub element because inRoom is NULL");
			return;
		}
		for (auto& element : _elements)
		{
			auto newElement = inRoom_->create<T>(element);
			if (newElement == nullptr)
			{
				WARNINGWINDOW("Failed to create sub element");
				continue;
			}
			newElement->setPaintId(paintId_);
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
		for (auto& element : subElements_)
		{
			if (element)
				element->remove();
		}
		subElements_.clear();
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
	void UILayoutVertical<T>::onUpdateEnd()
	{
		bool isFirstElement = true;
		int totalY = this->y;
		for (auto& element : subElements_)
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
		isFocused_ = true;
		if (this->subElements_.size() == 0)
			return;
		if (input::isKeyDown(VK_DOWN))
		{
			selectedElementIndex = 0;
		}
		else if (input::isKeyDown(VK_UP))
		{
			selectedElementIndex = (int)this->subElements_.size() - 1;
		}
		this->subElements_[selectedElementIndex]->focus();
	}

	template<typename T>
		requires std::is_base_of_v<UIElement, T> && std::is_base_of_v<IFocusable, T>
	inline bool UILayoutVerticalSelectable<T>::unfocus()
	{
		if (!isFocused())
			return false;
		if (!this->subElements_[selectedElementIndex]->unfocus())
			return false;
		if (input::isKeyDown(VK_DOWN))
		{
			if (selectedElementIndex == this->subElements_.size() - 1)
			{
				isFocused_ = false;
				return true;
			}
			else
			{
				selectedElementIndex++;
				this->subElements_[selectedElementIndex]->focus();
				return false;
			}
		}
		else if (input::isKeyDown(VK_UP))
		{
			if (selectedElementIndex == 0)
			{
				isFocused_ = false;
				return true;
			}
			else
			{
				selectedElementIndex--;
				this->subElements_[selectedElementIndex]->focus();
				return false;
			}
		}
		return false;
	}

	template<typename T>
		requires std::is_base_of_v<UIElement, T>&& std::is_base_of_v<IFocusable, T>
	inline void UILayoutVerticalSelectable<T>::onUpdateEnd()
	{
		if (isFocused())
		{
			if (input::isKeyDown(VK_DOWN))
			{
				if (selectedElementIndex < this->subElements_.size() - 1)
				{
					this->subElements_[selectedElementIndex]->unfocus();
					selectedElementIndex++;
					this->subElements_[selectedElementIndex]->focus();
				}

			}
			else if (input::isKeyDown(VK_UP))
			{
				if (selectedElementIndex > 0)
				{
					this->subElements_[selectedElementIndex]->unfocus();
					selectedElementIndex--;
					this->subElements_[selectedElementIndex]->focus();
				}
			}
		}
		UILayoutVertical<T>::onUpdateEnd();
	}
}