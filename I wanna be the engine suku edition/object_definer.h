#pragma once
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
		Room* roomTo;
		PlaceChanger(float _x = 0, float _y = 0, Room* _roomTo = nullptr);
	};

	class Warp :public PlaceChanger
	{
	public:
		static Sprite* spr;
		Warp(float _x = 0, float _y = 0, Room* _roomTo = nullptr);
	};

	class Wall :public Object
	{
	public:
		static Sprite* spr;
		Wall(float _x = 0, float _y = 0);
	};

	class Spike :public Object
	{
	public:
		static Sprite* sprUp;
		static Sprite* sprDown;
		static Sprite* sprLeft;
		static Sprite* sprRight;
		Spike(float _x = 0, float _y = 0, short _dir = 0);
	};

	class Cherry :public Object
	{
	public:
		static Sprite* spr;
		Cherry(float _x = 0, float _y = 0);
	};

	class Platform :public Object
	{
	public:
		static Sprite* spr;
		Platform(float _x = 0, float _y = 0);
	};

	class VineLeft :public Object
	{
	public:
		static Sprite* spr;
		VineLeft(float _x = 0, float _y = 0);
	};

	class VineRight :public Object
	{
	public:
		static Sprite* spr;
		VineRight(float _x = 0, float _y = 0);
	};

	class Trigger :public Object
	{
	public:
		static Sprite* spr;
		Trigger(float _x = 0, float _y = 0);
	};

	class Water :public Object
	{
	public:
		static Sprite* spr;
		Water(float _x = 0, float _y = 0);
	};

	class WaterExtraJump :public Object
	{
	public:
		static Sprite* spr;
		WaterExtraJump(float _x = 0, float _y = 0);
	};

	class Blood :public Object
	{
	public:
		static Sprite* spr;
		float gravity;
		Blood(float _x = 0, float _y = 0, float _wspeed = 0, float _hspeed = 0);

		virtual void reviseState() override;
		//virtual void updateState() override;
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
		static Sprite* sprStanding;
		static Sprite* sprRunning;
		static Sprite* sprJumping;
		static Sprite* sprFalling;
		static Sprite* sprSliding;


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
	};

	// .......................................................................................
	// extensive objects

}