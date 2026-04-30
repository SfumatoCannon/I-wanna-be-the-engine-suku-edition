#pragma once
#include "config_element.h"

namespace suku
{
	class ConfigElementPool
	{
	public:

		// Initial Window State
		inline static ConfigElement<bool> isMaximized = { "Window", "isMaximized", false };
		inline static ConfigElement<bool> isFullScreen = { "Window", "isFullScreen", false };
		inline static ConfigElement<int> windowWidth = { "Window", "width", -1 };
		inline static ConfigElement<int> windowHeight = { "Window", "height", -1 };
		inline static ConfigElement<int> windowPosX = { "Window", "posX", -1 };
		inline static ConfigElement<int> windowPosY = { "Window", "posY", -1 };

		// Render
		inline static ConfigElement<int> renderFPS = { "Render", "fps", 50 };
		inline static ConfigElement<bool> isVSyncOn = { "Render", "vsync", true };
	};
}