#pragma once

#include "UIElement.h"
#include "interfaces/IFocusable.h"
#include <vector>

namespace suku
{

	template<typename T>
		requires std::is_base_of_v<UIElement, T>
	class UILayoutVertical : public UIElement
	{
	public:
		UILayoutVertical<T>(real _x, real _y, real _width = 0, real _height = 0, Room* _inRoom = nullptr) : UIElement(_x, _y, _width, _height, _inRoom) { setUpdateId(-1); }
		void addElement(std::shared_ptr<T> _element);
		void make(std::initializer_list<T> _elements, int _elementWidth, int _elementHeight);
		void clear();
		virtual void onRemove() override;
		virtual void onUpdateEnd() override;
		virtual void onPaint() override { }
	protected:
		std::vector<std::shared_ptr<T>> subElements_;
	};

	template<typename T>
		requires std::is_base_of_v<UIElement, T> && std::is_base_of_v<IFocusable, T>
	class UILayoutVerticalSelectable : public UILayoutVertical<T>, IFocusable
	{
	public:
		UILayoutVerticalSelectable<T>(real _x, real _y, real _width = 0, real _height = 0, Room* _inRoom = nullptr) : UILayoutVertical<T>(_x, _y, _width, _height, _inRoom) { this->setUpdateId(1); }

		// Implement IFocusable interface
		virtual void focus() override;
		virtual bool unfocus() override;
		virtual bool isFocused() const override { return isFocused_; }

		virtual void onUpdateEnd() override;
	private:
		bool isFocused_ = false;
		int selectedElementIndex = 0;
	};
}

#include "UILayout.inl"