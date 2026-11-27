#include "ConfigPage.h"
#include <suku_gamewindow/includes.h>

namespace suku
{
	ConfigPage::ConfigPage(Room* _inRoom) : UILayoutVerticalSelectable<ConfigOptionUIElement>(0, 0, GameWindow::getLogicalWidth(), GameWindow::getLogicalHeight(), _inRoom)
	{
		float configElementWidth = width_;
		float configElementHeight = 48;
		setPaintId(1000);
		make({
				{ GameLoopConfig::renderFPS, "Render FPS" },
				{ GameLoopConfig::isVSyncOn, "Vsync", "abcdddffffffffffffffffffffff\nddddddddddddddddddddd"},
				{ GameLoopConfig::vsyncFrameRate, "Vsync Frame Rate" },
				{ GameWindowConfig::isFullScreen, "Fullscreen"}
			}, 
			configElementWidth, configElementHeight
		);
		unfocus();
	}

	void ConfigPage::focus()
	{
		UILayoutVerticalSelectable<ConfigOptionUIElement>::focus();
		setLayoutVisible(true);
	}

	void ConfigPage::unfocus()
	{
		UILayoutVerticalSelectable<ConfigOptionUIElement>::unfocus();
		setLayoutVisible(false);
	}

	void ConfigPage::onUpdate()
	{
		UILayoutVerticalSelectable<ConfigOptionUIElement>::onUpdate();
		if (!isFocused())
			return;
		if (input::isKeyUp(VK_Z))
		{
			unfocus();
		}
	}
}