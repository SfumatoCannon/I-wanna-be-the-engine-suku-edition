#pragma once
#include "config_element.h"

namespace suku
{
	class ConfigElementPool
	{
	public:
		inline static ConfigElement<int> renderFPS = { "renderFPS", 50 };
	};
}