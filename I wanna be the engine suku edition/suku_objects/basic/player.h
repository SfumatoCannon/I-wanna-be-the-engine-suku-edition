#pragma once

#include <suku_core/object.h>

namespace suku
{
	class Player;
	class Blood;

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

		virtual void onRoomEntering() override;
		virtual void onRestarting() override;
		virtual void preUpdate() override;
		virtual void update() override;
		virtual void postUpdate() override;

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

	class Blood :public Object
	{
	public:
		static Sprite spr;
		float gravity;
		Blood(float _x = 0, float _y = 0, float _wspeed = 0, float _hspeed = 0);

		virtual void preUpdate() override;
		virtual void update() override;
	};
}