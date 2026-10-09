#include "object.h"
#include "room.h"
#include "sprite.h"
#include <suku_draw/includes.h>

using namespace suku::maths;

namespace suku
{
	void Object::paintBody()const
	{
		if (!sprite_)
			return;
		SpriteElement* spr = getSpriteFrame();
		if (!spr)
			return;
		spr->paint(bRound(x - centerOffsetX), bRound(y - centerOffsetY), getTransform(), opacity);
	}

	void Object::paintBody(float _alpha) const
	{
		if (!sprite_)
			return;
		SpriteElement* spr = getSpriteFrame();
		if (!spr)
			return;
		spr->paint(bRound(x - centerOffsetX), bRound(y - centerOffsetY), getTransform(), _alpha);
	}

	void Object::paintBody(float _x, float _y)const
	{
		if (!sprite_)
			return;
		SpriteElement* spr = getSpriteFrame();
		if (!spr)
			return;
		spr->paint(bRound(_x - centerOffsetX), bRound(_y - centerOffsetY), getTransform(), opacity);
	}

	void Object::paintBody(float _x, float _y, float _alpha) const
	{
		if (!sprite_)
			return;
		SpriteElement* spr = getSpriteFrame();
		if (!spr)
			return;
		spr->paint(bRound(_x - centerOffsetX), bRound(_y - centerOffsetY), getTransform(), _alpha);
	}

	void Object::paintBody(float _x, float _y, Transform _spriteTransform) const
	{
		if (!sprite_)
			return;
		SpriteElement* spr = getSpriteFrame();
		if (!spr)
			return;
		spr->paint(bRound(_x - centerOffsetX), bRound(_y - centerOffsetY), _spriteTransform, opacity);
	}

	void Object::paintBody(float _x, float _y, Transform _spriteTransform, float _alpha) const
	{
		if (!sprite_)
			return;
		SpriteElement* spr = getSpriteFrame();
		if (!spr)
			return;
		spr->paint(bRound(_x - centerOffsetX), bRound(_y - centerOffsetY), _spriteTransform, _alpha);
	}

	void Object::paintBodySmooth()const
	{
		if (!sprite_)
			return;
		SpriteElement* spr = getSpriteFrame();
		if (!spr)
			return;
		spr->paint(bRound(x - centerOffsetX), bRound(y - centerOffsetY), getTransform(), opacity);
	}

	void Object::paintBodySmooth(float _alpha) const
	{
		if (!sprite_)
			return;
		SpriteElement* spr = getSpriteFrame();
		if (!spr)
			return;
		spr->paint(bRound(x - centerOffsetX), bRound(y - centerOffsetY), getTransform(), _alpha);
	}

	void Object::paintBodySmooth(float _x, float _y)const
	{
		if (!sprite_)
			return;
		SpriteElement* spr = getSpriteFrame();
		if (!spr)
			return;
		spr->paint(bRound(_x - centerOffsetX), bRound(_y - centerOffsetY), getTransform(), opacity);
	}

	void Object::paintBodySmooth(float _x, float _y, float _alpha) const
	{
		if (!sprite_)
			return;
		SpriteElement* spr = getSpriteFrame();
		if (!spr)
			return;
		spr->paint(bRound(_x - centerOffsetX), bRound(_y - centerOffsetY), getTransform(), _alpha);
	}

	void Object::paintBodySmooth(float _x, float _y, Transform _spriteTransform) const
	{
		if (!sprite_)
			return;
		SpriteElement* spr = getSpriteFrame();
		if (!spr)
			return;
		spr->paint(bRound(_x - centerOffsetX), bRound(_y - centerOffsetY), _spriteTransform, opacity);
	}

	void Object::paintBodySmooth(float _x, float _y, Transform _spriteTransform, float _alpha) const
	{
		if (!sprite_)
			return;
		SpriteElement* spr = getSpriteFrame();
		if (!spr)
			return;
		spr->paint(bRound(_x - centerOffsetX), bRound(_y - centerOffsetY), _spriteTransform, _alpha);
	}

	void Object::paintBody(Transform _spriteTransform) const
	{
		if (!sprite_)
			return;
		SpriteElement* spr = getSpriteFrame();
		if (!spr)
			return;
		spr->paint(_spriteTransform, opacity);
	}

	void Object::paintBody(Transform _spriteTransform, float _alpha) const
	{
		if (!sprite_)
			return;
		SpriteElement* spr = getSpriteFrame();
		if (!spr)
			return;
		spr->paint(_spriteTransform, _alpha);
	}

	void Object::updateFunction()
	{
		clock_++;
		for (auto iter = actionList_.begin(); iter != actionList_.end();)
		{
			if ((*iter)() == false)
				iter = actionList_.erase(iter);
			else iter++;
		}
	}

	SpriteElement* Object::getSpriteFrame()const
	{
		if (!sprite_)
			return nullptr;
		return sprite_->getFrameState(spriteBasicIndex, static_cast<long double>(clock_) * spriteAnimationRate);
	}

	UINT Object::getSpriteIndex() const
	{
		if (!sprite_)
			return 0;
		return sprite_->getFrameStateIndex(clock_);
	}

	double Object::getSpriteFlipTime() const
	{
		if (!sprite_)
			return 0.0;
		return getSpriteAnimationRate() * static_cast<double>(sprite_->getFlipTime());
	}

	void Object::setSpriteFlipTime(double _speed)
	{
		if (sprite_)
		{
			double animationRate = _speed / static_cast<double>(sprite_->getFlipTime());
			setSpriteAnimationRate(animationRate);
		}
	}

	void Object::setPreUpdateId(double _id)
	{
		if (isInRoom())
			inRoom_->setObjectRevisePriority(this, _id);
		else preUpdateId_ = _id;
	}

	void Object::setUpdateId(double _id)
	{
		if (isInRoom())
			inRoom_->setObjectUpdatePriority(this, _id);
		else updateId_ = _id;
	}

	void Object::setPostUpdateId(double _id)
	{
		if (isInRoom())
			inRoom_->setObjectRecheckPriority(this, _id);
		else postUpdateId_ = _id;
	}

	void Object::setPaintId(double _id)
	{
		if (isInRoom())
			inRoom_->setObjectPaintPriority(this, _id);
		else paintId_ = _id;
	}

	void Object::setTargetPaintLayer(PaintLayer& _layer)
	{
		targetPaintLayer_ = &_layer;
	}

	double Object::getWidth()
	{
		if (sprite_ == nullptr)
			return 0;
		return sprite_->getWidth() * (float)getScaleX();
	}

	double Object::getHeight()
	{
		if (sprite_ == nullptr)
			return 0;
		return sprite_->getHeight() * (float)getScaleY();
	}

	Vector Object::getSize()
	{
		if (sprite_ == nullptr)
			return Vector(0.0f, 0.0f);
		return Vector(sprite_->getWidth() * getScaleX(), sprite_->getHeight() * getScaleY());
	}

	Transform Object::getTransform() const
	{
		return scale(0.0f, 0.0f, xScale, yScale) + rotation(0.0f, 0.0f, angle) /*+ extraTransform_*/;
	}

	Object* Object::setPosition(real _x, real _y)
	{
		x = _x;
		y = _y;
		return this;
	}

	Object::Object(real _x, real _y, Room* _inRoom)
		: x(_x), y(_y), inRoom_(_inRoom),
		xLastFrame_(FLT_MAX), yLastFrame_(FLT_MAX),
		spawnX(_x), spawnY(_y) {}

	void Object::remove()
	{
		onRemove();
		if (isInRoom())
		{
			inRoom_->remove(this);
		}
		else
		{
			removeTag_ = true;
		}
	}

	void Object::destroy()
	{
		remove();
		destroyTag_ = true;
		// inRoom_ != nullptr 时其生命周期由 Room 管理
		if (!isInRoom())
		{
			delete this;
		}
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

	void Object::addAction(std::function<bool(Object*)> _actionFunc)
	{
		actionList_.push_back([=]() {
			return _actionFunc(this);
			});
	}

	void Object::addDelayAction(int _time, std::function<bool(Object*)> _actionFunc)
	{
		actionList_.push_back([=, count = 0]() mutable {
			if (count == _time)
			{
				return _actionFunc(this);
			}
			count++;
			return true;
			});
	}

	void Object::addTimelineAction(std::vector<std::pair<int, std::function<bool(Object*)>>> _actionVec)
	{
		for (auto& [time, action] : _actionVec)
		{
			addDelayAction(time, action);
		}
	}

	void Object::addTimelineAction(std::vector<int> _timeVec, std::vector<std::function<bool(Object*)>> _actionVec)
	{
		if (_timeVec.size() < _actionVec.size())
		{
			WARNINGWINDOW("Time vector and action vector don't match.");
			return;
		}
		for (int i = 0; i < _actionVec.size(); i++)
		{
			addDelayAction(_timeVec[i], _actionVec[i]);
		}
	}

	void Object::addTimelineAction(std::function<bool(Object*, int)> _actionFuncWithTime)
	{
		actionList_.push_back([=, count = 0]() mutable {
			count++;
			return _actionFuncWithTime(this, count);
			});
	}

	void Object::clearActions()
	{
		actionList_.clear();
	}

	void Object::movingTo(float _xTo, float _yTo, int _time)
	{
		float hspeed = ((_xTo - x) / (float)_time);
		float vspeed = ((_yTo - y) / (float)_time);
		actionList_.push_back([=, count = 0]() mutable {
			if (count == _time)
			{
				hspeedTemp = 0;
				vspeedTemp = 0;
				x = _xTo;
				y = _yTo;
				return false;
			}
			else
			{
				hspeedTemp = hspeed;
				vspeedTemp = vspeed;
				count++;
				return true;
			}
			});
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
		if (!_obj.getSpriteFrame() || !getSpriteFrame())
			return;
		if (_isPredict)
		{
			_obj.x += _obj.totalHspeed();
			_obj.y += _obj.totalVspeed();
		}
		float yTo;
		//yTo = _obj.y + _obj.getSpriteFrame()->hitY - getSpriteFrame()->hitHeight - getSpriteFrame()->hitY + y - round(y);

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
		if (!_obj.getSpriteFrame() || !getSpriteFrame())
			return;
		if (_isPredict)
		{
			_obj.x += _obj.totalHspeed();
			_obj.y += _obj.totalVspeed();
		}
		float yTo;
		//yTo = _obj.y + _obj.getSpriteFrame()->hitY + _obj.getSpriteFrame()->hitHeight - getSpriteFrame()->hitY + y - round(y) + 1;

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
		if (!_obj.getSpriteFrame() || !getSpriteFrame())
			return;
		if (_isPredict)
		{
			_obj.x += _obj.totalHspeed();
			_obj.y += _obj.totalVspeed();
		}
		float xTo;
		//xTo = _obj.x + _obj.getSpriteFrame()->hitX - getSpriteFrame()->hitWidth - getSpriteFrame()->hitX + x - round(x);

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
		if (!_obj.getSpriteFrame() || !getSpriteFrame())
			return;
		if (_isPredict)
		{
			_obj.x += _obj.totalHspeed();
			_obj.y += _obj.totalVspeed();
		}
		float xTo;
		//xTo = _obj.x + _obj.getSpriteFrame()->hitX + _obj.getSpriteFrame()->hitWidth - getSpriteFrame()->hitX + x - round(x);

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