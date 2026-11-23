#pragma once
#include "../UIElement.h"
namespace suku
{
	class IFocusable
	{
	public:
		virtual ~IFocusable() = default;
		virtual void focus() = 0;
		virtual void unfocus() = 0;
		virtual bool isFocused() const = 0;
	protected:
		IFocusable() = default;
		IFocusable(const IFocusable& _other) = default;
	};

	class IFocusableImplBasic : public IFocusable
	{
	public:
		virtual void focus() override { isFocused_ = true; }
		virtual void unfocus() override { isFocused_ = false; }
		virtual bool isFocused() const override { return isFocused_; }
	protected:
		bool isFocused_ = false;
	};
} 