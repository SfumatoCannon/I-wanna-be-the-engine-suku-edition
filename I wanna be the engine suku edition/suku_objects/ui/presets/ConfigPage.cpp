#include "ConfigPage.h"
#include <suku_gamewindow/includes.h>

namespace suku
{
	ConfigPage::ConfigPage() : UILayoutVerticalSelectable<ConfigOptionUIElement>(0, 0, GameWindow::getLogicalWidth(), GameWindow::getLogicalHeight())
	{
		float configElementWidth = width_ * 0.5f;
		float configElementHeight = 48;
		make({
				{ GameLoopConfig::renderFPS, "Render FPS" },
				{ GameLoopConfig::isVSyncOn, "Vsync", "abcdddffffffffffffffffffffff\nddddddddddddddddddddd"},
				{ GameLoopConfig::vsyncFrameRate, "Vsync Frame Rate" },
				{ GameWindowConfig::isFullScreen, "Fullscreen"}
			}, 
			configElementWidth, configElementHeight
		);
	}
}