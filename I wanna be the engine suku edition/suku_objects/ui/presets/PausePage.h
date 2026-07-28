#pragma once
#include "../UIElement.h"
#include "../UILayout.h"

namespace suku
{
	class PausePage : public UIElement
	{
	public:
		PausePage();
		virtual void update() override;
		virtual bool onPaint() override;
	private:
		bool isPaused_ = false;
	};
}