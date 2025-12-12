#pragma once
#include "suku_foundation/includes.h"
#include "suku_core/includes.h"

void loadFromJtoolMsg(suku::Room* _room, std::string _msg);

// .......................................................................................
// basic objects
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
	class Blood;
	class Player;

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
			setReviseStateId(-1);
			setUpdateStateId(-1);
			setRecheckStateId(-1);
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
		Spike(float _x = 0, float _y = 0, short _dir = 0);
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

	class Blood :public Object
	{
	public:
		static Sprite spr;
		float gravity;
		Blood(float _x = 0, float _y = 0, float _wspeed = 0, float _hspeed = 0);

		virtual void reviseState() override;
		virtual void updateState() override;
	};

	class Player :public Object
	{
	public:
		static Sprite sprStanding;
		static Sprite sprRunning;
		static Sprite sprJumping;
		static Sprite sprFalling;
		static Sprite sprSliding;

		bool isInfinityJump;
		bool isFrozen;
		short maxJumpTime;
		float movingSpeed;
		float gravity;

		Player(float _x = 0, float _y = 0);

		virtual void onAppearing() override;
		virtual void onRestarting() override;
		virtual void reviseState() override;
		virtual void updateState() override;
		virtual void recheckState() override;

		void moveLeft();
		void moveRight();
		void vineJumpLeft();
		void vineJumpRight();
		void startJump();
		void stopJump();
		void save();
		void spawn();
		bool isAlive();
	private:
		short nowBloodNum_;
		short jumpTime_;
		bool side_;
		bool isDied_;
		bool isOnPlatform_;
		bool isOnFloor_;
		bool isOnVineLeft_;
		bool isOnVineRight_;

		void die();
		void bleed();
	};

	// .......................................................................................
	// extensive objects

}