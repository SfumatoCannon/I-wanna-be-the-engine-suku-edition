#include "../I wanna be the engine suku edition.h"

using namespace suku;

class RoomConfigPage : public Room
{
public:
	RoomConfigPage() : Room()
	{
		int windowWidth = GameWindow::getLogicalWidth();
		int windowHeight = GameWindow::getLogicalHeight();
		float configElementWidth = windowWidth * 0.5f;
		float configElementHeight = 48;
		auto optionLayout = create<UILayoutVerticalSelectable<OptionElement>>((windowWidth - configElementWidth) / 2, 0);
		optionLayout->make(
			{
				{ GameLoopConfig::renderFPS, "Render FPS" },
				{ GameLoopConfig::isVSyncOn, "Vsync", "abcdddffffffffffffffffffffff\nddddddddddddddddddddd"},
				{ GameLoopConfig::vsyncFrameRate, "Vsync Frame Rate" },
				{ GameWindowConfig::isFullScreen, "Fullscreen"}
			},
			configElementWidth, configElementHeight
		);
		optionLayout->select();
		
		create<PausePage>();
	}
};