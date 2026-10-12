#pragma once
#include "suku_foundation/includes.h"
#include "suku_core/includes.h"
#include "suku_foundation/suku_type.h"
#include <suku_core/sprite.h>
#include <suku_constants.h>

namespace suku
{
	class PlaceChangerRaw;
	template<suku_room T> class PlaceChanger;
	template<suku_room T> class Warp;
	class Block;
	class Spike;
	class Cherry;
	class Platform;
	class VineLeft;
	class VineRight;
	class Trigger;
	class Water;
	class WaterExtraJump;

	class Trigger :public Object
	{
	public:
		inline static Sprite spr{ BitmapSpriteElement(32, 32, SquareShape(32), 0, 0) };
		Trigger(float _x = 0, float _y = 0) : Object(_x, _y)
		{
			sprite_ = &spr;
		}
	};

	class PlaceChangerRaw :public Object
	{
	public:
		PlaceChangerRaw(float _x = 0, float _y = 0) : Object(_x, _y)
		{
			sprite_ = &Trigger::spr;
		}

		Room* getRoomTo() const
		{
			return getRoomToFunc_ ? getRoomToFunc_() : nullptr;
		}

		template<suku_room T>
		void setRoomTo()
		{
			getRoomToFunc_ = []() { return RoomPool::getRoom<T>(); };
			warpFunc_ = []() { gotoRoom<T>(); };
		}

		void warp()
		{
			if (warpFunc_)
				warpFunc_();
		}
	private:
		std::function<Room* ()> getRoomToFunc_;
		std::function<void()> warpFunc_;
	};

	template<suku_room T>
	class PlaceChanger :public PlaceChangerRaw
	{
	public:
		PlaceChanger(float _x = 0, float _y = 0) : PlaceChangerRaw(_x, _y)
		{
			sprite_ = &Trigger::spr;
			setRoomTo<T>();
		}
	};

	template<suku_room T>
	class Warp :public PlaceChanger<T>
	{
	public:
		inline static Sprite spr{ BitmapSpriteElement("Image\\warp.png", 16, 16, 0.2f) };
		Warp(float _x = 0, float _y = 0) : PlaceChanger<T>(_x, _y)
		{
			this->sprite_ = &spr;
		}
	};

	class Solid :public Object
	{
	public:
		Solid(float _x = 0, float _y = 0) : Object(_x, _y) {}
	};

	class Spike :public Object
	{
	public:
		inline static Sprite sprUp{ "Image\\spike_u.png" };
		inline static Sprite sprDown{ "Image\\spike_d.png" };
		inline static Sprite sprLeft{ "Image\\spike_l.png" };
		inline static Sprite sprRight{ "Image\\spike_r.png" };
		Spike(float _x = 0, float _y = 0, Direction _dir = Direction::Up) : Object(_x, _y)
		{
			setPaintId(3);
			switch (_dir)
			{
			case Direction::Up:
				sprite_ = &sprUp;
				break;
			case Direction::Down:
				sprite_ = &sprDown;
				break;
			case Direction::Left:
				sprite_ = &sprLeft;
				break;
			case Direction::Right:
				sprite_ = &sprRight;
				break;
			default:
				sprite_ = &sprUp;
				break;
			}
		}
	};

	class Cherry :public Object
	{
	public:
		inline static Sprite spr{ 25,
			BitmapSpriteElement("Image\\cherry1.png", 10, 12),
			BitmapSpriteElement("Image\\cherry2.png", 10, 12) };
		Cherry(float _x = 0, float _y = 0) : Object(_x, _y)
		{
			sprite_ = &spr;
			setPaintId(3);
		}
	};

	class Platform :public Object
	{
	public:
		inline static Sprite spr{ BitmapSpriteElement("Image\\platform.png", 8, 16) };
		Platform(float _x = 0, float _y = 0) : Object(_x, _y)
		{
			sprite_ = &spr;
			setPaintId(2);
		}
	};

	class VineLeft :public Object
	{
	public:
		inline static Sprite spr{ BitmapSpriteElement("Image\\walljump_l.png", RectangleShape(14, 32, 18, 0)) };
		VineLeft(float _x = 0, float _y = 0) : Object(_x, _y)
		{
			sprite_ = &spr;
			setPaintId(3);
		}
	};

	class VineRight :public Object
	{
	public:
		inline static Sprite spr{ BitmapSpriteElement("Image\\walljump_r.png", RectangleShape(14, 32, 0, 0)) };
		VineRight(float _x = 0, float _y = 0) : Object(_x, _y)
		{
			sprite_ = &spr;
			setPaintId(3);
		}
	};

	class Water :public Object
	{
	public:
		inline static Sprite spr{ BitmapSpriteElement("Image\\water_noextrajump.png", SquareShape(32)) };
		Water(float _x = 0, float _y = 0) : Object(_x, _y)
		{
			sprite_ = &spr;
			setPaintId(4);
		}
	};

	class WaterExtraJump :public Object
	{
	public:
		inline static Sprite spr{ BitmapSpriteElement("Image\\water_extrajump.png", SquareShape(32)) };
		WaterExtraJump(float _x = 0, float _y = 0) : Object(_x, _y)
		{
			sprite_ = &spr;
			setPaintId(4);
		}
	};
}