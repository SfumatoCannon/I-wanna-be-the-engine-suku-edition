#pragma once
#include <suku_config/config_element.h>

namespace suku
{
	class ConfigElementPool
	{
	public:
		// Render
		inline static ConfigElement<int> renderFPS = { "Render", "fps", 50, {50, 60, 90, 100, 120, 144, 160, 200, 240} };
		inline static ConfigElement<bool> isVSyncOn = { "Render", "vsync", true };
		inline static ConfigElement<int> vsyncFrameRate = { "Render", "vsyncFrameRate", 1, 1, 4 };
	};
}