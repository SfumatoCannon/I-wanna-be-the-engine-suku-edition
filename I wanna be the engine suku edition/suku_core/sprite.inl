#pragma once

namespace suku
{

	template<typename SprZ>
	inline Sprite::Sprite(const SprZ& _spriteZ)
	{
		flipTime_ = 1;
		push(_spriteZ);
	}

	template<typename SprZ, typename ...SprZNext>
	inline Sprite::Sprite(int _flipTime, const SprZ& _spriteZ, const SprZNext & ..._spriteZNext)
	{
		flipTime_ = _flipTime;
		push(_spriteZ);
		push(_spriteZNext...);
	}

	template<typename SprZ>
	inline void Sprite::init(const SprZ& _spriteZ)
	{
		bodyList.clear();
		flipTime_ = 1;
		push(_spriteZ);
	}

	template<typename SprZ, typename ...SprZNext>
	inline void Sprite::init(int _flipTime, const SprZ& _spriteZ, const SprZNext & ..._spriteZNext)
	{
		bodyList.clear();
		flipTime_ = _flipTime;
		push(_spriteZ);
		push(_spriteZNext...);
	}

	template<typename SprZ>
	inline void Sprite::push(const SprZ& _spriteZ)
	{
		SprZ* newSpriteZ = new SprZ(_spriteZ);
		bodyList.push_back(newSpriteZ);
	}

	template<typename SprZ, typename ...SprZNext>
	inline void Sprite::push(const SprZ& _spriteZ, const SprZNext & ..._spriteZNext)
	{
		push(_spriteZ);
		push(_spriteZNext...);
	}
}