#pragma once
#include "tile.h"

namespace suku
{
	template<typename... T>			
		requires (std::is_same_v<T, Tile> && ...)
	TilePack::TilePack(T&&... _tiles)
	{
		(add(std::move(_tiles)), ...);
	}
}