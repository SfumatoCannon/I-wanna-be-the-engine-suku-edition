#pragma once
#include "sprite.h"

namespace suku
{
	template<sprite_element_type T>
	inline Sprite::Sprite(T&& _spriteZ) 
		: width_(_spriteZ.width), height_(_spriteZ.height), centerX_(_spriteZ.centerX), centerY_(_spriteZ.centerY)
	{
		flipTime_ = 1;
		push(std::move(_spriteZ));
	}

	template<sprite_element_type T, sprite_element_type... TNext>
	inline Sprite::Sprite(int _flipTime, T&& _spriteZ, TNext&& ..._spriteZNext)
		: width_(_spriteZ.width), height_(_spriteZ.height), centerX_(_spriteZ.centerX), centerY_(_spriteZ.centerY)
	{
		flipTime_ = _flipTime;
		push(std::move(_spriteZ));
		push(std::move(_spriteZNext...));
	}

	template<sprite_element_type T>
	inline void Sprite::init(T&& _spriteZ)
	{
		bodyList.clear();
		flipTime_ = 1;
		push(std::move(_spriteZ));
	}

	template<sprite_element_type T, sprite_element_type... TNext>
	inline void Sprite::init(int _flipTime, T&& _spriteZ, TNext&& ..._spriteZNext)
	{
		bodyList.clear();
		flipTime_ = _flipTime;
		push(std::move(_spriteZ));
		push(std::move(_spriteZNext...));
	}

	template<sprite_element_type T>
	inline void Sprite::push(T&& _spriteZ)
	{
		if (_spriteZ.width > width_)
			width_ = _spriteZ.width;
		if (_spriteZ.height > height_)
			height_ = _spriteZ.height;
		bodyList.push_back(std::make_unique<T>(std::move(_spriteZ)));
	}

	template<sprite_element_type T, sprite_element_type... TNext>
	inline void Sprite::push(T&& _spriteZ, TNext&& ..._spriteZNext)
	{
		push(std::move(_spriteZ));
		push(std::move(_spriteZNext...));
	}
}