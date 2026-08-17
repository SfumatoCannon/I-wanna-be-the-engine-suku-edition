#pragma once

#include <suku_core/room.h>
#include <suku_foundation/suku_string.h>

namespace suku
{
	class MapLoader
	{
	public:
		static void loadFromJtoolData(suku::Room* _room, std::string_view _data);
		static void loadFromJtoolFile(suku::Room* _room, String _url);
	};
}