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
		focus();
	}

	void ConfigPage::focus()
	{
		UILayoutVerticalSelectable<ConfigOptionUIElement>::focus();
		IFocusableImplBasic::focus();
		visible = true;
		for (auto& o : subElements_)
			o->visible = true;
	}

	void ConfigPage::unfocus()
	{
		UILayoutVerticalSelectable<ConfigOptionUIElement>::unfocus();
		IFocusableImplBasic::unfocus();
		visible = false;
		for (auto& o : subElements_)
			o->visible = false;
	}
}