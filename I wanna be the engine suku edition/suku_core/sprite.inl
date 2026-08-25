#pragma once

namespace suku
{
	template<typename SprZ>
	inline Sprite::Sprite(SprZ&& _spriteZ) 
		: width_(_spriteZ.width), height_(_spriteZ.height), centerX_(_spriteZ.centerX), centerY_(_spriteZ.centerY)
	{
		flipTime_ = 1;
		push(std::move(_spriteZ));
	}

	template<typename SprZ, typename ...SprZNext>
	inline Sprite::Sprite(int _flipTime, SprZ&& _spriteZ, SprZNext&& ..._spriteZNext)
		: width_(_spriteZ.width), height_(_spriteZ.height), centerX_(_spriteZ.centerX), centerY_(_spriteZ.centerY)
	{
		flipTime_ = _flipTime;
		push(std::move(_spriteZ));
		push(std::move(_spriteZNext...));
	}

	template<typename SprZ>
	inline void Sprite::init(SprZ&& _spriteZ)
	{
		bodyList.clear();
		flipTime_ = 1;
		push(std::move(_spriteZ));
	}

	template<typename SprZ, typename ...SprZNext>
	inline void Sprite::init(int _flipTime, SprZ&& _spriteZ, SprZNext&& ..._spriteZNext)
	{
		bodyList.clear();
		flipTime_ = _flipTime;
		push(std::move(_spriteZ));
		push(std::move(_spriteZNext...));
	}

	template<typename SprZ>
	inline void Sprite::push(SprZ&& _spriteZ)
	{
		if (_spriteZ.width > width_)
			width_ = _spriteZ.width;
		if (_spriteZ.height > height_)
			height_ = _spriteZ.height;
		bodyList.push_back(std::make_unique<SprZ>(std::move(_spriteZ)));
	}

	template<typename SprZ, typename ...SprZNext>
	inline void Sprite::push(SprZ&& _spriteZ, SprZNext&& ..._spriteZNext)
	{
		push(std::move(_spriteZ));
		push(std::move(_spriteZNext...));
	}
}