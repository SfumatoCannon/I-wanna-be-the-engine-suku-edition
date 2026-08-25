#pragma once

namespace suku
{
	template<typename SprZ>
	inline Sprite::Sprite(const SprZ& _spriteZ) 
		: width_(_spriteZ.width), height_(_spriteZ.height), centerX_(_spriteZ.centerX), centerY_(_spriteZ.centerY)
	{
		flipTime_ = 1;
		push(_spriteZ);
	}

	template<typename SprZ, typename ...SprZNext>
	inline Sprite::Sprite(int _flipTime, const SprZ& _spriteZ, const SprZNext & ..._spriteZNext)
		: width_(_spriteZ.width), height_(_spriteZ.height), centerX_(_spriteZ.centerX), centerY_(_spriteZ.centerY)
	{
		flipTime_ = _flipTime;
		push(_spriteZ);
		push(_spriteZNext...);
	}

	template<typename SprZ>
	inline void Sprite::init(SprZ&& _spriteZ)
	{
		bodyList.clear();
		flipTime_ = 1;
		push(_spriteZ);
	}

	template<typename SprZ, typename ...SprZNext>
	inline void Sprite::init(int _flipTime, SprZ&& _spriteZ, SprZNext&& ..._spriteZNext)
	{
		bodyList.clear();
		flipTime_ = _flipTime;
		push(_spriteZ);
		push(_spriteZNext...);
	}

	template<typename SprZ>
	inline void Sprite::push(SprZ&& _spriteZ)
	{
		if (_spriteZ.width > width_)
			width_ = _spriteZ.width;
		if (_spriteZ.height > height_)
			height_ = _spriteZ.height;
		bodyList.emplace_back(new SprZ(std::move(_spriteZ)));
	}

	template<typename SprZ, typename ...SprZNext>
	inline void Sprite::push(SprZ&& _spriteZ, SprZNext&& ..._spriteZNext)
	{
		push(_spriteZ);
		push(_spriteZNext...);
	}
}