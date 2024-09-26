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
		sprite_ = &sprTrigger;
		var["warp_room"] << _roomTo;
	}

	Warp::Warp(float _x, float _y, Room* _roomTo) :PlaceChanger(_x, _y, _roomTo)
	{
		sprite_ = &sprWarp;
	}

	Wall::Wall(float _x, float _y) : Object(_x, _y)
	{
		tag_ = typecode(Wall);
		sprite_ = &sprWall;
		setReviseStateId(-1);
		setUpdateStateId(-1);
		setRecheckStateId(-1);
		setPaintId(2);
	}

	Spike::Spike(float _x, float _y, short _dir) :Object(_x, _y)
	{
		setPaintId(3);
		switch (_dir)
		{
		case DIR_UP:
			sprite_ = &sprSpikeUp;
			break;
		case DIR_DOWN:
			sprite_ = &sprSpikeDown;
			break;
		case DIR_LEFT:
			sprite_ = &sprSpikeLeft;
			break;
		case DIR_RIGHT:
			sprite_ = &sprSpikeRight;
			break;
		default:
			sprite_ = &sprSpikeUp;
			break;
		}
	}

	Cherry::Cherry(float _x, float _y) :Object(_x, _y)
	{
		sprite_ = &sprCherry;
		setPaintId(3);
	}

	Platform::Platform(float _x, float _y) :Object(_x, _y)
	{
		sprite_ = &sprPlatform;
		setPaintId(2);
	}

	WaterExtraJump::WaterExtraJump(float _x, float _y) :Object(_x, _y)
	{
		sprite_ = &sprWaterExtraJump;
		setPaintId(4);
	}

	Trigger::Trigger(float _x, float _y) :Object(_x, _y)
	{
		sprite_ = &sprTrigger;
	}

	VineLeft::VineLeft(float _x, float _y) :Object(_x, _y)
	{
		sprite_ = &sprVineLeft;
		setPaintId(3);
	}

	VineRight::VineRight(float _x, float _y) :Object(_x, _y)
	{
		sprite_ = &sprVineRight;
		setPaintId(3);
	}


	Blood::Blood(float _x, float _y, float _wspeed, float _hspeed) :Object(_x, _y)
	{
		setPaintId(1);
		hspeed = _wspeed;
		vspeed = _hspeed;
		sprite_ = &sprBlood;
		gravity = VALUE_G;
	}

	void Blood::reviseState()
	{
		if (vspeed == 0 && hspeed == 0)
			return;
		vspeed += gravity;
	}

	void Blood::updateState()
	{
		if (totalHspeed() == 0 && totalVspeed() == 0)
			return;
		/*if (!(touchObject(ID_WALL, x + totalHspeed(), y + totalVspeed())))
		{
			x += totalHspeed();
			y += totalVspeed();
		}
		else
		{
			Object* temp;
			x += totalHspeed();
			while (temp = touchObject(ID_WALL))
			{
				if (temp->nowState()->hitArea == nullptr && temp->angle == 0.0)
				{
					if (totalHspeed() > 0)
						x = temp->x + temp->nowState()->hitX - nowState()->hitWidth - nowState()->hitX;
					else
						x = temp->x + temp->nowState()->hitX + temp->nowState()->hitWidth - nowState()->hitX;
				}
				else x = x - (totalHspeed() > 0 ? 1.0 : -1.0);
			}
			y += totalVspeed();
			while (temp = touchObject(ID_WALL))
			{
				if (temp->nowState()->hitArea == nullptr && temp->angle == 0.0)
				{
					if (totalVspeed() > 0)
						y = temp->y + temp->nowState()->hitY - nowState()->hitHeight - nowState()->hitY;
					else
						y = temp->y + temp->nowState()->hitY + temp->nowState()->hitHeight - nowState()->hitY;
				}
				else y = y - (totalVspeed() > 0 ? 1.0 : -1.0);
				stopMoving();
			}
		}*/
	}

	Water::Water(float _x, float _y) :Object(_x, _y)
	{
		paintId_ = 4;
		sprite_ = &sprWater;
	}

	Player::Player(float _x, float _y) :Object(_x, _y)
	{
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
		memset(deathBlood_, 0, sizeof(deathBlood_));
		sprite_ = &sprPlayerIdle;
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

	void Player::setRoom(Room& _room)
	{
		//inRoom_->objectPointerArray.insert(inRoom_->kindEnd[ID_PLAYER], this);
		//inRoom_->player = this;
		//setPlaceAndSave(_room.playerStartX, _room.playerStartY);
		//for (int i = 0; i < PLAYER_BLOODNUMMAX; i++)
		//	deathBlood_[i] = inRoom_->findObj(Blood(), inRoom_->create(Blood()));
		spawn();
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
		//state_ |= STATE_DIED;
		alpha = 0.0;
		isDied_ = true;
	}

	void Player::bleed()
	{
		short i;
		i = 0;
		BitmapSpriteZ* body2 = nowState();
		if (!body2)
			return;
		while (nowBloodNum_ < PLAYER_BLOODNUMMAX)
		{
			i++;
			float bloodhspeed = modR((float)randD(), PLAYER_BLOODWOFFSET);
			float bloodvspeed = modR((float)randD(), PLAYER_BLOODHOFFSET);
			deathBlood_[nowBloodNum_]->x = x + body2->centerX;
			deathBlood_[nowBloodNum_]->y = y + body2->centerY;
			deathBlood_[nowBloodNum_]->hspeed = bloodhspeed;
			deathBlood_[nowBloodNum_]->vspeed = bloodvspeed;
			nowBloodNum_++;
			if (i >= PLAYER_BLOODNUMONCE)
				break;
		}
	}

	void Player::reviseState()
	{
		if (isKeyDown[VK_R])
		{
			//if (inRoom_)
			//	inRoom_->reset();
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
			sprite_ = &sprPlayerJumping;
		else if (!isOnFloor_ && totalVspeed() > (isOnPlatform_ ? 0.4 : 0))
		{
			if (isOnVineLeft_ || isOnVineRight_)
				sprite_ = &sprPlayerSliding;
			else sprite_ = &sprPlayerFalling;
		}
		else
		{
			if (isKeyHolding[VK_LEFT] || isKeyHolding[VK_RIGHT])
				sprite_ = &sprPlayerRunning;
			else
				sprite_ = &sprPlayerIdle;
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
		//for (auto& obj : tempList)
		//{
		//	if (isCrashed(*obj, x + totalHspeed(), y, obj->x + obj->totalHspeed(), obj->y))
		//	{
		//		if (totalHspeed() < obj->totalHspeed())
		//			contactToRight(*obj, true);
		//		else if (totalHspeed() > obj->totalHspeed())
		//			contactToLeft(*obj, true);
		//	}
		//	if (isCrashed(*obj, x, y + totalVspeed(), obj->x, obj->y + obj->totalVspeed()))
		//	{
		//		if (totalVspeed() < obj->totalVspeed())
		//		{
		//			vspeed = 0;
		//			contactToDown(*obj, true);
		//		}
		//		else if (totalVspeed() > obj->totalVspeed())
		//		{
		//			vspeed = 0;
		//			contactToUp(*obj, true);
		//		}
		//	}
		//}
		float vspeedBefore = totalVspeed();
		moveContactOld(tempList);
		if (vspeedBefore != totalVspeed())
		{
			vspeedTemp = totalVspeed();
			vspeed = 0;
		}
		if (getCrashedObject<Wall>(x + totalHspeed(), y + totalVspeed(), true))
			hspeed = hspeedTemp = 0;

		/*templist = touchObjectList(ID_BOX, x, y);
		for (auto i : templist)
		{
			//die();
			//return;
			if (i->totalVspeed() < 0)
				contactToUp(*i, true);
			else if (i->totalVspeed() > 0)
				contactToDown(*i, true);
		}
		if (touchObject(ID_WALL) || touchObject(ID_BOX))
		{
			die();
			return;
		}
		if (touchObject(ID_WALL, x + totalHspeed(), y + totalVspeed()) || touchObject(ID_BOX, x + totalHspeed(), y + totalVspeed()))
		{
			while (temp = touchObject(ID_WALL, x + totalHspeed(), y))
			{
				if (totalHspeed() > 0)
					contactToLeft(*temp);
				else if (totalHspeed() < 0)
					contactToRight(*temp);
				else break;
			}
			while (temp = touchObject(ID_BOX, x + totalHspeed(), y))
			{
				if (totalHspeed() > 0)
					contactToLeft(*temp);
				else if (totalHspeed() < 0)
					contactToRight(*temp);
				else break;
			}
			x += totalHspeed();
			if (touchObject(ID_WALL, x, y + totalVspeed()) || touchObject(ID_BOX, x, y + totalVspeed()))
			{
				float tvspeed = totalVspeed();
				while (temp = touchObject(ID_WALL, x, y + totalVspeed()))
				{
					if (totalVspeed() > 0)
						contactToUp(*temp);
					else if (totalVspeed() < 0)
						contactToDown(*temp);
					else break;
				}
				while (temp = touchObject(ID_BOX, x, y + totalVspeed()))
				{

					if (totalVspeed() > 0)
						contactToUp(*temp);
					else if (totalVspeed() < 0)
						contactToDown(*temp);
					else break;
				}
				//y += tvspeed;
				y += totalVspeed();
				stopMoving(false, true);
			}
			else y += totalVspeed();
		}
		else
		{
			x += totalHspeed();
			y += totalVspeed();
		}*/

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
		if (auto warp = getCrashedObject<PlaceChanger>())
		{
			if (warp->var["warp_room"].getValue<Room*>())
			{
				setRoom(*warp->var["warp_room"].getValue<Room*>());
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



	Box::Box(float _x, float _y) :Object(_x, _y)
	{
		paintId_ = 2;
		sprite_ = &sprBox;
		gravity = 0.4f;
	}

	void Box::pushLeft(float _xTo, Object* _nowobj)
	{
		/*std::list<Object*> objList;
		if (_nowobj == nullptr)
			_nowobj = this;
		_nowobj->hspeedTemp = min(_nowobj->totalHspeed(),
			_xTo - _nowobj->nowState()->hitX - _nowobj->nowState()->hitWidth - _nowobj->x);
		objList = _nowobj->touchObjectList(ID_BOX, _nowobj->x + _nowobj->totalHspeed(), _nowobj->y + _nowobj->totalVspeed());
		for (auto& i : objList)
		{
			if (i->x < _nowobj->x)
				pushLeft(_nowobj->x + _nowobj->nowState()->hitX + _nowobj->totalHspeed(), i);
		}*/
	}

	void Box::pushRight(float _xTo, Object* _nowobj)
	{
		//std::list<Object*> objList;
		//if (_nowobj == nullptr)
		//	_nowobj = this;
		//_nowobj->hspeedTemp = max(_nowobj->totalHspeed(),
		//	_xTo - _nowobj->nowState()->hitX - _nowobj->x);
		//objList = _nowobj->touchObjectList(ID_BOX, _nowobj->x + _nowobj->totalHspeed(), _nowobj->y /* + _nowobj->totalVspeed()*/);
		//for (auto& i : objList)
		//{
		//	if (i->x > _nowobj->x)
		//		pushRight(_nowobj->x + _nowobj->totalHspeed()
		//			+ _nowobj->nowState()->hitX + _nowobj->nowState()->hitWidth, i);
		//}
	}

	void Box::pushUp(float _yTo, Object* _nowobj)
	{
		//std::list<Object*> objList;
		//if (_nowobj == nullptr)
		//	_nowobj = this;
		//_nowobj->y = _yTo - _nowobj->nowState()->hitHeight - _nowobj->nowState()->hitY;
		//_nowobj->vspeed = 0;
		//objList = _nowobj->touchObjectList(ID_BOX);
		//for (auto& i : objList)
		//{
		//	if (i->y < _nowobj->y)
		//		pushUp(_nowobj->y + _nowobj->nowState()->hitY, i);
		//}
	}

	void Box::reviseState()
	{
		vspeed += gravity;
		/*if (!inRoom_->player)
			return;
		Object* player = inRoom_->player;
		if (!player)
			return;
		if (isCrashed(*player, x - player->totalHspeed(), y) &&
			!isCrashed(*player, x, y))
		{
			double playerHspeed = player->totalHspeed();
			Object* temp;
			if (playerHspeed > 0)
			{
				while (temp = player->touchObject(ID_WALL, player->x + playerHspeed, player->y))
				{
					if (temp->nowState()->hitArea == nullptr && temp->angle == 0.0)
						playerHspeed = round(temp->x) + temp->nowState()->hitX
						- player->nowState()->hitWidth - player->nowState()->hitX - round(player->x);
					else playerHspeed = playerHspeed - 1.0;
				}
				pushRight(player->x + playerHspeed + player->nowState()->hitX + player->nowState()->hitWidth);
			}
			else if (playerHspeed < 0)
			{
				while (temp = player->touchObject(ID_WALL, player->x + playerHspeed, player->y))
				{
					if (temp->nowState()->hitArea == nullptr && temp->angle == 0.0)
						playerHspeed = round(temp->x) + temp->nowState()->hitX + temp->nowState()->hitWidth
						- player->nowState()->hitX - round(player->x);
					else playerHspeed = playerHspeed + 1.0;
				}
				pushLeft(player->x + playerHspeed + player->nowState()->hitX);
			}
		}*/
	}

	void Box::updateState()
	{
		//Object* temp;
		/*if (touchObject(ID_WALL, x + totalHspeed(), y + totalVspeed()))
		{
			while (temp = touchObject(ID_WALL, x + totalHspeed(), y))
			{
				if (totalHspeed() > 0)
					contactToLeft(*temp);
				else if (totalHspeed() < 0)
					contactToRight(*temp);
				else break;
			}
			x += totalHspeed();
			while (temp = touchObject(ID_WALL, x, y + totalVspeed()))
			{
				if (totalVspeed() > 0)
					contactToUp(*temp);
				else if (totalVspeed() < 0)
					contactToDown(*temp);
				else break;
			}
			y += totalVspeed();
			pushUp(y + nowState()->hitY + nowState()->hitHeight);
		}
		else
		{
			x += totalHspeed();
			y += totalVspeed();
		}*/
	}
}