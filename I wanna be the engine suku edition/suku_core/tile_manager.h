#pragma once

#include "sprite.h"
#include <suku_foundation/suku_type.h>
#include <suku_core/object.h>
#include <map>

namespace suku
{
	class Tile
	{
	public:
		Tile(Sprite& _sprSource, Sprite&& _spr);
		~Tile();
		void use();
		static void resetAll();
	private:
		inline static std::map<Sprite*, Sprite*> spriteLinkPool_;
		Sprite& spriteSource_;
		Sprite sprite_;
	};

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

#include "tile_manager.inl"