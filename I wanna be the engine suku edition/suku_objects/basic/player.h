#pragma once

#include <suku_core/object.h>

namespace suku
{
	class Player;
	class Blood;
	class Bullet;

	class Player :public Object
	{
	public:
		inline static Sprite sprStanding{ "Image\\player_standing.png", 4, 5, RectangleShape(11, 21, 12, 11), 17, 23 };
		inline static Sprite sprRunning{ "Image\\player_running.png", 4, 2, RectangleShape(11, 21, 12, 11), 17, 23 };
		inline static Sprite sprJumping{ "Image\\player_jumping.png", 2, 4, RectangleShape(11, 21, 12, 11), 17, 23 };
		inline static Sprite sprFalling{ "Image\\player_falling.png", 2, 4, RectangleShape(11, 21, 12, 11), 17, 23 };
		inline static Sprite sprSliding{ "Image\\player_sliding.png", 2, 3, RectangleShape(11, 21, 12, 11), 17, 23 };

		bool isInfinityJump;
		bool isFrozen;
		short maxJumpTime;
		float movingSpeed;
		float gravity;

		Player(float _x = 0, float _y = 0, Room* _room = nullptr);

		virtual void onRoomStart() override;
		virtual void onUpdateStart() override;
		virtual void onUpdate() override;
		virtual void onUpdateEnd() override;
		virtual void onSave() override;

		void moveLeft();
		void moveRight();
		void vineJumpLeft();
		void vineJumpRight();
		void startJump();
		void stopJump();
		void shoot();

		bool isAlive();
	private:
		short nowBloodNum_;
		short jumpTime_;
		Direction side_;
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
		inline static Sprite spr{ "Image\\blood.png", SquareShape(2) };
		float gravity;
		Blood(float _x = 0, float _y = 0, float _wspeed = 0, float _hspeed = 0);

		virtual void onUpdateStart() override;
		virtual void onUpdate() override;
		virtual void onRoomRestart() override;
	};

	class Bullet :public Object
	{
	public:
		inline static Sprite spr{ "Image\\bullet.png", SquareShape(4) };
		Bullet(real _x, real _y, Direction _side) : Object(_x, _y)
		{
			sprite_ = &spr;
			if (_side == Direction::Left)
				hspeed = -16;
			else if (_side == Direction::Right)
				hspeed = 16;
			else if (_side == Direction::Up)
				vspeed = -16;
			else // _side == Direction::Down
				vspeed = 16;
		}
		virtual void onUpdateEnd() override;
	};
}