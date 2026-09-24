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
		UILayoutVertical<T>(float _x, float _y) : UIElement(_x, _y, 0, 0) { setUpdateId(-1); }
		void addElement(T* _element);
		void make(std::initializer_list<T> _elements, int _elementWidth, int _elementHeight);
		void clear();
		virtual void onRemove() override;
		virtual void onPostUpdate() override;
		virtual bool onPaint() override { return false; }
	protected:
		std::vector<T*> elements_;
	};

	template<typename T>
		requires std::is_base_of_v<UIElement, T> && std::is_base_of_v<IFocusable, T>
	class UILayoutVerticalSelectable : public UILayoutVertical<T>, IFocusable
	{
	public:
		UILayoutVerticalSelectable<T>(float _x, float _y) : UILayoutVertical<T>(_x, _y) { this->setUpdateId(1); }

		// Implement IFocusable interface
		virtual void focus() override;
		virtual bool unfocus() override;
		virtual bool isFocused() const override { return isSelected_; }

		virtual void onPostUpdate() override;
	private:
		bool isSelected_ = false;
		int selectedElementIndex = 0;
	};
}

#include "UILayout.inl"