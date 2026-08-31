#include "tile_manager.h"

namespace suku
{
    Tile::Tile(Sprite& _sprSource, Sprite&& _spr)
        :spriteSource_(_sprSource), sprite_(std::move(_spr))
    {
    }

    Tile::~Tile()
    {
        auto iter = spriteLinkPool_.find(&spriteSource_);
        if (iter != spriteLinkPool_.end())
        {
            if ((*iter).second == &sprite_)
            {
                std::swap(spriteSource_, sprite_);
                spriteLinkPool_.erase(iter);
            }
        }
    }

    void Tile::use()
    {
        if (spriteLinkPool_.find(&spriteSource_) != spriteLinkPool_.end())
        {
            std::swap(spriteSource_, *spriteLinkPool_[&spriteSource_]);
        }
        std::swap(spriteSource_, sprite_);
        spriteLinkPool_[&spriteSource_] = &sprite_;
    }

    void Tile::unuse()
    {
        auto iter = spriteLinkPool_.find(&spriteSource_);
        if (iter != spriteLinkPool_.end())
        {
            if ((*iter).second == &sprite_)
            {
                std::swap(spriteSource_, sprite_);
                spriteLinkPool_.erase(iter);
            }
        }
    }

    void Tile::resetAll()
    {
        for (auto& [key, value] : spriteLinkPool_)
        {
            std::swap(*key, *value);
        }
        spriteLinkPool_.clear();
    }

    TilePack::TilePack(Tile&& _tile)
    {
        add(std::move(_tile));
    }

    void TilePack::add(Tile&& _tile)
    {
        tiles_.emplace_back(std::move(_tile));
    }

    void TilePack::use()
    {
        for (auto& i : tiles_)
        {
            i.use();
        }
    }

    void TilePack::unuse()
    {
        for (auto& i : tiles_)
        {
            i.unuse();
        }
    }
}