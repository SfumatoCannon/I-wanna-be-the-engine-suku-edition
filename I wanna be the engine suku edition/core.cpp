#include "core.h"

namespace suku
{
	Room* nowRoom;
	TypeTree collisionInheritTree;

	std::vector<std::pair<size_t, int>> reviseStateArray;
	std::vector<std::pair<size_t, int>> updateStateArray;
	std::vector<std::pair<size_t, int>> recheckStateArray;


	BitmapCollisionBox::BitmapCollisionBox(bool** _hitArea, int _hitX, int _hitY, UINT _hitWidth, UINT _hitHeight)
	{
		hitArea = _hitArea;
		hitX = _hitX, hitY = _hitY;
		hitWidth = _hitWidth, hitHeight = _hitHeight;
	}

	BitmapCollisionBox::BitmapCollisionBox(IWICBitmap* _bitmap, float _alphaThreshold)
	{
		auto [width, height] = getBitmapSize(_bitmap);
		hitWidth = width, hitHeight = height;
		hitX = hitY = 0;
		hitArea = malloc2D<bool>(hitWidth, hitHeight);
		getHitAreaFromBitmap(hitArea, Bitmap(_bitmap), _alphaThreshold);
	}

	BitmapCollisionBox::BitmapCollisionBox(Bitmap* _pBitmap, float _alphaThreshold)
	{
		if (_pBitmap == nullptr)
		{
			hitWidth = hitHeight = 0;
			hitX = hitY = 0;
			hitArea = nullptr;
			return;
		}
		hitWidth = _pBitmap->getWidth();
		hitHeight = _pBitmap->getHeight();
		hitX = hitY = 0;
		hitArea = malloc2D<bool>(hitWidth, hitHeight);
		getHitAreaFromBitmap(hitArea, *_pBitmap, _alphaThreshold);
	}

	BitmapCollisionBox::~BitmapCollisionBox()
	{
		free2D(hitArea, hitWidth, hitHeight);
	}

	bool BitmapCollisionBox::isCrashed(Transform _transform, const BitmapCollisionBox& _other, Transform _otherTransform)const
	{
		if (hitArea == nullptr || _other.hitArea == nullptr)
			return false;
		UINT i, j;
		float px, py;
		Transform targetTransform = _transform + _otherTransform.invertTransform();
		for (i = 0; i < hitWidth; i++)
			for (j = 0; j < hitHeight; j++)
			{
				if (hitArea[i][j])
				{
					px = (float)(hitX + i);
					py = (float)(hitY + j);
					targetTransform.transformPoint(&px, &py);
					px -= (float)(_other.hitX);
					py -= (float)(_other.hitY);
					px = round(px);
					py = round(py);
					if (*(_other.hitArea + (int)py * _other.hitWidth + (int)px))
						return true;
				}
			}
		return false;
	}

	bool BitmapCollisionBox::isCrashed(Transform _transform, const ShapeCollisionBox& _other, Transform _otherTransform)const
	{
		if (hitArea == nullptr)
			return false;
		static Shape pixelShape = SquareShape(1, 0, 0);
		D2D1_GEOMETRY_RELATION result;
		UINT i, j;
		for (i = 0; i < hitWidth; i++)
			for (j = 0; j < hitHeight; j++)
			{
				if (hitArea[i][j])
				{
					pixelShape.currentGeometry->CompareWithGeometry(

						_other.shape.currentGeometry,
						((_transform + translation((float)i, (float)j)).invertTransform()
							+ _otherTransform).matrix,
						&result);
					if (result != D2D1_GEOMETRY_RELATION_DISJOINT)
						return true;
				}
			}
		return false;
	}

	bool BitmapCollisionBox::isCrashed(Transform _transform, const CollisionBox& _other, Transform _otherTransform)const
	{
		return _other.isCrashed(_otherTransform, *this, _transform);
	}

	ShapeCollisionBox::ShapeCollisionBox(const Shape& _shape)
	{
		shape = _shape;
	}

	bool ShapeCollisionBox::isCrashed(Transform _transform, const BitmapCollisionBox& _other, Transform _otherTransform)const
	{
		if (_other.hitArea == nullptr)
			return false;
		static Shape pixelShape = SquareShape(1, 0, 0);
		D2D1_GEOMETRY_RELATION result;
		UINT i, j;
		for (i = 0; i < _other.hitWidth; i++)
			for (j = 0; j < _other.hitHeight; j++)
			{
				if (_other.hitArea[i][j])
				{
					pixelShape.currentGeometry->CompareWithGeometry(
						shape.currentGeometry,
						((_otherTransform + translation((float)i, (float)j)).invertTransform()
							+ _transform).matrix,
						&result);
					if (result != D2D1_GEOMETRY_RELATION_DISJOINT)
						return true;
				}
			}
		return false;
	}

	bool ShapeCollisionBox::isCrashed(Transform _transform, const ShapeCollisionBox& _other, Transform _otherTransform)const
	{
		D2D1_GEOMETRY_RELATION result;
		Transform targetTransform = _otherTransform * _transform.invertTransform();
		shape.currentGeometry->CompareWithGeometry(
			_other.shape.currentGeometry,
			targetTransform.matrix,
			&result
		);
		if (result != D2D1_GEOMETRY_RELATION_DISJOINT)
			return true;
		else
			return false;
	}

	bool ShapeCollisionBox::isCrashed(Transform _transform, const CollisionBox& _other, Transform _otherTransform)const
	{
		return _other.isCrashed(_otherTransform, *this, _transform);
	}

	bool SpriteZero::isCrashed(Transform _transform, const SpriteZero& _other, Transform _otherTransform)const
	{
		if (!hitArea || !(_other.hitArea))
			return false;
		return hitArea->isCrashed(_transform, *(_other.hitArea), _otherTransform);
	}

	bool SpriteZero::isCrashed(Transform _transform, const SpriteZero* _other, Transform _otherTransform)const
	{
		if (!hitArea || !(_other->hitArea))
			return false;
		return hitArea->isCrashed(_transform, *(_other->hitArea), _otherTransform);
	}

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

	inline float Object::totalHspeed()const
	{
		return hspeed + hspeedTemp;
	}

	inline float Object::totalVspeed()const
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
	/*
	* old version
	bool Object::isCrashed(Object _obj, bool _isPredict)
	{
		BitmapSpriteZero* bodyza = nowState();
		BitmapSpriteZero* bodyzb = _obj.nowState();
		if (_isPredict)
		{
			_obj.x += _obj.totalHspeed();
			_obj.y += _obj.totalVspeed();
		}
		if (!bodyza || !bodyzb)
			return false;
		if (bodyza->hitArea == nullptr && bodyzb->hitArea == nullptr)
		{
			double xScale_a = fabs(xScale), yScale_a = fabs(yScale);
			double xScale_b = fabs(_obj.xScale), yScale_b = fabs(_obj.yScale);
			if (angle == 0.0 && _obj.angle == 0.0)
				return isCrashedRect(round(x + bodyza->hitX * xScale_a), round(y + bodyza->hitY * yScale_a), round(bodyza->hitWidth * xScale_a), round(bodyza->hitHeight * yScale_a),
					round(_obj.x + bodyzb->hitX * xScale_b), round(_obj.y + bodyzb->hitY * yScale_b), round(bodyzb->hitWidth * xScale_b), round(bodyzb->hitHeight * yScale_b));
			else
			{
				double disA = getDistance(0, 0, bodyza->hitWidth * xScale_a, bodyza->hitHeight * yScale_a);
				double disB = getDistance(0, 0, bodyzb->hitWidth * xScale_b, bodyzb->hitHeight * yScale_b);
				if (!isCrashedRect(x + bodyza->centerX * xScale_a - disA, y + bodyza->centerY * yScale_a - disA, disA * 2, disA * 2,
					_obj.x + bodyzb->centerX * xScale_b - disB, _obj.y + bodyzb->centerY * yScale_b - disB, disB * 2, disB * 2))
					return false;
				double xThis[4] = { 0 }, yThis[4] = { 0 };
				double xObj[4] = { 0 }, yObj[4] = { 0 };
				xThis[0] = x + bodyza->hitX * xScale_a;
				yThis[0] = y + bodyza->hitY * yScale_a;
				xThis[1] = xThis[0] + bodyza->hitWidth * xScale_a;
				yThis[1] = yThis[0];
				xThis[2] = xThis[0];
				yThis[2] = yThis[0] + bodyza->hitHeight * yScale_a;
				xThis[3] = xThis[1];
				yThis[3] = yThis[2];
				xObj[0] = _obj.x + bodyzb->hitX * xScale_b;
				yObj[0] = _obj.y + bodyzb->hitY * yScale_b;
				xObj[1] = xObj[0] + bodyzb->hitWidth * xScale_b;
				yObj[1] = yObj[0];
				xObj[2] = xObj[0];
				yObj[2] = yObj[0] + bodyzb->hitHeight * yScale_b;
				xObj[3] = xObj[1];
				yObj[3] = yObj[2];
				int i, j;
				for (i = 0; i < 4; i++)
					rotateDot(x + bodyza->centerX * xScale_a, y + bodyza->centerY * yScale_a, angle, &xThis[i], &yThis[i]);
				for (i = 0; i < 4; i++)
					rotateDot(_obj.x + bodyzb->centerX * xScale_b, _obj.y + bodyzb->centerY * yScale_b, _obj.angle, &xObj[i], &yObj[i]);
				for (i = 0; i < 4; i++)
				{
					xThis[i] = round(xThis[i]);
					yThis[i] = round(yThis[i]);
					xObj[i] = round(xObj[i]);
					yObj[i] = round(yObj[i]);
				}
				int lineA[4] = { 0,0,1,2 };
				int lineB[4] = { 1,2,3,3 };
				for (i = 0; i < 4; i++)
					for (j = 0; j < 4; j++)
					{
						if (isSegmentsIntersect(xThis[lineA[i]], yThis[lineA[i]], xThis[lineB[i]], yThis[lineB[i]],
							xObj[lineA[j]], yObj[lineA[j]], xObj[lineB[j]], yObj[lineB[j]]))
							return true;
					}
				return false;
			}
		}
		else if (bodyza->hitArea == nullptr && bodyzb->hitArea != nullptr)
		{
			double xScale_a = fabs(xScale), yScale_a = fabs(yScale);
			if (angle == 0.0 && _obj.angle == 0.0)
			{
				if (!isCrashedRect(x + bodyza->hitX * xScale_a, y + bodyza->hitY * yScale, bodyza->hitWidth * xScale_a, bodyza->hitHeight * yScale,
					_obj.x + bodyzb->hitX * _obj.xScale, _obj.y + bodyzb->hitY * _obj.yScale,
					bodyzb->hitWidth * _obj.xScale, bodyzb->hitHeight * _obj.yScale))
					return false;
				int i, j;
				for (i = 0; i < bodyzb->hitHeight; i++)
					for (j = 0; j < bodyzb->hitWidth; j++)
						if (*(bodyzb->hitArea + i * bodyzb->hitWidth + j))
						{
							if (isCrashedRect(round(x + bodyza->hitX * xScale_a), round(y + bodyza->hitY * yScale), round(bodyza->hitWidth * xScale_a), round(bodyza->hitHeight * yScale),
								round(_obj.x + (bodyzb->hitX + j) * _obj.xScale), round(_obj.y + (bodyzb->hitY + i) * _obj.yScale), _obj.xScale, _obj.yScale))
								return true;
						}
				return false;
			}
			else
			{
				double disA = getDistance(0, 0, bodyza->hitWidth * xScale, bodyza->hitHeight * yScale);
				double disB = getDistance(0, 0, bodyzb->hitWidth * _obj.xScale, bodyzb->hitHeight * _obj.yScale);
				if (!isCrashedRect(x + bodyza->centerX * xScale - disA, y + bodyza->centerY * yScale - disA, disA * 2, disA * 2,
					_obj.x + bodyzb->centerX * _obj.xScale - disB, _obj.y + bodyzb->centerY * _obj.yScale - disB, disB * 2, disB * 2))
					return false;
				double px, py;
				int i, j;
				for (i = 0; i < bodyzb->hitHeight; i++)
					for (j = 0; j < bodyzb->hitWidth; j++)
						if (*(bodyzb->hitArea + i * bodyzb->hitWidth + j))
						{
							px = _obj.x + (bodyzb->hitX + j) * _obj.xScale;
							py = _obj.y + (bodyzb->hitY + i) * _obj.yScale;
							rotateDot(_obj.x + bodyzb->centerX * _obj.xScale, _obj.y + bodyzb->centerY * _obj.yScale, _obj.angle, &px, &py);
							rotateDot(x + bodyza->centerX * xScale, y + bodyza->centerY * yScale, -angle, &px, &py);
							if (isCrashedRect(round(x + bodyza->hitX * xScale), round(y + bodyza->hitY * yScale), round(bodyza->hitWidth * xScale), round(bodyza->hitHeight * yScale),
								round(px), round(py), _obj.xScale, _obj.yScale))
								return true;
						}
				return false;
			}
		}
		else if (bodyza->hitArea == nullptr && bodyzb != nullptr)
			return _obj.isCrashed(*this);
		else
		{
			double disA = getDistance(0, 0, bodyza->hitWidth * xScale, bodyza->hitHeight * yScale);
			double disB = getDistance(0, 0, bodyzb->hitWidth * _obj.xScale, bodyzb->hitHeight * _obj.yScale);
			if (!isCrashedRect(x + bodyza->centerX * xScale - disA, y + bodyza->centerY * yScale - disA, disA * 2, disA * 2,
				_obj.x + bodyzb->centerX * _obj.xScale - disB, _obj.y + bodyzb->centerY * _obj.yScale - disB, disB * 2, disB * 2))
				return false;
			double px, py;
			int i, j;
			for (i = 0; i < bodyzb->hitHeight; i++)
				for (j = 0; j < bodyzb->hitWidth; j++)
					if (*(bodyzb->hitArea + i * bodyzb->hitWidth + j))
					{
						px = _obj.x + (bodyzb->hitX + j) * _obj.xScale;
						py = _obj.y + (bodyzb->hitY + i) * _obj.yScale;
						rotateDot(_obj.x + bodyzb->centerX * _obj.xScale, _obj.y + bodyzb->centerY * _obj.yScale, _obj.angle, &px, &py);
						rotateDot(x + bodyza->centerX * xScale, y + bodyza->centerY * yScale, -angle, &px, &py);
						if (*(bodyza->hitArea + (int)round(py - bodyza->hitX - x) * bodyza->hitWidth + (int)round(px - bodyza->hitY - y)))
							return true;
					}
			return false;
		}
	}
	*/

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

	Room::Room()
	{
		hasCreated = false;
		objectPointerArray.clear();
		playerStartX = 0.0;
		playerStartY = 0.0;
	}

	void Room::setPlayerStart(float _x, float _y)
	{
		playerStartX = _x;
		playerStartY = _y;
	}

	Object* Room::findObj(size_t _kindId, size_t _pos)
	{
		size_t x = 0;
		Object* objPointer = nullptr;
		std::list<Object*>* targetList = objectList<Object>();
		for (auto& i : *targetList)
		{
			x++;
			if (x == _pos)
				return i;
		}
		return nullptr;
	}
	/*
	Object* Room::findObjWithPosition(int _kind, double _x, double _y)
	{
		double minDistance = 100000000.0;
		Object* result = nullptr;
		for (auto i = kindStart[_kind]; i != kindEnd[_kind]; i++)
		{
			if (getDistance((*i)->x, (*i)->y, _x, _y) < minDistance)
			{
				minDistance = getDistance((*i)->x, (*i)->y, _x, _y);
				result = (*i);
			}
		}
		return result;
	}

	Object* Room::findObjWithCenterPosition(int _kind, double _x, double _y)
	{
		double minDistance = 100000000.0;
		Object* result = nullptr;
		for (auto i = kindStart[_kind]; i != kindEnd[_kind]; i++)
		{
			if ((*i)->nowState() == nullptr)
				continue;
			if (getDistance((*i)->nowState()->centerX, (*i)->nowState()->centerY, _x, _y) < minDistance)
			{
				minDistance = getDistance((*i)->x, (*i)->y, _x, _y);
				result = (*i);
			}
		}
		return result;
	}*/


	void Room::update()
	{
		onUpdateStart();

		Object* previousObj = nullptr;
		std::list<Object*>* objectList;
		objectPointerArray[typecode(Object)] >> objectList;

		for (auto& obj : *objectList)
		{
			if (previousObj && previousObj->removeTag_)
			{
				previousObj->removeTag_ = false;
				previousObj->remove();
				if (previousObj->destroyTag_)
				{
					delete previousObj;
					previousObj = nullptr;
				}
			}
			obj->hspeedTemp = obj->vspeedTemp = 0;
			obj->var["xBefore"] << obj->x;
			obj->var["yBefore"] << obj->y;
			obj->isUpdating_ = true;
			obj->updateFunction();
			obj->isUpdating_ = false;
			previousObj = obj;
		}
		if (previousObj && previousObj->removeTag_)
		{
			previousObj->removeTag_ = false;
			previousObj->remove();
			if (previousObj->destroyTag_)
			{
				delete previousObj;
				previousObj = nullptr;
			}
		}

		for (auto& x : reviseStateArray)
			for (auto& obj : x.second)
			{
				if (previousObj && previousObj->removeTag_)
				{
					previousObj->removeTag_ = false;
					previousObj->remove();
					if (previousObj->destroyTag_)
					{
						delete previousObj;
						previousObj = nullptr;
					}
				}
				obj->isUpdating_ = true;
				obj->reviseState();
				obj->isUpdating_ = false;
				previousObj = obj;
			}
		if (previousObj && previousObj->removeTag_)
		{
			previousObj->removeTag_ = false;
			previousObj->remove();
			if (previousObj->destroyTag_)
			{
				delete previousObj;
				previousObj = nullptr;
			}
		}

		for (auto& x : updateStateArray)
			for (auto& obj : x.second)
			{
				if (previousObj && previousObj->removeTag_)
				{
					previousObj->removeTag_ = false;
					previousObj->remove();
					if (previousObj->destroyTag_)
					{
						delete previousObj;
						previousObj = nullptr;
					}
				}
				obj->isUpdating_ = true;
				obj->updateState();
				obj->isUpdating_ = false;
				previousObj = obj;
			}
		if (previousObj && previousObj->removeTag_)
		{
			previousObj->removeTag_ = false;
			previousObj->remove();
			if (previousObj->destroyTag_)
			{
				delete previousObj;
				previousObj = nullptr;
			}
		}

		for (auto& x : recheckStateArray)
			for (auto& obj : x.second)
			{
				if (previousObj && previousObj->removeTag_)
				{
					previousObj->removeTag_ = false;
					previousObj->remove();
					if (previousObj->destroyTag_)
					{
						delete previousObj;
						previousObj = nullptr;
					}
				}
				obj->isUpdating_ = true;
				obj->recheckState();
				obj->isUpdating_ = false;
				previousObj = obj;
			}
		if (previousObj && previousObj->removeTag_)
		{
			previousObj->removeTag_ = false;
			previousObj->remove();
			if (previousObj->destroyTag_)
			{
				delete previousObj;
				previousObj = nullptr;
			}
		}

		for (auto& obj : *objectList)
		{
			obj->x += obj->totalHspeed();
			obj->y += obj->totalVspeed();
		}

		onUpdateEnd();
	}

	void Room::paint()
	{
		onPaintStart();

		for (auto& x : paintArray)
			for (auto& obj : x.second)
			{
				if (!obj->onPaint())
					obj->paintBody();
			}

		onPaintEnd();
	}

	void Room::additionalFramePaint(float _offset)
	{
		for (auto& x : paintArray)
			for (auto& obj : x.second)
			{
				float deltaX = obj->x - obj->var["xBefore"].getValue<float>();
				float deltaY = obj->y - obj->var["yBefore"].getValue<float>();
				obj->paintBody(obj->var["xBefore"].getValue<float>() + deltaX * _offset,
					obj->var["yBefore"].getValue<float>() + deltaY * _offset);
			}
	}

	void Room::reset()
	{
		std::list<Object*>* allObjList = objectList<Object>();
		for (auto& objPointer : *allObjList)
		{
			objPointer->onRestarting();
		}
		//player->spawn();
	}

	/*
	void Room::paintBody()
	{
		int j;
		std::list<Object*>::iterator k;
		std::list<Object*> object_painting_array[VALUE_MAXPAINT];
		//for (j = 0; j < VALUE_MAXPAINT; j++)
		//	object_painting_array[j].clear();
		for (auto& i : objectPointerArray[typeid(Object).hash_code()])
		{
			Object* obj;
			i >> obj;
			object_painting_array[obj->paintId_].push_back(obj);
		}
		//object_painting_array[player->paintId_].push_back(player);
		for (j = 0; j < VALUE_MAXPAINT; j++)
			for (k = object_painting_array[j].begin(); k != object_painting_array[j].end(); k++)
				(*k)->paintBody();
	}*/

	void BitmapSpriteZero::catchBitmap(String _path, UINT _startX, UINT _startY)
	{
		if (pBitmap_ != nullptr)
		{
			delete pBitmap_;
			pBitmap_ = nullptr;
		}
		pBitmap_ = new Bitmap(_path, _startX, _startY, width, height);
	}

	void BitmapSpriteZero::catchBitmapAndSize(String _path)
	{
		if (pBitmap_ != nullptr)
		{
			delete pBitmap_;
			pBitmap_ = nullptr;
		}
		pBitmap_ = new Bitmap(_path);
		width = pBitmap_->getWidth();
		height = pBitmap_->getHeight();
	}

	void BitmapSpriteZero::paint(float _x, float _y, float _xScale, float _yScale, float _alpha, float _angle)
	{
		pBitmap_->paint(translation(_x, _y) + scale(centerX, centerY, _xScale, _yScale) + rotation(centerX, centerY, _angle),
			_alpha);
		//drawBitmap(d2dBitmap_, (float)width, (float)height, _alpha,
		//	translation(_x, _y) + scale(centerX, centerY, _xScale, _yScale) + rotation(centerX, centerY, _angle));
	}

	void BitmapSpriteZero::paint(float _x, float _y, Transform _transform, float _alpha)
	{
		pBitmap_->paint(translation(_x, _y) + _transform,
			_alpha);
		//drawBitmap(d2dBitmap_, (float)width, (float)height, _alpha,
		//	translation(_x, _y) + _transform);
	}

	void BitmapSpriteZero::paint(Transform _transform, float _alpha)
	{
		pBitmap_->paint(_transform, _alpha);
		//drawBitmap(d2dBitmap_, (float)width, (float)height, _alpha, _transform);
	}

	BitmapSpriteZero::BitmapSpriteZero()
	{
		hitArea = nullptr;
		pBitmap_ = nullptr;
		width = 0;
		height = 0;
		centerX = 0;
		centerY = 0;
	}

	BitmapSpriteZero::BitmapSpriteZero(const Shape& _collisionBox, const Bitmap& _bitmap, float _centerX, float _centerY)
	{
		width = _bitmap.getWidth();
		height = _bitmap.getHeight();
		centerX = _centerX;
		centerY = _centerY;
		pBitmap_ = new Bitmap(_bitmap);
		hitArea = new ShapeCollisionBox(_collisionBox);
	}

	BitmapSpriteZero::BitmapSpriteZero(String _path, const Shape& _collisionBox, float _centerX, float _centerY)
	{
		centerX = _centerX;
		centerY = _centerY;
		pBitmap_ = nullptr;
		catchBitmapAndSize(_path);
		hitArea = new ShapeCollisionBox(_collisionBox);
	}

	BitmapSpriteZero::BitmapSpriteZero(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height,
		const Shape& _collisionBox, float _centerX, float _centerY)
	{
		width = _width;
		height = _height;
		centerX = _centerX;
		centerY = _centerY;
		pBitmap_ = new Bitmap(_path, _startX, _startY, _width, _height);
		hitArea = new ShapeCollisionBox(_collisionBox);
	}

	BitmapSpriteZero::BitmapSpriteZero(UINT _width, UINT _height, const Shape& _collisionBox, float _centerX, float _centerY)
	{
		width = _width;
		height = _height;
		centerX = _centerX;
		centerY = _centerY;
		pBitmap_ = nullptr;
		hitArea = new ShapeCollisionBox(_collisionBox);
	}

	BitmapSpriteZero::BitmapSpriteZero(UINT _width, UINT _height, const BitmapCollisionBox& _collisionBox,
		float _centerX, float _centerY, String _path)
	{
		hitArea = new BitmapCollisionBox(_collisionBox);
		width = _width;
		height = _height;
		centerX = _centerX;
		centerY = _centerY;
		pBitmap_ = new Bitmap(_path);
	}

	BitmapSpriteZero::BitmapSpriteZero(String _path, float _centerX, float _centerY, float _alphaThreshold)
	{
		pBitmap_ = nullptr;
		centerX = _centerX;
		centerY = _centerY;

		pBitmap_ = new Bitmap(_path);

		if (pBitmap_)
		{
			width = pBitmap_->getWidth();
			height = pBitmap_->getHeight();
			hitArea = new BitmapCollisionBox(pBitmap_, _alphaThreshold);
		}
		else
		{
			width = height = 0;
			hitArea = nullptr;
		}
	}

	BitmapSpriteZero::BitmapSpriteZero(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height,
		float _centerX, float _centerY, float _alphaThreshold)
	{
		pBitmap_ = nullptr;
		width = _width;
		height = _height;
		centerX = _centerX;
		centerY = _centerY;

		pBitmap_ = new Bitmap(_path, _startX, _startY, _width, _height);

		if (pBitmap_)
			hitArea = new BitmapCollisionBox(pBitmap_, _alphaThreshold);
		else
			hitArea = nullptr;
	}

	Sprite::Sprite()
	{
		flipTime_ = 1;
	}

	Sprite::Sprite(String _path, UINT _amount, int _flipTime, const Shape& _collisionBox, float _centerX, float _centerY)
	{
		flipTime_ = _flipTime;
		Bitmap originalBitmap(_path);
		auto [originalWidth, originalHeight] = originalBitmap.getSize();
		UINT width = originalWidth / _amount;
		for (UINT i = 0; i < _amount; i++)
		{
			BitmapSpriteZero* sprZ = new BitmapSpriteZero(_path, i * width, 0, width, originalHeight,
				_collisionBox, _centerX, _centerY);
			bodyList.push_back((SpriteZero*)sprZ);
		}
	}

	Sprite::Sprite(String _path, UINT _amount, int _flipTime, float _centerX, float _centerY, float _alphaThreshold)
	{
		flipTime_ = _flipTime;
		Bitmap originalBitmap(_path);
		auto [originalWidth, originalHeight] = originalBitmap.getSize();
		UINT width = originalWidth / _amount;
		for (UINT i = 0; i < _amount; i++)
		{
			BitmapSpriteZero* sprZ = new BitmapSpriteZero(_path, i * width, 0, width, originalHeight,
				_centerX, _centerY, _alphaThreshold); 
			bodyList.push_back((SpriteZero*)sprZ);
		}
	}

	Sprite::Sprite(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height, UINT _amount, int _flipTime,
		const Shape& _collisionBox, float _centerX, float _centerY)
	{
		flipTime_ = _flipTime;
		for (UINT i = 0; i < _amount; i++)
		{
			BitmapSpriteZero* sprZ = new BitmapSpriteZero(_path, _startX + i * _width, _startY, _width, _height,
				_collisionBox, _centerX, _centerY);
			bodyList.push_back((SpriteZero*)sprZ);
		}
	}

	Sprite::Sprite(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height, UINT _amount, int _flipTime,
		float _centerX, float _centerY, float _alphaThreshold)
	{
		flipTime_ = _flipTime;
		for (UINT i = 0; i < _amount; i++)
		{
			BitmapSpriteZero* sprZ = new BitmapSpriteZero(_path, _startX + i * _width, _startY, _width, _height,
				_centerX, _centerY, _alphaThreshold);
			bodyList.push_back((SpriteZero*)sprZ);
		}
	}

	void Sprite::setSpeed(int _speed)
	{
		flipTime_ = _speed;
	}

	SpriteZero* Sprite::getState(int _wp)
	{
		if (bodyList.empty())
			return nullptr;
		else return bodyList[_wp / flipTime_ % bodyList.size()];
	}

	Group Group::operator&(Group& _group)
	{
		Group result;
		for (auto& i : member_)
			if (_group.isInclude(i))
				result.append(i);
		return result;
	}

	Group Group::operator|(Group& _group)
	{
		Group result;
		for (auto& i : member_)
			result.append(i);
		for (auto& i : _group.member_)
			result.append(i);
		return result;
	}

	Group Group::operator^(Group& _group)
	{
		Group result;
		for (auto& i : member_)
			result.append(i);
		for (auto& i : _group.member_)
		{
			if (isInclude(i))
				result.removeObj(i);
			else result.append(i);
		}
		return result;
	}

	size_t Group::getSize()
	{
		return member_.size();
	}

	size_t Group::append(Object* _object)
	{
		if (!isInclude(_object))
		{
			memberIterator_[_object] = member_.insert(member_.end(), _object);
		}
		return member_.size();
	}

	size_t Group::append(Object* _firstObject, Object* _secondObject, ...)
	{
		va_list objArg;
		va_start(objArg, _secondObject);
		append(_firstObject);
		append(_secondObject);
		Object* obj;
		while (1)
		{
			obj = va_arg(objArg, Object*);
			if (obj == nullptr)
				break;
			append(obj);
		}
		va_end(objArg);
		return member_.size();
	}

	size_t Group::append(std::list<Object*> _objectList)
	{
		for (auto& obj : _objectList)
			append(obj);
		return member_.size();
	}
	/*
	size_t Group::appendSelectively(Room* _room, std::function<bool(Object*)> _function, int _kind)
	{
		for (auto i = _room->kindStart[_kind]; i != _room->kindEnd[_kind]; i++)
			if ((*i)->id != 0 && !id_[*i] && _function(*i))
			{
				member_.push_back(*i);
				id_[*i] = member_.size();
			}
		return member_.size();
	}*/

	size_t Group::appendSelectively(Room* _room, std::function<bool(Object*)> _function)
	{
		std::list<Object*>* objectList = _room->objectList<Object>();
		for (auto& objPointer : *objectList)
		{
			if (_function(objPointer))
				append(objPointer);
		}
		return member_.size();
	}

	/*size_t Group::appendSelectively(Group* _group, std::function<bool(Object*)> _function, int _kind)
	{
		size_t size = _group->getSize();
		Object* obj;
		for (int i = 1; i <= size; i++)
		{
			obj = _group->findObjWithGroupId(i);
			if (obj->kind == _kind && _function(obj))
				create(obj);
		}
		return member_.size();
	}*/

	size_t Group::appendSelectively(Group* _group, std::function<bool(Object*)> _function)
	{
		size_t size = _group->getSize();
		Object* obj;
		for (size_t i = 1; i <= size; i++)
		{
			obj = _group->findObj(i);
			if (_function(obj))
				append(obj);
		}
		return member_.size();
	}

	size_t Group::appendSelectively(std::list<Object*> _objectList, std::function<bool(Object*)> _function)
	{
		for (auto& obj : _objectList)
		{
			if (_function(obj))
				append(obj);
		}
		return member_.size();
	}

	bool Group::isInclude(Object* _object)
	{
		return memberIterator_.find(_object) != memberIterator_.end();
	}


	void Group::removeObj(size_t _pos)
	{
		removeObj(findObj(_pos));
	}

	void Group::removeObj(Object* _object)
	{
		auto targetNode = memberIterator_.find(_object);
		if (targetNode != memberIterator_.end())
		{
			std::list<Object*>::iterator objectIterator = memberIterator_[_object];
			member_.erase(objectIterator);
			memberIterator_.erase(targetNode);
		}
	}

	Object* Group::findObj(size_t _pos)
	{
		size_t counter = 0;
		for (auto& obj : member_)
		{
			counter++;
			if (counter == _pos)
				return obj;
		}
		return nullptr;
	}

	void Group::setAction(std::function<void(Object*, int)> _function)
	{
		int j = 0;
		for (auto i = member_.begin(); i != member_.end(); i++)
		{
			j++;
			if (i != member_.end())
				_function(*i, j);
		}
	}

	ShapeSpriteZero::ShapeSpriteZero(const Shape& _shape, ID2D1Brush* _fillBrush,
		ID2D1Brush* _outlineBrush, float _outlineWidth, ID2D1StrokeStyle* _outlineStrokeStyle)
	{
		shape = _shape;
		SAFE_ADDREF(_fillBrush);
		fillBrush = _fillBrush;
		SAFE_ADDREF(_outlineBrush);
		outlineBrush = _outlineBrush;

		outlineWidth = _outlineWidth;

		SAFE_ADDREF(_outlineStrokeStyle);
		outlineStrokeStyle = _outlineStrokeStyle;
	}

	ShapeSpriteZero::ShapeSpriteZero(const Shape& _shape, const Color& _fillColor)
	{
		shape = _shape;

		ID2D1SolidColorBrush* newBrush;
		HRESULT hr;
		hr = pMainRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(_fillColor.r() / 255.0f, _fillColor.g() / 255.0f, _fillColor.b() / 255.0f, _fillColor.alpha),
			&newBrush
		);
		if (SUCCEEDED(hr))
		{
			newBrush->AddRef();
			fillBrush = newBrush;
			newBrush->AddRef();
			outlineBrush = newBrush;
		}
		else
		{
			fillBrush = nullptr;
			outlineBrush = nullptr;
		}

		outlineWidth = 1.0f;
		outlineStrokeStyle = nullptr;
	}

	ShapeSpriteZero::ShapeSpriteZero(const Shape& _shape, const Color& _fillColor,
		const Color& _outlineColor, float _outlineWidth, ID2D1StrokeStyle* _outlineStrokeStyle)
	{
		shape = _shape;

		ID2D1SolidColorBrush* newBrush;
		HRESULT hr;
		hr = pMainRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(_fillColor.r() / 255.0f, _fillColor.g() / 255.0f, _fillColor.b() / 255.0f, _fillColor.alpha),
			&newBrush
		);
		if (SUCCEEDED(hr))
		{
			newBrush->AddRef();
			fillBrush = newBrush;
		}
		else
			fillBrush = nullptr;

		hr = pMainRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(_outlineColor.r() / 255.0f, _outlineColor.g() / 255.0f, _outlineColor.b() / 255.0f, _outlineColor.alpha),
			&newBrush
		);
		if (SUCCEEDED(hr))
		{
			newBrush->AddRef();
			outlineBrush = newBrush;
		}
		else
			outlineBrush = nullptr;

		outlineWidth = _outlineWidth;
		SAFE_ADDREF(_outlineStrokeStyle);
		outlineStrokeStyle = _outlineStrokeStyle;
	}

	ShapeSpriteZero::~ShapeSpriteZero()
	{
		SAFE_RELEASE(fillBrush);
		SAFE_RELEASE(outlineBrush);
		SAFE_RELEASE(outlineStrokeStyle);
	}

	void ShapeSpriteZero::setShapeTransform(Transform _transform)
	{
		shape.setTransform(_transform);
	}

	void ShapeSpriteZero::paint(float _x, float _y, float _xScale, float _yScale, float _angle)
	{
		Transform paintingTransform = translation(_x, _y) + scale(centerX, centerY, _xScale, _yScale)
			+ rotation(centerX, centerY, _angle);
		shape.paint(paintingTransform, fillBrush, outlineBrush, outlineWidth, outlineStrokeStyle);
	}

	void ShapeSpriteZero::paint(float _x, float _y, Transform _paintingTransform)
	{
		shape.paint(_x, _y, _paintingTransform, fillBrush, outlineBrush, outlineWidth, outlineStrokeStyle);
	}

	void ShapeSpriteZero::paint(Transform _paintingTransform)
	{
		shape.paint(_paintingTransform, fillBrush, outlineBrush, outlineWidth, outlineStrokeStyle);
	}

	void ShapeSpriteZero::setFillColor(const Color& _color)
	{
		if (fillBrush != nullptr)
			SAFE_RELEASE(fillBrush);
		ID2D1SolidColorBrush* newBrush;
		HRESULT hr;
		hr = pMainRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(_color.r() / 255.0f, _color.g() / 255.0f, _color.b() / 255.0f, _color.alpha),
			&newBrush
		);
		if (SUCCEEDED(hr))
		{
			newBrush->AddRef();
			fillBrush = newBrush;
		}
	}

	void ShapeSpriteZero::setOutlineColor(const Color& _color)
	{
		if (outlineBrush != nullptr)
			SAFE_RELEASE(outlineBrush);
		ID2D1SolidColorBrush* newBrush;
		HRESULT hr;
		hr = pMainRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(_color.r() / 255.0f, _color.g() / 255.0f, _color.b() / 255.0f, _color.alpha),
			&newBrush
		);
		if (SUCCEEDED(hr))
		{
			newBrush->AddRef();
			outlineBrush = newBrush;
		}
	}

	void ShapeSpriteZero::setOutlineWidth(int _width)
	{
		outlineWidth = (float)_width;
	}

	void ShapeSpriteZero::paint(float _x, float _y, float _xScale, float _yScale, float _alpha, float _angle)
	{
		shape.paint(translation(_x, _y) + scale(centerX, centerY, _xScale, _yScale) + rotation(centerX, centerY, _angle),
			fillBrush,
			outlineBrush,
			outlineWidth,
			outlineStrokeStyle
		);
	}

	void ShapeSpriteZero::paint(float _x, float _y, Transform _transform, float _alpha)
	{
		shape.paint(translation(_x, _y) + _transform,
			fillBrush,
			outlineBrush,
			outlineWidth,
			outlineStrokeStyle
		);
	}

	void ShapeSpriteZero::paint(Transform _transform, float _alpha)
	{
		shape.paint(_transform,
			fillBrush,
			outlineBrush,
			outlineWidth,
			outlineStrokeStyle
		);
	}

	void gotoRoom(Room& _room)
	{
		if (_room.hasCreated == false)
		{
			_room.onCreate();
			//_room.onJoin()...?
			nowRoom = &_room;
		}
		else
		{
			_room.onJoin();
			nowRoom = &_room;
		}
	}
}
