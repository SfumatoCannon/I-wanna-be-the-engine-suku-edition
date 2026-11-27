#pragma once

#include "../UILayout.h"
#include "../ConfigOptionUIElement.h"
#include "../interfaces/IFocusable.h"
#include <suku_core/action.h>

namespace suku
{
	class ConfigPage : public UILayoutVerticalSelectable<ConfigOptionUIElement>
	{
	public:
		ConfigPage(Room* _inRoom);

		virtual void focus() override;
		virtual void unfocus() override;

		virtual void onUpdate() override;
	};
	OBJ_PAINT_ORDER(Object, ConfigPage);
	OBJ_PAINT_ORDER(ConfigPage, ConfigOptionUIElement);

	// Used in rTitle & rSelectSave, which has no PausePage, to call out config settings directly
	class ConfigPageCaller : public Object
	{
	public:
		ConfigPageCaller() : Object() { visible = false; }
		virtual void onUpdate() override
		{
			if (!inRoom_)
				return;

			ConfigPage* pConfigPage = inRoom_->getObject<ConfigPage>();
			if (!pConfigPage)
				return;

			if (input::isKeyDown(VK_BACK))
			{
				if (pConfigPage->isFocused())
				{
					pConfigPage->unfocus();
				}
				else
				{
					pConfigPage->focus();
				}
			}
		}
	};
}