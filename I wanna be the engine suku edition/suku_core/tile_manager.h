#pragma once

#include "sprite.h"
#include <suku_foundation/suku_type.h>
#include <suku_core/object.h>
#include <map>

namespace suku
{
	class TileManager
	{
	public:
		TileManager(std::initializer_list<std::pair<Typecode, Sprite>> _objectTileList);
		
		template<typename T> Sprite* getTile() { return getTile(typecode(T)); }
		Sprite* getTile(Typecode _typecode);
	private:
		std::map<Typecode, Sprite> tileOfObjectType_;
	};
}