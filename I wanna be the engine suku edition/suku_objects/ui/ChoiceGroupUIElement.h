#pragma once
#include "UIElement.h"
#include <suku_foundation/input.h>
#include <suku_foundation/suku_string.h>
#include <vector>
#include "interfaces/IFocusable.h"

namespace suku
{
	class ChoiceGroupUIElement : public UIElement, public IFocusableImplBasic
	{
	public:
		ChoiceGroupUIElement(std::vector<String>&& _options, unsigned int _defaultIndex = 0);
		unsigned int getCurrentIndex();
		const String& getCurrentChoice();

		virtual void onUpdate() override;
		virtual void onPaint() override;
	private:
		std::vector<String> options_;
		unsigned int selectedIndex_;
	};
}