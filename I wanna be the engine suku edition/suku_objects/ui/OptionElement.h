#pragma once
#include "UIElement.h"

namespace suku
{
	template<typename T>
	class OptionElement : public UIElement
	{
	public:
		OptionElement* prev = nullptr;
		OptionElement* next = nullptr;
		OptionElement(float _x = 0, float _y = 0);
		virtual void update() override;
		virtual bool onPaint() override;
	private:
		std::vector<T> options_;
		bool isSelected_ = false;
	};
}