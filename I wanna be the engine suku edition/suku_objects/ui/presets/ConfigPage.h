#pragma once

#include "../UILayout.h"
#include "../ConfigOptionUIElement.h"

namespace suku
{
	class ConfigPage : public UILayoutVerticalSelectable<ConfigOptionUIElement>
	{
	public:
		ConfigPage();

	};
}