#pragma once
#include "../UIElement.h"
namespace suku
{
	class IFocusable
	{
	public:
		virtual void focus() = 0;
		virtual bool unfocus() = 0; //返回值表示是否接受被取消选择
		virtual bool isFocused() const = 0;
	};

	class IFocusableImplBasic : public IFocusable
	{
	public:
		virtual void focus() override { isFocused_ = true; }
		virtual bool unfocus() override { isFocused_ = false; return true; }
		virtual bool isFocused() const override { return isFocused_; }
	protected:
		bool isFocused_ = false;
	};
} 