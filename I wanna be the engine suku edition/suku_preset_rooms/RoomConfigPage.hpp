#include "../I wanna be the engine suku edition.h"

using namespace suku;

class RoomConfigPage : public Room
{
public:
	RoomConfigPage() : Room()
	{
		create<OptionPool>()->make(
			{
				{ ConfigElementPool::renderFPS, "Render FPS" },
				{ ConfigElementPool::isVSyncOn, "Vsync"},
				{ ConfigElementPool::vsyncFrameRate, "Vsync Frame Rate" },
				{ ConfigElementPool::isFullScreen, "Fullscreen"}
			}
		, 100, 100, 256, 64);
	}
};