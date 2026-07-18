#pragma once

#include "UIElement.h"
#include "interfaces/ISelectable.h"

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
		virtual void postUpdate() override;
		virtual bool onPaint() override { return false; }
	private:
		std::list<T*> elements_;
	};
}

#include "UILayout.inl"