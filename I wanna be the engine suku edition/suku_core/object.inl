#pragma once
#include "room.h"
#include "object.h"
#include "sprite.h"

namespace suku
{
	using namespace suku::maths;

	template<suku_object Obj>
	bool Object::isCrashed(const Obj& _obj)const
	{
		return getSpriteFrame()->isCrashed(translation(bRound(x), bRound(y)) + transform, _obj.getSpriteFrame(),
			translation(bRound(_obj.x), bRound(_obj.y)) + _obj.transform);
	}

	template<suku_object Obj>
	bool Object::isCrashed(const Obj& _obj, float _x, float _y)const
	{
		return getSpriteFrame()->isCrashed(translation(bRound(_x), bRound(_y)) + transform, _obj.getSpriteFrame(),
			translation(bRound(_obj.x), bRound(_obj.y)) + _obj.transform);
	}

	template<suku_object Obj>
	bool Object::isCrashed(const Obj& _obj, float _x, float _y, float _objX, float _objY)const
	{
		return getSpriteFrame()->isCrashed(translation(bRound(_x), bRound(_y)) + transform, _obj.getSpriteFrame(),
			translation(bRound(_objX), bRound(_objY)) + _obj.transform);
	}

	template<suku_object Obj>
	bool Object::isCrashed(const Obj& _obj, Vector _position) const
	{
		return getSpriteFrame()->isCrashed(translation(bRound(_position.x), bRound(_position.y)) + transform, _obj.getSpriteFrame(),
			translation(bRound(_obj.x), bRound(_obj.y)) + _obj.transform);
	}

	template<suku_object Obj>
	bool Object::isCrashed(const Obj& _obj, Vector _position, Vector _objPosition) const
	{
		return getSpriteFrame()->isCrashed(translation(bRound(_position.x), bRound(_position.y)) + transform, _obj.getSpriteFrame(),
			translation(bRound(_objPosition.x), bRound(_objPosition.y)) + _obj.transform);
	}

	template<suku_object Obj>
	bool Object::isCrashed(const std::list<Obj*>& _objList) const
	{
		for (auto& i : _objList)
		{
			if (isCrashed(*i))
				return true;
		}
		return false;
	}

	template<suku_object Obj>
	bool Object::isCrashed(const std::list<Obj*>& _objList, float _x, float _y) const
	{
		for (auto& i : _objList)
		{
			if (isCrashed(*i, _x, _y))
				return true;
		}
		return false;
	}

	template<suku_object Obj>
	inline Obj* Object::selectObject(std::function<bool(Obj*)> _function)
	{
		if (!inRoom_)
			return nullptr;
		typename std::list<Obj*> targetList = inRoom_->getObjectList<Obj>();
		if (targetList != nullptr)
		{
			for (auto objPointer : targetList)
			{
				if (objPointer != this && _function(objPointer))
					return objPointer;
			}
		}
	}

	template<suku_object Obj>
	inline std::list<Obj*> Object::selectObjectList(std::function<bool(Obj*)> _function)
	{
		if (!inRoom_)
			return std::list<Obj*>();
		typename std::list<Obj*> targetList = inRoom_->getObjectList<Obj>();
		typename std::list<Obj*> resultList;
		if (targetList != nullptr)
		{
			for (auto objPointer : targetList)
			{
				if (objPointer != this && _function(objPointer))
					resultList.push_back(objPointer);
			}
		}
		return resultList;
	}

	template<suku_object Obj>
	inline Obj* Object::getCrashedObject(bool _isPredict)
	{
		if (!inRoom_)
			return nullptr;
		return inRoom_->getCrashedObject<Obj>(this);
		//std::list<Obj*> targetList = inRoom_->getObjectList<Obj>();
		//if (!targetList.empty())
		//{
		//	if (_isPredict)
		//	{
		//		for (auto objPointer : targetList)
		//		{
		//			if (static_cast<Object*>(objPointer) != static_cast<Object*>(this)
		//				&& isCrashed(*objPointer, x, y, objPointer->x, objPointer->y))
		//				return objPointer;
		//		}
		//	}
		//	else
		//	{
		//		for (auto objPointer : targetList)
		//		{
		//			if (static_cast<Object*>(objPointer) != static_cast<Object*>(this)
		//				&& isCrashed(*objPointer, x, y))
		//				return objPointer;
		//		}
		//	}
		//}
		//return nullptr;
	}

	template<suku_object Obj>
	inline std::list<Obj*> Object::getCrashedObjectList(bool _isPredict)
	{
		if (!inRoom_)
			return std::list<Obj*>();
		return inRoom_->getCrashedObjectList<Obj>(this);
		//std::list<Obj*> resultList;
		//if (!inRoom_)
		//	return resultList;
		//std::list<Obj*> targetList = inRoom_->getObjectList<Obj>();
		//if (!targetList.empty())
		//{
		//	if (_isPredict)
		//	{
		//		for (auto objPointer : targetList)
		//		{
		//			if (static_cast<Object*>(objPointer) != static_cast<Object*>(this)
		//				&& isCrashed(*objPointer, x, y, objPointer->x, objPointer->y))
		//				resultList.push_back(objPointer);
		//		}
		//	}
		//	else
		//	{
		//		for (auto objPointer : targetList)
		//		{
		//			if (static_cast<Object*>(objPointer) != static_cast<Object*>(this)
		//				&& isCrashed(*objPointer, x, y))
		//				resultList.push_back(objPointer);
		//		}
		//	}
		//}
		//return resultList;
	}

	template<suku_object Obj>
	inline Obj* Object::getCrashedObject(float _x, float _y, bool _isPredict)
	{
		if (!inRoom_)
			return nullptr;
		float oldX = x, oldY = y;
		x = _x;
		y = _y;
		Obj* result = inRoom_->getCrashedObject<Obj>(this);
		x = oldX;
		y = oldY;
		return result;
		//std::list<Obj*> targetList = inRoom_->getObjectList<Obj>();
		//if (!targetList.empty())
		//{
		//	if (_isPredict)
		//	{
		//		for (auto objPointer : targetList)
		//		{
		//			if (static_cast<Object*>(objPointer) != static_cast<Object*>(this)
		//				&& isCrashed(*objPointer, _x, _y, objPointer->x, objPointer->y))
		//				return objPointer;
		//		}
		//	}
		//	else
		//	{
		//		for (auto objPointer : targetList)
		//		{
		//			if (static_cast<Object*>(objPointer) != static_cast<Object*>(this)
		//				&& isCrashed(*objPointer, _x, _y))
		//				return objPointer;
		//		}
		//	}
		//}
		//return nullptr;
	}

	template<suku_object Obj>
	inline std::list<Obj*> Object::getCrashedObjectList(float _x, float _y, bool _isPredict)
	{
		if (!inRoom_)
			return std::list<Obj*>();
		float oldX = x, oldY = y;
		x = _x;
		y = _y;
		std::list<Obj*> result = inRoom_->getCrashedObjectList<Obj>(this);
		x = oldX;
		y = oldY;
		return result;
		//std::list<Obj*> resultList;
		//if (!inRoom_)
		//	return resultList;
		//std::list<Obj*> targetList = inRoom_->getObjectList<Obj>();
		//if (!targetList.empty())
		//{
		//	if (_isPredict)
		//	{
		//		for (auto objPointer : targetList)
		//		{
		//			if (static_cast<Object*>(objPointer) != static_cast<Object*>(this)
		//				&& isCrashed(*objPointer, _x, _y, objPointer->x, objPointer->y))
		//				resultList.push_back(objPointer);
		//		}
		//	}
		//	else
		//	{
		//		for (auto objPointer : targetList)
		//		{
		//			if (static_cast<Object*>(objPointer) != static_cast<Object*>(this)
		//				&& isCrashed(*objPointer, _x, _y))
		//				resultList.push_back(objPointer);
		//		}
		//	}
		//}
		//return resultList;
	}
}