#include "player.h"
#include <suku_core/includes.h>
#include <suku_foundation/includes.h>
#include <suku_constants.h>
#include <suku_sounds/includes.h>
#include "common_objects.hpp"
#include <resources/suku_sound_pool.h>

using namespace suku::resource;

namespace suku
{
	Player::Player(float _x, float _y, Room* _room) :Object(_x, _y, _room)
	{
		isDied_ = false;
		isOnPlatform_ = false;
		isOnFloor_ = false;
		isOnVineLeft_ = isOnVineRight_ = false;
		isInfinityJump = false;
		maxJumpTime = 2;
		movingSpeed = 3.0;
		gravity = 0.4f;
		side_ = Direction::Right;
		jumpTime_ = maxJumpTime;
		nowBloodNum_ = 0;
		setPreUpdateId(0);
		setUpdateId(0);
		setPostUpdateId(0);
		setPaintId(3);
		setSavable(x, "player_x");
		setSavable(y, "player_y");
		spawnX = x;
		spawnY = y;
		sprite_ = &sprStanding;
	}

	void Player::onRoomStart()
	{
		SaveFile::getCurrent()->loadVar(x);
		SaveFile::getCurrent()->loadVar(y);
		hspeed = 0;
		vspeed = 0;
		opacity = 1.0;
		isDied_ = false;
		nowBloodNum_ = 0;
		clock_ = 0;
		jumpTime_ = maxJumpTime;
		spawnX = x;
		spawnY = y;
	}

	void Player::startJump()
	{
		eventJump_ = true;
		if (!isOnVineLeft_ && !isOnVineRight_)
		{
			if (jumpTime_ == maxJumpTime)
			{
				Audio::sndJump.play();
				vspeed = -8.5f;
				jumpTime_--;
			}
			else if (jumpTime_ != 0)
			{
				Audio::sndDoubleJump.play();
				vspeed = -7.0f;
				if (!getCrashedObject<WaterExtraJump>(x, y))
					jumpTime_--;
			}
			else if (getCrashedObject<Water>(x, y))
			{
				Audio::sndDoubleJump.play();
				vspeed = -7.0f;
			}
		}
		else // when press shift on the vine
		{
			if (jumpTime_ != 0)
			{
				jumpTime_--;
			}
		}
	}

	void Player::stopJump()
	{
		if (vspeed < 0)
			vspeed *= 0.45f;
	}


	void Player::moveLeft()
	{
		hspeedTemp -= movingSpeed;
	}

	void Player::moveRight()
	{
		hspeedTemp += movingSpeed;
	}

	void Player::vineJumpLeft()
	{
		eventVineJump_ = true;
		Audio::sndVineJump.play();
		vspeed = -9.0f;
		hspeedTemp = -movingSpeed * 5.0f;
	}

	void Player::vineJumpRight()
	{
		eventVineJump_ = true;
		Audio::sndVineJump.play();
		vspeed = -9.0f;
		hspeedTemp = movingSpeed * 5.0f;
	}

	void Player::shoot()
	{
		eventShoot_ = true;
		if (!isAlive())
			return;
		Audio::sndShoot.play();
		inRoom_->create<Bullet>(x, y - 4, side_);
	}

	void Player::die()
	{
		if (isDied_)
			return;
		setSpeed(0.0f, 0.0f, 0.0f, 0.0f);
		opacity = 0.0;
		isDied_ = true;
		Audio::sndDeath.play();
	}

	void Player::bleed()
	{
		short i;
		i = 0;
		if (!inRoom())
			return;
		SpriteElement* body2 = getSpriteFrame();
		if (!body2)
			return;
		while (nowBloodNum_ < 300)
		{
			i++;
			float bloodhspeed = randF(-5, 5);
			float bloodvspeed = randF(-5, 5);
			Blood* newBlood = inRoom()->create<Blood>(x, y).get();
			newBlood->setSpeed(bloodhspeed, bloodvspeed);
			newBlood->angle = randF(0, 360);
			newBlood->xScale = newBlood->yScale = randF(0.5f, 1.5f);
			nowBloodNum_++;
			if (i >= 10)
				break;
		}
	}

	void Player::onUpdate()
	{
		if (!isAlive())
			return;
		auto tempList = getCrashedObjectList<Solid>(x + totalHspeed(), y + totalVspeed(), true);

		float vspeedBefore = totalVspeed();
		for (auto& wall : tempList)
			moveContactOld(*wall);
		if (vspeedBefore != totalVspeed())
		{
			vspeedTemp = totalVspeed();
			vspeed = 0;
		}
		if (getCrashedObject<Solid>(x + totalHspeed(), y + totalVspeed(), true))
			hspeed = hspeedTemp = 0;
	}

	void Player::onUpdateEnd()
	{
		if (!isAlive())
		{
			bleed();
		}
		if (getCrashedObject<Spike>() || getCrashedObject<Cherry>())
		{
			die();
			return;
		}
		if (auto placeChanger = getCrashedObject<PlaceChangerRaw>())
		{
			placeChanger->warp();
		}
	}

	void Player::onSave()
	{
		SaveFile::getCurrent()->saveVar(x);
		SaveFile::getCurrent()->saveVar(y);
	}

	void Player::acquireState()
	{
		eventJump_ = false;
		eventVineJump_ = false;
		eventShoot_ = false;

		if (!isAlive())
			return;
		
		Vector gravityDirection = { 0, 1 };
		// floor
		if (getCrashedObject<Solid>(x + gravityDirection.x, y + gravityDirection.y))
		{
			isOnFloor_ = true;
			jumpTime_ = maxJumpTime;
		}
		else
		{
			isOnFloor_ = false;
			if (jumpTime_ == maxJumpTime)
				jumpTime_--;
		}

		// vine
		if (!isOnFloor_)
		{
			Vector leftDirection = { -1, 0 };
			Vector rightDirection = { 1, 0 };
			if (getCrashedObject<VineLeft>(x + leftDirection.x, y + leftDirection.y))
				isOnVineLeft_ = true;
			else
				isOnVineLeft_ = false;
			
			if (getCrashedObject<VineRight>(x + rightDirection.x, y + rightDirection.y))
				isOnVineRight_ = true;
			else
				isOnVineRight_ = false;
		}

		// platform
		if (isOnPlatform_ && !getCrashedObject<Platform>(x, y + 4.0f, false))
			isOnPlatform_ = false;
	}

	void Player::handleInput()
	{
		if (input::isKeyDown(VK_R))
		{
			if (inRoom_)
			{
				inRoom_->restart();
			}
		}

		if (!isAlive())
			return;

		if (input::isKeyDown(VK_Z))
			shoot();

		if (input::isKeyDown(VK_SHIFT))
			startJump();
		if (input::isKeyUp(VK_SHIFT))
			stopJump();

		if (isOnVineLeft_ && input::isKeyDown(VK_RIGHT_ARROW))
		{
			side_ = Direction::Right;
			if (input::isKeyHolding(VK_SHIFT))
			{
				vineJumpRight();
			}
			else
			{
				moveRight();
			}
		}
		else if (isOnVineRight_ && input::isKeyDown(VK_LEFT_ARROW))
		{
			side_ = Direction::Left;
			if (input::isKeyHolding(VK_SHIFT))
			{
				vineJumpLeft();
			}
			else
			{
				moveLeft();
			}
		}
		else if (isOnVineLeft_)
		{
			side_ = Direction::Right;
		}
		else if (isOnVineRight_)
		{
			side_ = Direction::Left;
		}

		if (input::isKeyHolding(VK_RIGHT_ARROW) && !isOnVineRight_ && !isOnVineLeft_)
		{
			side_ = Direction::Right;
			moveRight();
		}
		else if (input::isKeyHolding(VK_LEFT_ARROW) && !isOnVineLeft_ && !isOnVineRight_)
		{
			side_ = Direction::Left;
			moveLeft();
		}

		if (side_ == Direction::Left)
		{
			xScale = { -1, Transition(0) };
			extraTransform_ = translation(-1, 0); // align the collision box
		}
		else
		{
			xScale = { 1, Transition(0) };
			extraTransform_ = translation(0, 0); // align the collision box
		}
	}

	void Player::updateSpeed()
	{
		if (getCrashedObject<Water>(x, y) && vspeed > 2.0)
			vspeed = 2.0;

		if (getCrashedObject<WaterExtraJump>(x, y))
		{
			if (jumpTime_ != maxJumpTime)
				jumpTime_ = maxJumpTime - 1;
			if (vspeed > 2.0)
				vspeed = 2.0;
		}

		if ((isOnVineLeft_ || isOnVineRight_) && !eventVineJump_)
		{
			side_ = isOnVineLeft_ ? Direction::Right : Direction::Left;
			vspeed = 2.0;
		}

		vspeed += gravity;
		if (vspeed > 9.4f)
			vspeed = 9.4f;
	}

	void Player::updateSprite()
	{
		using namespace suku::input;

		if (totalVspeed() < 0 && (!isOnFloor_ || isKeyDown(VK_SHIFT)))
			sprite_ = &sprJumping;
		else if (!isOnFloor_ && totalVspeed() > (isOnPlatform_ ? 0.4 : 0))
		{
			if (isOnVineLeft_ || isOnVineRight_)
				sprite_ = &sprSliding;
			else sprite_ = &sprFalling;
		}
		else
		{
			if (isKeyHolding(VK_LEFT) || isKeyHolding(VK_RIGHT))
				sprite_ = &sprRunning;
			else
				sprite_ = &sprStanding;
		}
	}

	Blood::Blood(float _x, float _y, float _wspeed, float _hspeed) :Object(_x, _y)
	{
		setPaintId(4);
		hspeed = _wspeed;
		vspeed = _hspeed;
		sprite_ = &spr;
		gravity = 0.2f;
	}

	void Blood::onUpdateStart()
	{
		if (vspeed == 0 && hspeed == 0)
			return;
		vspeed += gravity;
	}

	void Blood::onUpdate()
	{
		auto tempList = getCrashedObjectList<Solid>(x + totalHspeed(), y + totalVspeed(), true);

		float vspeedBefore = totalVspeed();

		for (auto& wall : tempList)
			moveContactOld(*wall);

		if (vspeedBefore != totalVspeed())
		{
			vspeedTemp = totalVspeed();
			vspeed = 0;
		}
		if (getCrashedObject<Solid>(x + totalHspeed(), y + totalVspeed(), true))
			hspeed = hspeedTemp = 0;
	}

	void Bullet::onUpdateStart()
	{
		auto bulletBlocker = getCrashedObject<BulletBlocker>();
		if (bulletBlocker)
		{
			bulletBlocker->opacity = { 1, Transition(0) };
			bulletBlocker->opacity = { 0, Transition(20, TransitionCurve::easeOutCubic) };
			destroy();
		}
	}

	void Bullet::onUpdateEnd()
	{
		if (getCrashedObject<Solid>())
			destroy();
	}
}