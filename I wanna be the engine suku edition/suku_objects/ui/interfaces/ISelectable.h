#pragma once
#include "../UIElement.h"
namespace suku
{
	class ISelectable
	{
	public:
		virtual void select() = 0;
		virtual bool deselect() = 0; //返回值表示是否接受被取消选择
		virtual bool isSelected() const = 0;
	};
} 