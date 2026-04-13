#pragma once

#include <suku_core/room.h>

namespace suku
{
	class MapLoader
	{
		static void loadFromJtoolMsg(suku::Room* _room, std::string _msg);
	};
}