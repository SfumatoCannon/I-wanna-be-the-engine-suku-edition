#include "object_definer.h"

void loadFromJtoolMsg(suku::Room* room, std::string msg)
{
	using namespace suku;
	size_t length = msg.length();
	int value[3], now = 0;//0:x  1:y  2:kind
	int temp = 0;
	for (size_t i = 0; i < length; i++)
	{
		if (msg[i] == ' ')
		{
			value[now] = temp;
			temp = 0;
			if (now == 2)
			{
				switch (value[2])
				{
				case 1:
					room->create(Wall((float)value[0], (float)value[1]));
					break;
				case 3:
					room->create(Spike((float)value[0], (float)value[1], DIR_UP));
					break;
				case 4:
					room->create(Spike((float)value[0], (float)value[1], DIR_RIGHT));
					break;
				case 5:
					room->create(Spike((float)value[0], (float)value[1], DIR_LEFT));
					break;
				case 6:
					room->create(Spike((float)value[0], (float)value[1], DIR_DOWN));
					break;
				case 20:
					room->setPlayerStart((float)value[0], (float)value[1]);
					break;
				default:
					break;
				}
			}
			now = (now + 1) % 3;
		}
		else if (msg[i] >= '0' && msg[i] <= '9')
			temp = temp * 10 + msg[i] - '0';
	}
}

namespace suku
{
	PlaceChanger::PlaceChanger(float _x, float _y, Room* _roomTo) :Object(_x, _y)
	{
		sprite_ = Trigger::spr;
		roomTo = _roomTo;
	}

	Sprite* Warp::spr;
	Warp::Warp(float _x, float _y, Room* _roomTo) :PlaceChanger(_x, _y, _roomTo)
	{
		SPR_INIT(spr, (BitmapSpriteZero("Image\\warp.png", 16, 16, 0.2f)));
		sprite_ = spr;
	}

	Sprite* Wall::spr;
	Wall::Wall(float _x, float _y) : Object(_x, _y)
	{
		SPR_INIT(spr, (BitmapSpriteZero("Image\\wall.png", SquareShape(32), 16, 16)));
		//SPR_INIT(spr, ("Image\\player_standing.png", 4, 5, SquareShape(32)));
		sprite_ = spr;
		setReviseStateId(-1);
		setUpdateStateId(-1);
		setRecheckStateId(-1);
		setPaintId(2);
	}

	Sprite* Spike::sprUp;
	Sprite* Spike::sprDown;
	Sprite* Spike::sprLeft;
	Sprite* Spike::sprRight;
	Spike::Spike(float _x, float _y, short _dir) :Object(_x, _y)
	{
		SPR_INIT(sprUp, (BitmapSpriteZero("Image\\spike_u.png", 16, 16)));
		SPR_INIT(sprDown, (BitmapSpriteZero("Image\\spike_d.png", 16, 16)));
		SPR_INIT(sprLeft, (BitmapSpriteZero("Image\\spike_l.png", 16, 16)));
		SPR_INIT(sprRight, (BitmapSpriteZero("Image\\spike_r.png", 16, 16)));
		setPaintId(3);
		switch (_dir)
		{
		case DIR_UP:
			sprite_ = sprUp;
			break;
		case DIR_DOWN:
			sprite_ = sprDown;
			break;
		case DIR_LEFT:
			sprite_ = sprLeft;
			break;
		case DIR_RIGHT:
			sprite_ = sprRight;
			break;
		default:
			sprite_ = sprUp;
			break;
		}
	}

	Sprite* Cherry::spr;
	Cherry::Cherry(float _x, float _y) :Object(_x, _y)
	{
		SPR_INIT(spr, (25,
			BitmapSpriteZero("Image\\cherry1.png", 10, 12),
			BitmapSpriteZero("Image\\cherry2.png", 10, 12)));
		sprite_ = spr;
		setPaintId(3);
	}

	Sprite* Platform::spr;
	Platform::Platform(float _x, float _y) :Object(_x, _y)
	{
		SPR_INIT(spr, (BitmapSpriteZero("Image\\platform.png", 8, 16)));
		sprite_ = spr;
		setPaintId(2);
	}

	Sprite* Water::spr;
	Water::Water(float _x, float _y) :Object(_x, _y)
	{
		SPR_INIT(spr, (BitmapSpriteZero("Image\\water_noextrajump.png", SquareShape(32))));
		sprite_ = spr;
		paintId_ = 4;
	}

	Sprite* WaterExtraJump::spr;
	WaterExtraJump::WaterExtraJump(float _x, float _y) :Object(_x, _y)
	{
		SPR_INIT(spr, (BitmapSpriteZero("Image\\water_extrajump.png", SquareShape(32))));
		sprite_ = spr;
		setPaintId(4);
	}

	Sprite* Trigger::spr;
	Trigger::Trigger(float _x, float _y) :Object(_x, _y)
	{
		SPR_INIT(spr, (BitmapSpriteZero(32, 32, SquareShape(32), 0, 0)));
		sprite_ = spr;
	}

	Sprite* VineLeft::spr;
	VineLeft::VineLeft(float _x, float _y) :Object(_x, _y)
	{
		SPR_INIT(spr, (BitmapSpriteZero("Image\\walljump_l.png", RectangleShape(14, 32, 18, 0))));
		sprite_ = spr;
		setPaintId(3);
	}

	Sprite* VineRight::spr;
	VineRight::VineRight(float _x, float _y) :Object(_x, _y)
	{
		SPR_INIT(spr, (BitmapSpriteZero("Image\\walljump_r.png", RectangleShape(14, 32, 0, 0))));
		sprite_ = spr;
		setPaintId(3);
	}

	Sprite* Blood::spr;
	Blood::Blood(float _x, float _y, float _wspeed, float _hspeed) :Object(_x, _y)
	{
		SPR_INIT(spr, (BitmapSpriteZero("Image\\blood.png", SquareShape(2))));
		setPaintId(4);
		hspeed = _wspeed;
		vspeed = _hspeed;
		sprite_ = spr;
		gravity = VALUE_G;
	}

	void Blood::reviseState()
	{
		if (isKeyDown[VK_R])
			destroy();
		if (vspeed == 0 && hspeed == 0)
			return;
		vspeed += gravity;
	}

	void Blood::updateState()
	{
		auto tempList = getCrashedObjectList<Wall>(x + totalHspeed(), y + totalVspeed(), true);

		float vspeedBefore = totalVspeed();
		moveContactOld(tempList);
		if (vspeedBefore != totalVspeed())
		{
			vspeedTemp = totalVspeed();
			vspeed = 0;
		}
		if (getCrashedObject<Wall>(x + totalHspeed(), y + totalVspeed(), true))
			hspeed = hspeedTemp = 0;
	}

	Sprite* Player::sprStanding;
	Sprite* Player::sprRunning;
	Sprite* Player::sprJumping;
	Sprite* Player::sprFalling;
	Sprite* Player::sprSliding;
	Player::Player(float _x, float _y) :Object(_x, _y)
	{
		SPR_INIT(sprStanding, ("Image\\player_standing.png", 4, 5, RectangleShape(11, 21, 12, 11), 17, 23));
		SPR_INIT(sprRunning, ("Image\\player_running.png", 4, 2, RectangleShape(11, 21, 12, 11), 17, 23));
		SPR_INIT(sprJumping, ("Image\\player_jumping.png", 2, 4, RectangleShape(11, 21, 12, 11), 17, 23));
		SPR_INIT(sprFalling, ("Image\\player_falling.png", 2, 4, RectangleShape(11, 21, 12, 11), 17, 23));
		SPR_INIT(sprSliding, ("Image\\player_sliding.png", 2, 3, RectangleShape(11, 21, 12, 11), 17, 23));

		isDied_ = false;
		isOnPlatform_ = false;
		isOnFloor_ = false;
		isOnVineLeft_ = isOnVineRight_ = false;
		paintId_ = 3;
		isInfinityJump = false;
		isFrozen = false;
		maxJumpTime = 2;
		movingSpeed = PLAYER_MOVINGSPEED;
		gravity = VALUE_G;
		spawnX = _x;
		spawnY = _y;
		side_ = SIDE_RIGHT;
		jumpTime_ = maxJumpTime;
		nowBloodNum_ = 0;
		setReviseStateId(0);
		setUpdateStateId(0);
		setRecheckStateId(0);
		setSavable(x, "player_x");
		setSavable(y, "player_y");
		sprite_ = sprStanding;
	}

	void Player::onAppearing()
	{
		x = inRoom()->playerStartX;
		y = inRoom()->playerStartY;
		spawnX = x;
		spawnY = y;
		saveVar(x);
		saveVar(y);
	}

	void Player::onRestarting()
	{
		x = spawnX;
		y = spawnY;
	}

	void Player::startJump()
	{
		if (!isAlive())
			return;
		if (!isOnVineLeft_ && !isOnVineRight_)
		{
			if (jumpTime_ == maxJumpTime)
			{
				vspeed = -8.5f;
				jumpTime_--;
			}
			else if (jumpTime_ != 0)
			{
				vspeed = -7.0f;
				if (!getCrashedObject<WaterExtraJump>(x, y))
					jumpTime_--;
			}
			else if (getCrashedObject<Water>(x, y))
			{
				vspeed = -7.0f;
			}
		}
		else // when press shift on the vine
		{
			if (jumpTime_ != 0)
				jumpTime_--;
		}
	}

	void Player::stopJump()
	{
		if (vspeed < 0)
			vspeed *= 0.45f;
	}

	void Player::spawn()
	{
		x = spawnX;
		y = spawnY;
		hspeed = 0;
		vspeed = 0;
		alpha = 1.0;
		isDied_ = false;
		//for (int i = 0; i < PLAYER_BLOODNUMMAX; i++)
		//	deathBlood_[i]->remove();
		nowBloodNum_ = 0;
		clock_ = 0;
		jumpTime_ = maxJumpTime;
	}

	void Player::save()
	{
		if (!isAlive())
			return;
		spawnX = x;
		spawnY = y;
		//saveToFile();
	}

	bool Player::isAlive()
	{
		//return state_ & STATE_DIED ? 0 : 1;
		return !isDied_;
	}

	void Player::die()
	{
		setSpeed(0.0f, 0.0f, 0.0f, 0.0f);
		alpha = 0.0;
		isDied_ = true;
	}

	void Player::bleed()
	{
		short i;
		i = 0;
		if (!inRoom())
			return;
		SpriteZero* body2 = nowState();
		if (!body2)
			return;
		while (nowBloodNum_ < PLAYER_BLOODNUMMAX)
		{
			i++;
			float bloodhspeed = randF(-PLAYER_BLOODHOFFSET, PLAYER_BLOODHOFFSET);
			float bloodvspeed = randF(-PLAYER_BLOODVOFFSET, PLAYER_BLOODVOFFSET);
			inRoom()->create(Blood(centerX(), centerY()))->setSpeed(bloodhspeed, bloodvspeed);
			nowBloodNum_++;
			if (i >= PLAYER_BLOODNUMONCE)
				break;
		}
	}

	void Player::reviseState()
	{
		if (isKeyDown[VK_R])
		{
			spawn();
		}

		if (!isAlive())
		{
			bleed();
			return;
		}

		if (auto wall = getCrashedObject<Wall>(x, y + 1, false))
		{
			isOnFloor_ = true;
			jumpTime_ = maxJumpTime;
			vspeedTemp = wall->totalVspeed();
			if (wall->totalHspeed() != 0)
				hspeedTemp = wall->totalHspeed();
			if (wall->totalVspeed() > 0)
				vspeed = wall->totalVspeed();
		}
		else
		{
			isOnFloor_ = false;
			if (jumpTime_ == maxJumpTime)
				jumpTime_--;
		}

		if (!isOnFloor_ && getCrashedObject<VineLeft>(x - 1, y, false))
			isOnVineLeft_ = true;
		else
			isOnVineLeft_ = false;
		if (!isOnFloor_ && getCrashedObject<VineRight>(x + 1, y, false))
			isOnVineRight_ = true;
		else
			isOnVineRight_ = false;

		if (isOnPlatform_ && !getCrashedObject<Platform>(x, y + 4.0f, false))
			isOnPlatform_ = false;

		if (isOnVineLeft_ || isOnVineRight_)
		{
			side_ = isOnVineLeft_ ? SIDE_RIGHT : SIDE_LEFT;
			vspeed = 2.0;
		}

		if (getCrashedObject<Water>(x, y) && vspeed > 2.0)
			vspeed = 2.0;

		if (getCrashedObject<WaterExtraJump>(x, y))
		{
			if (jumpTime_ != maxJumpTime)
				jumpTime_ = maxJumpTime - 1;
			if (vspeed > 2.0)
				vspeed = 2.0;
		}

		if (!isFrozen)
		{
			if (isOnVineLeft_)
			{
				if (isKeyDown[VK_RIGHT] && isKeyHolding[VK_SHIFT])
					vineJumpRight();
				else if (isKeyHolding[VK_RIGHT])
					moveRight();
			}
			else if (isOnVineRight_)
			{
				if (isKeyDown[VK_LEFT] && isKeyHolding[VK_SHIFT])
					vineJumpLeft();
				else if (isKeyHolding[VK_LEFT])
					moveLeft();
			}
			else	// not on vine
			{
				if (isKeyHolding[VK_RIGHT])
				{
					side_ = SIDE_RIGHT;
					moveRight();
				}
				else if (isKeyHolding[VK_LEFT])
				{
					side_ = SIDE_LEFT;
					moveLeft();
				}
			}
			if (isKeyDown[VK_S])
				save();
			if (isKeyDown[VK_SHIFT])
				startJump();
			if (isKeyUp[VK_SHIFT])
				stopJump();
		}

		if (totalVspeed() < 0 && (!isOnFloor_ || isKeyDown[VK_SHIFT]))
			sprite_ = sprJumping;
		else if (!isOnFloor_ && totalVspeed() > (isOnPlatform_ ? 0.4 : 0))
		{
			if (isOnVineLeft_ || isOnVineRight_)
				sprite_ = sprSliding;
			else sprite_ = sprFalling;
		}
		else
		{
			if (isKeyHolding[VK_LEFT] || isKeyHolding[VK_RIGHT])
				sprite_ = sprRunning;
			else
				sprite_ = sprStanding;
		}

		//xScale = (side_ == 0 ? 1.0 : -1.0);
		spriteTransform = scale(PLAYER_CENTERX, PLAYER_CENTERY, (side_ == 0 ? 1.0f : -1.0f), 1);

		vspeed += gravity;
		if (vspeed > 9.4f)
			vspeed = 9.4f;
	}

	void Player::updateState()
	{
		if (!isAlive())
			return;
		auto tempList = getCrashedObjectList<Wall>(x + totalHspeed(), y + totalVspeed(), true);

		float vspeedBefore = totalVspeed();
		moveContactOld(tempList);
		if (vspeedBefore != totalVspeed())
		{
			vspeedTemp = totalVspeed();
			vspeed = 0;
		}
		if (getCrashedObject<Wall>(x + totalHspeed(), y + totalVspeed(), true))
			hspeed = hspeedTemp = 0;

		/*if (temp = touchObject(ID_PLATFORM))
		{
			if (y + nowState()->centerY - totalVspeed() / 2 <= temp->y)
			{
				if (temp->vspeed >= 0)
				{
					y = temp->y + temp->nowState()->hitY - nowState()->hitHeight - nowState()->hitY;
					vspeed = temp->vspeed;
				}
				jumpTime_ = maxJumpTime;
			}
			isOnPlatform_ = true;
		}*/
	}

	void Player::recheckState()
	{
		if (!isAlive())
			return;

		if (getCrashedObject<Spike>() || getCrashedObject<Cherry>())
		{
			die();
			return;
		}
		if (auto warp = getCrashedObjectPrecisely<Warp>())
		{
			if (warp->roomTo != nullptr)
			{
				gotoRoom(*warp->roomTo);
			}
		}
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
		vspeed = -9.0f;
		hspeedTemp = -movingSpeed * 5.0f;
	}

	void Player::vineJumpRight()
	{
		vspeed = -9.0f;
		hspeedTemp = movingSpeed * 5.0f;
	}

}