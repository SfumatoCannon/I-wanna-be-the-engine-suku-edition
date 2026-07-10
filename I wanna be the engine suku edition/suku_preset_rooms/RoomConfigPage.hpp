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
				{ ConfigElementPool::isVSyncOn, "Vsync", "abcdddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff\nddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"},
				{ ConfigElementPool::vsyncFrameRate, "Vsync Frame Rate" },
				{ ConfigElementPool::isFullScreen, "Fullscreen"}
			}
		, (windowWidth - configElementWidth) / 2, 64, configElementWidth, configElementHeight);
	}
};