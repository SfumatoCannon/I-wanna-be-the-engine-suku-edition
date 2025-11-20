#pragma once
#include "object.h"
#include "room.h"

namespace suku
{
	template<typename Obj>
	inline void Room::createObjectList()
	{
		std::list<Object*> newList;
		objectPointerArray_[typecode(Obj)] = newList;
	}

	template<typename Obj>
	inline std::list<Obj*> Room::getObjectList()
	{
		if (typecode(Obj) == typecode(Object))
		{
			// get all objects in objectPointerArray_
			std::list<Obj*> resultList;
			for (auto& [kindId, objList] : objectPointerArray_)
			{
				for (auto& object : objList)
				{
					resultList.push_back(static_cast<Obj*>(object));
				}
			}
			return resultList;
		}

		std::list<Typecode> allTypeList = suku::SukuObjectTypeTree::getInstance().getAllChildrenList<Obj>();
		std::list<Obj*> resultList;
		for (auto typecode : allTypeList)
		{
			auto resultIter = objectPointerArray_.find(typecode);
			if (resultIter != objectPointerArray_.end())
			{
				std::list<Object*>& targetList = (*resultIter).second;
				auto& removeList = objectPointerRemoveArray_[typecode];
				if (!removeList.empty())
				{
					for (auto& i : removeList)
						targetList.erase(i);
					removeList.clear();
				}
				for (auto& object : targetList)
				{
					resultList.push_back(static_cast<Obj*>(object));
				}
			}
		}
		return resultList;
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
			createObjectList<Obj>();
			suku::SukuObjectTypeTree::getInstance().append<Obj>();
		}

		newObj->kindId_ = typecode(Obj);
		newObj->inRoom_ = this;

		newObj->objectIterator_ = objectPointerArray_[typecode(Obj)].insert(
			objectPointerArray_[typecode(Obj)].end(), static_cast<Object*>(newObj));

		newObj->reviseStateIterator_ = reviseStateArray_[newObj->reviseStateId_].insert(
			reviseStateArray_[newObj->reviseStateId_].end(), static_cast<Object*>(newObj));

		newObj->updateStateIterator_ = updateStateArray_[newObj->updateStateId_].insert(
			updateStateArray_[newObj->updateStateId_].end(), static_cast<Object*>(newObj));

		newObj->recheckStateIterator_ = recheckStateArray_[newObj->recheckStateId_].insert(
			recheckStateArray_[newObj->recheckStateId_].end(), static_cast<Object*>(newObj));

		newObj->paintIterator_ = paintArray_[newObj->paintId_].insert(
			paintArray_[newObj->paintId_].end(), static_cast<Object*>(newObj));

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
}