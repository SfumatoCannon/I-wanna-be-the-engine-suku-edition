#pragma once
#include "suku_foundation/includes.h"
#include "suku_core/includes.h"
#include "suku_foundation/type_tree.h"
#include <suku_constants.h>

namespace suku
{
	class Background;
	class PlaceChanger;
	class Warp;
	class Wall;
	class Spike;
	class Cherry;
	class Platform;
	class VineLeft;
	class VineRight;
	class Trigger;
	class Water;
	class WaterExtraJump;

	class Background :public Object
	{
	public:
		Background() {}
		Background(const Bitmap& _bitmap);
		Background(const Background& _bg);
		Background(Background&& _bg)noexcept;
		virtual bool onPaint() override;
		Bitmap picture;
	};

	class Trigger :public Object
	{
	public:
		inline static Sprite spr{ BitmapSpriteElement(32, 32, SquareShape(32), 0, 0) };
		Trigger(float _x = 0, float _y = 0) : Object(_x, _y)
		{
			sprite_ = &spr;
		}
	};

	class PlaceChanger :public Object
	{
	public:
		Room* roomTo;
		PlaceChanger(float _x = 0, float _y = 0, Room* _roomTo = nullptr) : Object(_x, _y), roomTo(_roomTo)
		{
			sprite_ = &Trigger::spr;
		}
	};

	class Warp :public PlaceChanger
	{
	public:
		inline static Sprite spr{ BitmapSpriteElement("Image\\warp.png", 16, 16, 0.2f) };
		Warp(float _x = 0, float _y = 0, Room* _roomTo = nullptr) : PlaceChanger(_x, _y, _roomTo)
		{
			sprite_ = &spr;
		}
	};

	class Wall :public Object
	{
	public:
		inline static Sprite spr{ BitmapSpriteElement("Image\\wall.png", SquareShape(32), 16, 16) };
		Wall(float _x = 0, float _y = 0) : Object(_x, _y)
		{
			sprite_ = &spr;
			setPreUpdateId(-1);
			setUpdateId(-1);
			setPostUpdateId(-1);
			setPaintId(2);
		}
	};

	class Spike :public Object
	{
		SUKU_DERIVE(Spike, Wall);
	public:
		static Sprite sprUp;
		static Sprite sprDown;
		static Sprite sprLeft;
		static Sprite sprRight;
		Spike(float _x = 0, float _y = 0, Direction _dir = Direction::Up);
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