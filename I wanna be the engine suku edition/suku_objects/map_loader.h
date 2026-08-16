#pragma once

#include <suku_core/room.h>

namespace suku
{
	class MapLoader
	{
	public:
		static void loadFromJtool(suku::Room* _room, std::string_view _msg);
	};
}