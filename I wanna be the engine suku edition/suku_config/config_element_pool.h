#pragma once
#include "config_element.h"

namespace suku
{
	class ConfigElementPool
	{
	public:
		inline static ConfigElement<int> renderFPS = { "renderFPS", 50 };
		inline static ConfigElement<bool> isMaximized = { "isMaximized", false };
		inline static ConfigElement<bool> isFullScreen = { "isFullScreen", false };
		inline static ConfigElement<bool> isVSyncOn = { "Vsync", true };
		inline static ConfigElement<int> windowWidth = { "windowWidth", -1 };
		inline static ConfigElement<int> windowHeight = { "windowHeight", -1 };
		inline static ConfigElement<int> windowPosX = { "windowPosX", -1 };
		inline static ConfigElement<int> windowPosY = { "windowPosY", -1 };
	};
}