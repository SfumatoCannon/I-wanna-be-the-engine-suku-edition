#include "object.h"
#include "room.h"
#include "sprite.h"
#include "../suku_draw/includes.h"

using namespace suku::maths;

namespace suku
{
	void Object::paintBody()
	{
		if (!sprite_)
			return;
		SpriteZero* spr = nowState();
		if (!spr)
			return;
		if (isClearPainting)
			spr->paint(bRound(x), bRound(y), spriteTransform, alpha);
		else spr->paint(x, y, spriteTransform, alpha);
	}

	void Object::paintBody(float _x, float _y)
	{
		if (!sprite_)
			return;
		SpriteZero* spr = nowState();
		if (!spr)
			return;
		if (isClearPainting)
			spr->paint(bRound(_x), bRound(_y), spriteTransform, alpha);
		else spr->paint(_x, _y, spriteTransform, alpha);
	}

	void Object::updateFunction()
	{
		clock_++;
		for (auto it = functionList_.begin(); it != functionList_.end();)
		{
			if ((*it)() == false)
				it = functionList_.erase(it++);
			else it++;
		}
	}

	void Object::onRestarting()
	{
		x = spawnX;
		y = spawnY;
	}

	SpriteZero* Object::nowState()const
	{
		if (!sprite_)
			return nullptr;
		return sprite_->getState(clock_);
	}

	void Object::setReviseStateId(double _id)
	{
		if (inRoom_)
		{
			auto& reviseStateArray = inRoom_->reviseStateArray;
			reviseStateArray[reviseStateId_].erase(reviseStateIterator_);
			reviseStateId_ = _id;
			reviseStateIterator_ =
				reviseStateArray[reviseStateId_].insert(reviseStateArray[reviseStateId_].end(), this);
		}
		else reviseStateId_ = _id;
	}

	void Object::setUpdateStateId(double _id)
	{
		if (inRoom_)
		{
			auto& updateStateArray = inRoom_->updateStateArray;
			updateStateArray[updateStateId_].erase(updateStateIterator_);
			updateStateId_ = _id;
			updateStateIterator_ =
				updateStateArray[updateStateId_].insert(updateStateArray[updateStateId_].end(), this);
		}
		else updateStateId_ = _id;
	}

	void Object::setRecheckStateId(double _id)
	{
		if (inRoom_)
		{
			auto& recheckStateArray = inRoom_->recheckStateArray;
			recheckStateArray[recheckStateId_].erase(recheckStateIterator_);
			recheckStateId_ = _id;
			recheckStateIterator_ =
				recheckStateArray[recheckStateId_].insert(recheckStateArray[recheckStateId_].end(), this);
		}
		else recheckStateId_ = _id;
	}

	void Object::setPaintId(double _id)
	{
		if (inRoom_)
		{
			auto& paintArray = inRoom_->paintArray;
			paintArray[paintId_].erase(paintIterator_);
			paintId_ = _id;
			paintIterator_ =
				paintArray[paintId_].insert(paintArray[paintId_].end(), this);
		}
		else paintId_ = _id;
	}

	float Object::centerX()
	{
		float cx = nowState()->centerX;
		float cy = nowState()->centerY;
		spriteTransform.transformPoint(&cx, &cy);
		return x + cx;
	}

	float Object::centerY()
	{
		float cx = nowState()->centerX;
		float cy = nowState()->centerY;
		spriteTransform.transformPoint(&cx, &cy);
		return y + cy;
	}

	Vector Object::center()
	{
		auto result = spriteTransform.transformPoint(nowState()->centerX, nowState()->centerY);
		result.first += x;
		result.second += y;
		return result;
	}

	double Object::getSpriteAngle()
	{
		float width = (float)nowState()->width;
		float height = (float)nowState()->height;
		auto [x, y] = spriteTransform.transformPoint(width, height);
		return atan((double)(width / height)) - atan((double)(x / y)) / PI * 180.0;
	}

	double Object::getSpriteXScale()
	{
		float width = (float)nowState()->width;
		float height = (float)nowState()->height;
		auto [x, y] = spriteTransform.transformPoint(width, height);
		return (double)x / (double)width;
	}

	double Object::getSpriteYScale()
	{
		float width = (float)nowState()->width;
		float height = (float)nowState()->height;
		auto [x, y] = spriteTransform.transformPoint(width, height);
		return (double)y / (double)height;
	}

	Var& Object::operator[](std::string _str)
	{
		return var[_str];
	}

	Object::Object(float _x, float _y)
	{
		var.clear();
		reviseStateId_ = 0;
		updateStateId_ = 0;
		recheckStateId_ = 0;
		destroyTag_ = false;
		paintId_ = 0;
		sprite_ = nullptr;
		inRoom_ = nullptr;
		clock_ = 0;
		x = _x;
		y = _y;
		var["xBefore"] = _x;
		var["yBefore"] = _y;
		xScale = 1.0;
		yScale = 1.0;
		isUpdating_ = false;
		isClearPainting = true;
		removeTag_ = false;
		spawnX = _x;
		spawnY = _y;
		alpha = 1.0;
		vspeed = hspeed = vspeedTemp = hspeedTemp = 0;
		spriteTransform = Transform();
		paintId_ = 0;
		kindId_ = typecode(Object);
	}

	void Object::remove()
	{
		onDestroy();
		if (inRoom_)
		{
			if (isUpdating_)
				removeTag_ = true;
			else
			{
				inRoom_->objectList<Object>()->erase(objectIterator_);
				inRoom_->objectParentPointerArray[kindId_].erase(objectParentIterator_);
				inRoom_->objectPointerRemoveArray[kindId_].push_back(insideObjectIterator_);
				//inRoom_->objectPointerArray[kindId_].erase(insideObjectIterator_);
				inRoom_->reviseStateArray[reviseStateId_].erase(reviseStateIterator_);
				inRoom_->updateStateArray[updateStateId_].erase(updateStateIterator_);
				inRoom_->recheckStateArray[recheckStateId_].erase(recheckStateIterator_);
				inRoom_->paintArray[paintId_].erase(paintIterator_);
				inRoom_ = nullptr;
			}
		}
	}

	void Object::destroy()
	{
		remove();
		if (inRoom_ && isUpdating_)
			destroyTag_ = true;
		else delete this;
	}

	float Object::totalHspeed()const
	{
		return hspeed + hspeedTemp;
	}

	float Object::totalVspeed()const
	{
		return vspeed + vspeedTemp;
	}

	void Object::setSpeed(float _hspeed, float _vspeed, float _hspeedTemp, float _vspeedTemp)
	{
		hspeed = _hspeed;
		vspeed = _vspeed;
		hspeedTemp = _hspeedTemp;
		vspeedTemp = _vspeedTemp;
	}

	void Object::setSpeedTemp(float _hspeedTemp, float _vspeedTemp)
	{
		hspeedTemp = _hspeedTemp;
		vspeedTemp = _vspeedTemp;
	}

	void Object::rotate(float _angle)
	{
		spriteTransform = spriteTransform + rotation(nowState()->centerX, nowState()->centerY, _angle);
	}

	void Object::rotate(float _angle, double _rotatingCenterX, double _rotatingCenterY, bool _isRotatingItself)
	{
		double centerX = x + nowState()->centerX * xScale, centerY = y + nowState()->centerY * yScale;
		rotateDot(_rotatingCenterX, _rotatingCenterY, _angle, &centerX, &centerY);
		x = (float)centerX - nowState()->centerX * xScale;
		y = (float)centerY - nowState()->centerY * yScale;
		if (_isRotatingItself)
			rotate(_angle);
	}

	void Object::rotateTo(float _angle, int _time)
	{
		functionList_.push_back([=]() {
			static int nowtime = _time;
			static float rotate_speed = _angle / _time;
			rotate(rotate_speed);
			nowtime--;
			if (nowtime == 0) return false;
			else return true;
			});
	}

	void Object::rotateTo(float _angle, double _rotatingCenterX, double _rotatingCenterY, int _time, bool _isRotatingItself)
	{
		functionList_.push_back([=]() {
			static int nowtime = _time;
			static float rotate_speed = _angle / _time;
			rotate(rotate_speed, _rotatingCenterX, _rotatingCenterY, _isRotatingItself);
			nowtime--;
			if (nowtime == 0) return false;
			else return true;
			});
	}

	void Object::setAction(std::function<bool(Object*)> _function)
	{
		functionList_.push_back([=]() {
			return _function(this);
			});
	}

	void Object::setDelayAction(int _time, std::function<void(Object*)> _function)
	{
		var["setDelayAction_nowtime"] << _time;
		functionList_.push_back([=]() {
			if (var["setDelayAction_nowtime"] == (int)0)
			{
				_function(this);
				return false;
			}
			var["setDelayAction_nowtime"] << (var["setDelayAction_nowtime"].getValue(int()) - 1);
			return true;
			});
	}

	void Object::movingTo(float _xTo, float _yTo, int _time)
	{
		var["movingTo_nowtime"] << _time;
		var["movingTo_hspeed"] << ((_xTo - x) / (float)_time);
		var["movingTo_vspeed"] << ((_yTo - y) / (float)_time);
		functionList_.push_back([=]() {
			hspeedTemp = var["movingTo_hspeed"].getValue<float>();
			vspeedTemp = var["movingTo_vspeed"].getValue<float>();
			var["movingTo_nowtime"] -= 1;
			if (var["movingTo_nowtime"] == 0) return false;
			else return true;
			});
	}

	bool Object::isCrashed(const Object& _obj)const
	{
		return nowState()->isCrashed(translation(bRound(x), bRound(y)) + spriteTransform, _obj.nowState(),
			translation(bRound(_obj.x), bRound(_obj.y)) + _obj.spriteTransform);
	}

	bool Object::isCrashed(const Object& _obj, float _x, float _y)const
	{
		return nowState()->isCrashed(translation(bRound(_x), bRound(_y)) + spriteTransform, _obj.nowState(),
			translation(bRound(_obj.x), bRound(_obj.y)) + _obj.spriteTransform);
	}

	bool Object::isCrashed(const Object& _obj, float _x, float _y, float _objX, float _objY)const
	{
		return nowState()->isCrashed(translation(bRound(_x), bRound(_y)) + spriteTransform, _obj.nowState(),
			translation(bRound(_objX), bRound(_objY)) + _obj.spriteTransform);
	}

	bool Object::isCrashed(const std::list<Object*>& _objList) const
	{
		for (auto& i : _objList)
		{
			if (isCrashed(*i))
				return true;
		}
		return false;
	}

	bool Object::isCrashed(const std::list<Object*>& _objList, float _x, float _y) const
	{
		for (auto& i : _objList)
		{
			if (isCrashed(*i, _x, _y))
				return true;
		}
		return false;
	}

	void Object::moveContactOld(const Object& _obj, bool _isPredict)
	{
		if (!isCrashed(_obj, x + totalHspeed(), y))
		{
			if (isCrashed(_obj, x, y + totalVspeed()))
			{
				auto [newHspeed1, newVspeed1] = getContactSpeed(_obj, 0, totalVspeed());
				vspeedTemp = newVspeed1 - vspeed;
			}
			if (isCrashed(_obj, x + totalHspeed(), y + totalVspeed()))
			{
				auto [newHspeed2, newVspeed2] = getContactSpeed(_obj, totalHspeed(), 0, x, y + totalVspeed());
				hspeedTemp = newHspeed2 - hspeed;
			}
		}
		else
		{
			auto [newHspeed1, newVspeed1] = getContactSpeed(_obj, totalHspeed(), 0);
			hspeedTemp = newHspeed1 - hspeed;
			if (isCrashed(_obj, x + totalHspeed(), y + totalVspeed()))
			{
				auto [newHspeed2, newVspeed2] = getContactSpeed(_obj, 0, totalVspeed(), x + totalHspeed(), y);
				vspeedTemp = newVspeed2 - vspeed;
			}
		}
	}

	void Object::moveContactOld(const std::list<Object*>& _objList, bool _isPredict)
	{
		if (!isCrashed(_objList, x + totalHspeed(), y))
		{
			if (isCrashed(_objList, x, y + totalVspeed()))
			{
				auto [newHspeed1, newVspeed1] = getContactSpeed(_objList, 0, totalVspeed());
				vspeedTemp = newVspeed1 - vspeed;
			}
			if (isCrashed(_objList, x + totalHspeed(), y + totalVspeed()))
			{
				auto [newHspeed2, newVspeed2] = getContactSpeed(_objList, totalHspeed(), 0, x, y + totalVspeed());
				hspeedTemp = newHspeed2 - hspeed;
			}
		}
		else
		{
			auto [newHspeed1, newVspeed1] = getContactSpeed(_objList, totalHspeed(), 0);
			hspeedTemp = newHspeed1 - hspeed;
			if (isCrashed(_objList, x + totalHspeed(), y + totalVspeed()))
			{
				auto [newHspeed2, newVspeed2] = getContactSpeed(_objList, 0, totalVspeed(), x + totalHspeed(), y);
				vspeedTemp = newVspeed2 - vspeed;
			}
		}
	}

	void Object::moveContactNew(const Object& _obj, bool _isPredict)
	{
		auto [newHspeed, newVspeed] = getContactSpeed(_obj, totalHspeed(), totalVspeed());
		hspeedTemp = newHspeed - hspeed;
		vspeedTemp = newVspeed - vspeed;
	}

	void Object::moveContactNew(const std::list<Object*>& _objList, bool _isPredict)
	{
		auto [newHspeed, newVspeed] = getContactSpeed(_objList, totalHspeed(), totalVspeed());
		hspeedTemp = newHspeed - hspeed;
		vspeedTemp = newVspeed - vspeed;
	}

	Vector Object::getContactSpeed(const Object& _obj, float _hspeed, float _vspeed)
	{
		float cosA, sinA;
		if (_hspeed == 0)
		{
			cosA = 0;
			sinA = _vspeed > 0.0f ? 1.0f : -1.0f;
		}
		else if (_vspeed == 0)
		{
			cosA = _hspeed > 0.0f ? 1.0f : -1.0f;
			sinA = 0;
		}
		else
		{
			cosA = _hspeed / sqrt(_hspeed * _hspeed + _vspeed * _vspeed);
			sinA = _vspeed / sqrt(_hspeed * _hspeed + _vspeed * _vspeed);
		}
		int lSpeed = 0, rSpeed = (int)sqrt(_vspeed * _vspeed + _hspeed * _hspeed) + 1;
		int mid;
		while (lSpeed < rSpeed)
		{
			mid = (lSpeed + rSpeed + 1) >> 1;
			if (isCrashed(_obj, x + mid * cosA, y + mid * (float)sinA))
				rSpeed = mid - 1;
			else lSpeed = mid;
		}
		return { lSpeed * cosA, lSpeed * sinA };
	}

	Vector Object::getContactSpeed(const Object& _obj, float _hspeed, float _vspeed, float _x, float _y)
	{
		float cosA, sinA;
		if (_hspeed == 0)
		{
			cosA = 0;
			sinA = _vspeed > 0.0f ? 1.0f : -1.0f;
		}
		else if (_vspeed == 0)
		{
			cosA = _hspeed > 0.0f ? 1.0f : -1.0f;
			sinA = 0;
		}
		else
		{
			cosA = _hspeed / sqrt(_hspeed * _hspeed + _vspeed * _vspeed);
			sinA = _vspeed / sqrt(_hspeed * _hspeed + _vspeed * _vspeed);
		}
		int lSpeed = 0, rSpeed = (int)sqrt(_vspeed * _vspeed + _hspeed * _hspeed) + 1;
		int mid;
		while (lSpeed < rSpeed)
		{
			mid = (lSpeed + rSpeed + 1) >> 1;
			if (isCrashed(_obj, _x + mid * cosA, _y + mid * sinA))
				rSpeed = mid - 1;
			else lSpeed = mid;
		}
		return { lSpeed * cosA, lSpeed * sinA };
	}

	Vector Object::getContactSpeed(const std::list<Object*>& _objList, float _hspeed, float _vspeed)
	{
		float cosA, sinA;
		if (_hspeed == 0)
		{
			cosA = 0;
			sinA = _vspeed > 0.0f ? 1.0f : -1.0f;
		}
		else if (_vspeed == 0)
		{
			cosA = _hspeed > 0.0f ? 1.0f : -1.0f;
			sinA = 0;
		}
		else
		{
			cosA = _hspeed / sqrt(_hspeed * _hspeed + _vspeed * _vspeed);
			sinA = _vspeed / sqrt(_hspeed * _hspeed + _vspeed * _vspeed);
		}
		int lSpeed = 0, rSpeed = (int)sqrt(_vspeed * _vspeed + _hspeed * _hspeed) + 1;
		int mid;
		while (lSpeed < rSpeed)
		{
			mid = (lSpeed + rSpeed + 1) >> 1;
			if (isCrashed(_objList, x + mid * cosA, y + mid * (float)sinA))
				rSpeed = mid - 1;
			else lSpeed = mid;
		}
		return { lSpeed * cosA, lSpeed * sinA };
	}

	Vector Object::getContactSpeed(const std::list<Object*>& _objList, float _hspeed, float _vspeed, float _x, float _y)
	{
		float cosA, sinA;
		if (_hspeed == 0)
		{
			cosA = 0;
			sinA = _vspeed > 0.0f ? 1.0f : -1.0f;
		}
		else if (_vspeed == 0)
		{
			cosA = _hspeed > 0.0f ? 1.0f : -1.0f;
			sinA = 0;
		}
		else
		{
			cosA = _hspeed / sqrt(_hspeed * _hspeed + _vspeed * _vspeed);
			sinA = _vspeed / sqrt(_hspeed * _hspeed + _vspeed * _vspeed);
		}
		int lSpeed = 0, rSpeed = (int)sqrt(_vspeed * _vspeed + _hspeed * _hspeed) + 1;
		int mid;
		while (lSpeed < rSpeed)
		{
			mid = (lSpeed + rSpeed + 1) >> 1;
			if (isCrashed(_objList, _x + mid * cosA, _y + mid * (float)sinA))
				rSpeed = mid - 1;
			else lSpeed = mid;
		}
		return { lSpeed * cosA, lSpeed * sinA };
	}

	void Object::contactToUp(Object _obj, bool _isPredict, bool _isMoveDirectly)
	{
		if (!_obj.nowState() || !nowState())
			return;
		if (_isPredict)
		{
			_obj.x += _obj.totalHspeed();
			_obj.y += _obj.totalVspeed();
		}
		float yTo;
		//yTo = _obj.y + _obj.nowState()->hitY - nowState()->hitHeight - nowState()->hitY + y - round(y);

		float yBefore = y;
		while (!isCrashed(_obj))
			y++;
		yTo = y - 1;
		y = yBefore;

		if (_isMoveDirectly)
			y = yTo;
		else vspeedTemp = yTo - y - vspeed;
	}

	void Object::contactToDown(Object _obj, bool _isPredict, bool _isMoveDirectly)
	{
		if (!_obj.nowState() || !nowState())
			return;
		if (_isPredict)
		{
			_obj.x += _obj.totalHspeed();
			_obj.y += _obj.totalVspeed();
		}
		float yTo;
		//yTo = _obj.y + _obj.nowState()->hitY + _obj.nowState()->hitHeight - nowState()->hitY + y - round(y) + 1;

		float yBefore = y;
		while (!isCrashed(_obj))
			y--;
		yTo = y + 1;
		y = yBefore;

		if (_isMoveDirectly)
			y = yTo;
		else vspeedTemp = yTo - y - vspeed;
	}

	void Object::contactToLeft(Object _obj, bool _isPredict, bool _isMoveDirectly)
	{
		if (!_obj.nowState() || !nowState())
			return;
		if (_isPredict)
		{
			_obj.x += _obj.totalHspeed();
			_obj.y += _obj.totalVspeed();
		}
		float xTo;
		//xTo = _obj.x + _obj.nowState()->hitX - nowState()->hitWidth - nowState()->hitX + x - round(x);

		float xBefore = x;
		while (!isCrashed(_obj))
			x++;
		xTo = x - 1;
		x = xBefore;

		if (_isMoveDirectly)
			x = xTo;
		else hspeedTemp = xTo - x - hspeed;
	}

	void Object::contactToRight(Object _obj, bool _isPredict, bool _isMoveDirectly)
	{
		if (!_obj.nowState() || !nowState())
			return;
		if (_isPredict)
		{
			_obj.x += _obj.totalHspeed();
			_obj.y += _obj.totalVspeed();
		}
		float xTo;
		//xTo = _obj.x + _obj.nowState()->hitX + _obj.nowState()->hitWidth - nowState()->hitX + x - round(x);

		float xBefore = x;
		while (!isCrashed(_obj))
			x--;
		xTo = x + 1;
		x = xBefore;

		if (_isMoveDirectly)
			x = xTo;
		else hspeedTemp = xTo - x - hspeed;
	}

	void Object::moveContact(Object _obj, bool _isPredict, bool _isMoveDirectly)
	{
		if (!_isPredict)
		{
			_obj.vspeed = _obj.vspeedTemp = 0;
			_obj.hspeed = _obj.hspeedTemp = 0;
		}

		if (!isCrashed(_obj, x + totalHspeed(), y + totalVspeed()))
		{
			if (_isMoveDirectly)
			{
				x = x + totalHspeed();
				y = y + totalVspeed();
			}
			return;
		}

		if (isCrashed(_obj, x + totalHspeed(), y, _obj.x + _obj.totalHspeed(), _obj.y))
		{
			if (totalHspeed() > _obj.totalHspeed())
				contactToLeft(_obj, _isMoveDirectly);
			else contactToRight(_obj, _isMoveDirectly);
		}
		else
		{
			float xTo = x + totalHspeed();
			if (_isMoveDirectly)
				x = xTo;
			else hspeedTemp = xTo - round(x) - hspeed;
		}
		if (isCrashed(_obj, x, y + totalVspeed(), _obj.x, _obj.y + _obj.totalVspeed()))
		{
			if (totalVspeed() > _obj.totalVspeed())
				contactToUp(_obj, _isMoveDirectly);
			else contactToDown(_obj, _isMoveDirectly);
		}
		else
		{
			float yTo = y + totalVspeed();
			if (_isMoveDirectly)
				y = yTo;
			else vspeedTemp = yTo - round(y) - vspeed;
		}
	}

	/*
	Object* Object::touchObject(int _objectkind)
	{
		if (inRoom_)
		{
			if (!sprite_) return nullptr;
			BitmapSpriteZero* body2 = nowState();
			if (!body2)
				return nullptr;
			for (auto i = inRoom_->kindStart[_objectkind]; i != inRoom_->kindEnd[_objectkind]; i++)
				if (isCrashed(*(*i)))
					return *i;
		}
		return nullptr;
	}

	Object* Object::touchObject(int _objectkind, double _x, double _y)
	{
		double tx = x, ty = y;
		x = _x, y = _y;
		if (inRoom_)
		{
			if (!sprite_) return nullptr;
			BitmapSpriteZero* body2 = nowState();
			if (!body2)
			{
				x = tx;
				y = ty;
				return nullptr;
			}
			for (auto i = inRoom_->kindStart[_objectkind]; i != inRoom_->kindEnd[_objectkind]; i++)
				if (isCrashed(*(*i)))
				{
					x = tx;
					y = ty;
					return *i;
				}
		}
		x = tx, y = ty;
		return nullptr;
	}

	Object* Object::nearbyObject(int _objectkind, double _dis)
	{
		if (inRoom_)
		{
			if (!sprite_) return nullptr;
			BitmapSpriteZero* body2 = nowState();
			if (!body2)
				return nullptr;
			for (auto i = inRoom_->kindStart[_objectkind]; i != inRoom_->kindEnd[_objectkind]; i++)
				if (distanceToObject(*(*i)) <= _dis)
					return *i;
		}
		return nullptr;
	}

	std::list<Object*> Object::touchObjectList(int _objectkind)
	{
		std::list<Object*> result;
		if (inRoom_)
		{
			if (!sprite_) return result;
			BitmapSpriteZero* body2 = nowState();
			if (!body2)
				return result;
			for (auto i = inRoom_->kindStart[_objectkind]; i != inRoom_->kindEnd[_objectkind]; i++)
				if ( isCrashed(*(*i)))
					result.push_back(*i);
		}
		return result;
	}

	std::list<Object*> Object::touchObjectList(int _objectkind, double _x, double _y)
	{
		std::list<Object*> result;
		double tx = x, ty = y;
		x = _x, y = _y;
		if (inRoom_)
		{
			if (!sprite_)
			{
				x = tx, y = ty;
				return result;
			}
			BitmapSpriteZero* body2 = nowState();
			if (!body2)
				return result;
			for (auto i = inRoom_->kindStart[_objectkind]; i != inRoom_->kindEnd[_objectkind]; i++)
				if ( isCrashed(*(*i)))
					result.push_back(*i);
		}
		x = tx, y = ty;
		return result;
	}

	std::list<Object*> Object::nearbyObjectList(int _objectkind, double _dis)
	{
		std::list<Object*> result;
		if (inRoom_)
		{
			if (!sprite_) return result;
			BitmapSpriteZero* body2 = nowState();
			if (!body2)
				return result;
			for (auto i = inRoom_->kindStart[_objectkind]; i != inRoom_->kindEnd[_objectkind]; i++)
				if (distanceToObject(*(*i)) <= _dis)
					result.push_back(*i);
		}
		return result;
	}*/

	void Object::save()
	{
		spawnX = x;
		spawnY = y;
	}

	void Object::spawn()
	{
		x = spawnX;
		y = spawnY;
		hspeed = 0;
		vspeed = 0;
		clock_ = 0;
	}
}