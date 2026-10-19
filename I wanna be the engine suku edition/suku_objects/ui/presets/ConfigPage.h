#pragma once

#include "../UILayout.h"
#include "../ConfigOptionUIElement.h"
#include "../interfaces/IFocusable.h"

namespace suku
{
	class ConfigPage : public UILayoutVerticalSelectable<ConfigOptionUIElement>, IFocusableImplBasic
	{
	public:
		ConfigPage(Room* _inRoom);

		virtual void focus() override;
		virtual bool unfocus() override;
	};
}