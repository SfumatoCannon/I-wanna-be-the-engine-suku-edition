#pragma once

#include "sprite.h"
#include <list>
#include <map>

namespace suku
{
	class Tile
	{
	public:
		Tile(Sprite& _sprSource, Sprite&& _spr);
		Tile(Tile&& _other) = default;
		~Tile();
		void use();
		void unuse();
		static void resetAll();
	private:
		inline static std::map<Sprite*, Sprite*> spriteLinkPool_;
		Sprite& spriteSource_;
		Sprite sprite_;
	};

	class TilePack
	{
	public:
		template<typename... T>
			requires (std::is_same_v<T, Tile> && ...)
		TilePack(T&&... _tiles);

		TilePack(std::list<Tile>&& _tiles) : tiles_(std::move(_tiles)) {}
		TilePack(Tile&& _tile);

		void add(Tile&& _tile);
		void use();
		void unuse();

		static void resetAll() { Tile::resetAll(); }
	private:
		std::list<Tile> tiles_;
	};
}

#include "tile_manager.inl"