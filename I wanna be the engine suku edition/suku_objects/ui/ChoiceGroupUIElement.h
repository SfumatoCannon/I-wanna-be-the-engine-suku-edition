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
		ChoiceGroupUIElement(real _x, real _y, int _width, int _height, std::vector<String>&& _options, unsigned int _defaultIndex = 0);
		unsigned int getCurrentIndex();
		const String& getCurrentChoice();

		ChoiceGroupUIElement* setOptions(std::vector<String>&& _options);
		ChoiceGroupUIElement* setSelectedIndex(unsigned int _index);

		const std::vector<String>& getOptions() { return options_; }
		unsigned int getOptionIndex(const String& _option);
		const String& getOption(unsigned int _index);

		ChoiceGroupUIElement* enableOption(unsigned int _index);
		ChoiceGroupUIElement* enableOption(const String& _option) { return enableOption(getOptionIndex(_option)); }
		ChoiceGroupUIElement* disableOption(unsigned int _index);
		ChoiceGroupUIElement* disableOption(const String& _option) { return disableOption(getOptionIndex(_option)); }

		ChoiceGroupUIElement* renameOption(unsigned int _index, const String& _newOption);

		virtual void onUpdate() override;
		virtual void onPaint() override;
	private:
		std::vector<String> options_;
		std::vector<bool> isEnabled_;
		unsigned int selectedIndex_;
	};
}