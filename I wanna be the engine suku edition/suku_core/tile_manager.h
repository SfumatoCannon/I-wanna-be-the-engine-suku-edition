#pragma once

#include "sprite.h"
#include <suku_foundation/suku_type.h>
#include <suku_core/object.h>
#include <map>

namespace suku
{
	class ITile
	{
	public:
		virtual void makeTile() = 0;
		virtual void resetTile() = 0;
	};

	template<suku_object Obj>
	class Tile : public ITile
	{
	public:
		Tile(Sprite&& _spr);
		virtual void makeTile(Room* _room) override;
		virtual void resetTile(Room* _room) override;
	private:
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