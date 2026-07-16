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
		create<OptionPool>()->make(
			{
				{ ConfigElementPool::renderFPS, "Render FPS" },
				{ ConfigElementPool::isVSyncOn, "Vsync", "abcdddffffffffffffffffffffff\nddddddddddddddddddddd"},
				{ ConfigElementPool::vsyncFrameRate, "Vsync Frame Rate" },
				{ ConfigElementPool::isFullScreen, "Fullscreen"}
			}
		, (windowWidth - configElementWidth) / 2, 64, configElementWidth, configElementHeight);
		//create<UILayoutVertical<OptionElement>>((windowWidth - configElementWidth) / 2, 0)->make(
		//	{
		//		{ ConfigElementPool::renderFPS, "Render FPS" },
		//		{ ConfigElementPool::isVSyncOn, "Vsync", "abcdddffffffffffffffffffffff\nddddddddddddddddddddd"},
		//		{ ConfigElementPool::vsyncFrameRate, "Vsync Frame Rate" },
		//		{ ConfigElementPool::isFullScreen, "Fullscreen"}
		//	},
		//	configElementWidth, configElementHeight
		//);
	}
};