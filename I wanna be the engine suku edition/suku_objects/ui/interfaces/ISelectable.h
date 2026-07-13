#pragma once
#include "../UIElement.h"
namespace suku
{
	class ISelectable : public UIElement
	{
	public:
		virtual void select() = 0;
		virtual void deselect() = 0;
		virtual bool isSelected() const = 0;
	};
}