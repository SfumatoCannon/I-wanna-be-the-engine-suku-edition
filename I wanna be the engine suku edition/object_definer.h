#pragma once
#include "sprite_definer.hpp"
#include "game_saving.h"
#include "basis.h"

void loadFromJtoolMsg(suku::Room* _room, std::string _msg);

// .......................................................................................
// basic objects
namespace suku
{
	class PlaceChanger;
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

	class PlaceChanger :public Object
	{
	public:
		PlaceChanger(float _x = 0, float _y = 0, Room* _roomTo = nullptr);
	};

	class Warp :public PlaceChanger
	{
	public:
		Warp(float _x = 0, float _y = 0, Room* _roomTo = nullptr);
	};

	class Wall :public Object
	{
	public:
		Wall(float _x = 0, float _y = 0);
	};

	class Spike :public Object
	{
	public:
		Spike(float _x = 0, float _y = 0, short _dir = 0);
	};

	class Cherry :public Object
	{
	public:
		Cherry(float _x = 0, float _y = 0);
	};

	class Platform :public Object
	{
	public:
		Platform(float _x = 0, float _y = 0);
	};

	class VineLeft :public Object
	{
	public:
		VineLeft(float _x = 0, float _y = 0);
	};

	class VineRight :public Object
	{
	public:
		VineRight(float _x = 0, float _y = 0);
	};

	class Trigger :public Object
	{
	public:
		Trigger(float _x = 0, float _y = 0);
	};

	class Water :public Object
	{
	public:
		Water(float _x = 0, float _y = 0);
	};

	class WaterExtraJump :public Object
	{
	public:
		WaterExtraJump(float _x = 0, float _y = 0);
	};

	class Blood :public Object
	{
	public:
		float gravity;
		Blood(float _x = 0, float _y = 0, float _wspeed = 0, float _hspeed = 0);

		virtual void reviseState() override;
		virtual void updateState() override;
	};

	class Player :public Object
	{
	private:
		short nowBloodNum_;
		Object* deathBlood_[PLAYER_BLOODNUMMAX];
		short jumpTime_;
		bool side_;
		bool isDied_;
		bool isOnPlatform_;
		bool isOnFloor_;
		bool isOnVineLeft_;
		bool isOnVineRight_;

		void die();
		void bleed();
	public:
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
		void setRoom(Room& _room);
		bool isAlive();
	};

	// .......................................................................................
	// extensive objects

	class Box :public Object
	{
	public:
		float gravity;
		Box(float _x = 0, float _y = 0);

		void pushLeft(float _xTo, Object* _nowobj = nullptr);
		void pushRight(float _xTo, Object* _nowobj = nullptr);
		void pushUp(float _yTo, Object* _nowobj = nullptr);

		virtual void reviseState() override;
		virtual void updateState() override;
	};
}