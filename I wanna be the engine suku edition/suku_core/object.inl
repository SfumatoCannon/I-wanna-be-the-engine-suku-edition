#pragma once
#include "room.h"
#include "object.h"

namespace suku
{
	template<suku_object Obj>
	inline Obj* Object::getInsideObject()
	{
		if (!inRoom_)
			return nullptr;
		typename std::list<Obj*>::iterator res;
		insideObjectIterator_ >> res;
		return (*res);
	}

	template<suku_object Obj>
	inline Obj* Object::selectObject(std::function<bool(Obj*)> _function)
	{
		if (!inRoom_)
			return nullptr;
		typename std::list<Obj*>* targetList = inRoom_->objectList<Obj>();
		if (targetList != nullptr)
		{
			for (auto& objPointer : *targetList)
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
		typename std::list<Obj*>* targetList = inRoom_->objectList<Obj>();
		typename std::list<Obj*> resultList;
		if (targetList != nullptr)
		{
			for (auto& objPointer : *targetList)
			{
				if (objPointer != this && _function(objPointer))
					resultList.push_back(objPointer);
			}
		}
		return resultList;
	}

	template<suku_object Obj>
	inline Obj* Object::getCrashedObjectPrecisely(bool _isPredict)
	{
		if (!inRoom_)
			return nullptr;
		typename std::list<Obj*>* targetList = inRoom_->objectList<Obj>();
		if (targetList != nullptr)
		{
			if (_isPredict)
			{
				for (auto& objPointer : *targetList)
				{
					if (objPointer != this && isCrashed(*objPointer, x, y, objPointer->x, objPointer->y))
						return objPointer;
				}
			}
			else
			{
				for (auto& objPointer : *targetList)
				{
					if (objPointer != this && isCrashed(*objPointer, x, y))
						return objPointer;
				}
			}
		}
		return nullptr;
	}

	template<suku_object Obj>
	inline Object* Object::getCrashedObject(bool _isPredict)
	{
		if (!inRoom_)
			return nullptr;
		std::list<TypeNode*> targetTypeList = collisionInheritTree.getAllChildList<Obj>();
		std::list<Object*>* targetList;
		while (!targetTypeList.empty())
		{
			targetList = &(inRoom_->objectParentPointerArray[targetTypeList.front()->code]);
			targetTypeList.pop_front();
			if (targetList != nullptr)
			{
				if (_isPredict)
				{
					for (auto& objPointer : *targetList)
					{
						if (objPointer != this && isCrashed(*objPointer, x, y, objPointer->x, objPointer->y))
							return objPointer;
					}
				}
				else
				{
					for (auto& objPointer : *targetList)
					{
						if (objPointer != this && isCrashed(*objPointer, x, y))
							return objPointer;
					}
				}
			}
		}
		return nullptr;
	}

	template<suku_object Obj>
	inline std::list<Obj*> Object::getCrashedObjectListPrecisely(bool _isPredict)
	{
		typename std::list<Obj*> resultList;
		if (!inRoom_)
			return resultList;
		typename std::list<Obj*>* targetList = inRoom_->objectList<Obj>();
		if (targetList != nullptr)
		{
			if (_isPredict)
			{
				for (auto& objPointer : targetList)
				{
					if (objPointer != this && isCrashed(*objPointer, x, y, objPointer->x, objPointer->y))
						resultList.push_back(objPointer);
				}
			}
			else
			{
				for (auto& objPointer : targetList)
				{
					if (objPointer != this && isCrashed(*objPointer, x, y))
						resultList.push_back(objPointer);
				}
			}
		}
		return resultList;
	}

	template<suku_object Obj>
	inline std::list<Object*> Object::getCrashedObjectList(bool _isPredict)
	{
		if (!inRoom_)
			return nullptr;
		std::list<TypeNode*> targetTypeList = collisionInheritTree.getAllChildList<Obj>();
		std::list<Object*>* targetList;
		std::list<Object*> resultList;
		while (!targetTypeList.empty())
		{
			targetList = &(inRoom_->objectParentPointerArray[targetTypeList.front()->code]);
			targetTypeList.pop_front();
			if (targetList != nullptr)
			{
				if (_isPredict)
				{
					for (auto& objPointer : *targetList)
					{
						if (objPointer != this && isCrashed(*objPointer, x, y, objPointer->x, objPointer->y))
							resultList.push_back(objPointer);
					}
				}
				else
				{
					for (auto& objPointer : *targetList)
					{
						if (objPointer != this && isCrashed(*objPointer, x, y))
							resultList.push_back(objPointer);
					}
				}
			}
		}
		return resultList;
	}

	template<suku_object Obj>
	inline Obj* Object::getCrashedObjectPrecisely(float _x, float _y, bool _isPredict)
	{
		if (!inRoom_)
			return nullptr;
		typename std::list<Obj*>* targetList = inRoom_->objectList<Obj>();
		if (targetList != nullptr)
		{
			if (_isPredict)
			{
				for (auto& objPointer : *targetList)
				{
					if (objPointer != this && isCrashed(*objPointer, _x, _y, objPointer->x, objPointer->y))
						return objPointer;
				}
			}
			else
			{
				for (auto& objPointer : *targetList)
				{
					if (objPointer != this && isCrashed(*objPointer, _x, _y))
						return objPointer;
				}
			}
		}
		return nullptr;
	}

	template<suku_object Obj>
	inline Object* Object::getCrashedObject(float _x, float _y, bool _isPredict)
	{
		if (!inRoom_)
			return nullptr;
		std::list<TypeNode*> targetTypeList = collisionInheritTree.getAllChildList<Obj>();
		std::list<Object*>* targetList;
		while (!targetTypeList.empty())
		{
			targetList = &(inRoom_->objectParentPointerArray[targetTypeList.front()->code]);
			targetTypeList.pop_front();
			if (targetList != nullptr)
			{
				if (_isPredict)
				{
					for (auto& objPointer : *targetList)
					{
						if (objPointer != this && isCrashed(*objPointer, _x, _y, objPointer->x, objPointer->y))
							return objPointer;
					}
				}
				else
				{
					for (auto& objPointer : *targetList)
					{
						if (objPointer != this && isCrashed(*objPointer, _x, _y))
							return objPointer;
					}
				}
			}
		}
		return nullptr;
	}

	template<suku_object Obj>
	inline std::list<Obj*> Object::getCrashedObjectListPrecisely(float _x, float _y, bool _isPredict)
	{
		typename std::list<Obj*> resultList;
		if (!inRoom_)
			return resultList;
		typename std::list<Obj*>* targetList = inRoom_->objectList<Obj>();
		if (targetList != nullptr)
		{
			if (_isPredict)
			{
				for (auto& objPointer : targetList)
				{
					if (objPointer != this && isCrashed(*objPointer, _x, _y, objPointer->x, objPointer->y))
						resultList.push_back(objPointer);
				}
			}
			else
			{
				for (auto& objPointer : targetList)
				{
					if (objPointer != this && isCrashed(*objPointer, _x, _y))
						resultList.push_back(objPointer);
				}
			}
		}
		return resultList;
	}

	template<suku_object Obj>
	inline std::list<Object*> Object::getCrashedObjectList(float _x, float _y, bool _isPredict)
	{
		if (!inRoom_)
			return std::list<Object*>();
		std::list<TypeNode*> targetTypeList = collisionInheritTree.getAllChildList<Obj>();
		std::list<Object*>* targetList;
		std::list<Object*> resultList;
		while (!targetTypeList.empty())
		{
			targetList = &(inRoom_->objectParentPointerArray[targetTypeList.front()->code]);
			targetTypeList.pop_front();
			if (targetList != nullptr)
			{
				if (_isPredict)
				{
					for (auto& objPointer : *targetList)
					{
						if (objPointer != this && isCrashed(*objPointer, _x, _y, objPointer->x, objPointer->y))
							resultList.push_back(objPointer);
					}
				}
				else
				{
					for (auto& objPointer : *targetList)
					{
						if (objPointer != this && isCrashed(*objPointer, _x, _y))
							resultList.push_back(objPointer);
					}
				}
			}
		}
		return resultList;
	}
}