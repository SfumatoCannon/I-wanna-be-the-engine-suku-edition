#include "ConfigPage.h"
#include <suku_gamewindow/includes.h>

namespace suku
{
	ConfigPage::ConfigPage(Room* _inRoom) : UILayoutVerticalSelectable<ConfigOptionUIElement>(0, 0, GameWindow::getLogicalWidth(), GameWindow::getLogicalHeight(), _inRoom)
	{
		float configElementWidth = width_;
		float configElementHeight = 48;
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
		IFocusableImplBasic::focus();
		isVisible = true;
		for (auto& o : subElements_)
			o->isVisible = true;
	}

	bool ConfigPage::unfocus()
	{
		IFocusableImplBasic::unfocus();
		isVisible = false;
		for (auto& o : subElements_)
			o->isVisible = false;
		return true;
	}
}