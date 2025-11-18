#pragma once
#include "object.h"
#include "room.h"

namespace suku
{
	template<typename Obj>
	inline void Room::createObjectList()
	{
		std::list<Object*> newList;
		objectPointerArray[typecode(Obj)] = newList;
	}

	template<typename Obj>
	inline std::list<Obj*> Room::getObjectList()
	{
		if (typecode(Obj) == typecode(Object))
		{
			// get all objects in objectPointerArray
			std::list<Obj*> resultList;
			for (auto& [kindId, objList] : objectPointerArray)
			{
				for (auto& object : objList)
				{
					resultList.push_back(static_cast<Obj*>(object));
				}
			}
			return resultList;
		}

		auto resultIter = objectPointerArray.find(typecode(Obj));
		if (resultIter != objectPointerArray.end())
		{
			std::list<Object*>& targetList = (*resultIter).second;
			auto& removeList = objectPointerRemoveArray[typecode(Obj)];
			if (!removeList.empty())
			{
				for (auto& i : removeList)
					targetList.erase(i);
				removeList.clear();
			}
			std::list<Obj*> resultList;
			for (auto& object : targetList)
			{
				resultList.push_back(static_cast<Obj*>(object));
			}
			return resultList;
		}
		else
		{
			return std::list<Obj*>();
		}
	}


	template<typename Obj>
	inline Obj* Room::append(Obj* _objectPointer)
	{
		Obj* newObj = _objectPointer;
		std::list<Obj*> objList = getObjectList<Obj>();
		bool isFirstObjInClass = false;
		if (objList.empty())
		{
			isFirstObjInClass = true;
			collisionInheritTree.append<Obj>();
			createObjectList<Obj>();
		}

		newObj->kindId_ = typecode(Obj);
		newObj->inRoom_ = this;

		newObj->objectIterator_ = objectPointerArray[typecode(Obj)].insert(
			objectPointerArray[typecode(Obj)].end(), static_cast<Object*>(newObj));

		newObj->reviseStateIterator_ = reviseStateArray[newObj->reviseStateId_].insert(
			reviseStateArray[newObj->reviseStateId_].end(), static_cast<Object*>(newObj));

		newObj->updateStateIterator_ = updateStateArray[newObj->updateStateId_].insert(
			updateStateArray[newObj->updateStateId_].end(), static_cast<Object*>(newObj));

		newObj->recheckStateIterator_ = recheckStateArray[newObj->recheckStateId_].insert(
			recheckStateArray[newObj->recheckStateId_].end(), static_cast<Object*>(newObj));

		newObj->paintIterator_ = paintArray[newObj->paintId_].insert(
			paintArray[newObj->paintId_].end(), static_cast<Object*>(newObj));

		if (isFirstObjInClass)
		{
			Obj::classInitialize();
		}

		return newObj;
	}

	template<typename Obj>
	inline Obj* Room::create(Obj& _object)
	{
		Obj* newObj = new Obj(_object);
		return append(newObj);
	}

	template<typename Obj>
	inline Obj* Room::create(Obj&& _object)
	{
		Obj* newObj = new Obj(std::move(_object));
		return append(newObj);
	}

	template<typename Obj, typename ...ObjNext>
	inline void Room::create(Obj _firstobject, ObjNext ..._objectnext)
	{
		create(_firstobject);
		create(_objectnext...);
	}

	template<typename Obj>
	inline void Room::createFill(Obj _object, float _fillwidth, float _fillheight, float _footx, float _footy)
	{
		float i, j;
		for (i = _object.x; i <= _object.x + _fillwidth; i += _footx)
			for (j = _object.y; j <= _object.y + _fillheight; j += _footy)
			{
				Obj a(_object);
				a.x = a.spawnX = i;
				a.y = a.spawnY = j;
				create(a);
			}
	}

	template<typename Obj>
	inline Obj* Room::createCenter(Obj _object)
	{
		_object.setPlaceAndSave(_object.x - _object.nowState()->centerX, _object.y - _object.nowState()->centerY);
		return create(_object);
	}

	template<typename Obj, typename ...ObjNext>
	inline void Room::createCenter(Obj _firstobject, ObjNext ..._objectnext)
	{
		createCenter(_firstobject);
		createCenter(_objectnext...);
	}

	template<typename Obj>
	inline void Room::createCenterFill(Obj _object, float _fillwidth, float _fillheight, float _footx, float _footy)
	{
		_object.setPlaceAndSave(_object.x - _object.nowState()->centerX, _object.y - _object.nowState()->centerY);
		createFill(_object, _fillwidth, _fillheight, _footx, _footy);
	}

	template<typename Father, typename Son>
	inline void linkCollisionType()
	{
		collisionInheritTree.unlink<Son>();
		collisionInheritTree.link<Father, Son>();
	}

	template<typename Son>
	void unlinkCollisionType()
	{
		collisionInheritTree.unlink<Son>();
	}
}