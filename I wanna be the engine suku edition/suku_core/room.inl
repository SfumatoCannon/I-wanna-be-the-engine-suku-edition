#pragma once
#include "object.h"
#include "room.h"
#include "room_collision_pool.h"

namespace suku
{
	template<typename Obj>
	inline void Room::createObjectList()
	{
		objectPointerArray_.try_emplace(typecode(Obj), std::list<std::shared_ptr<Object>>());
	}

	template<typename Obj>
	inline std::list<Obj*> Room::getObjectList()
	{
		if (typecode(Obj) == typecode(Object))
		{
			// return all objects in the room
			std::list<Obj*> resultList;
			for (auto& [typecode, objectList] : objectPointerArray_)
			{
				for (auto& objectPtr : objectList)
				{
					if (objectPtr->removeTag_ == false)
						resultList.push_back(static_cast<Obj*>(objectPtr.get()));
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
				auto& targetList = (*resultIter).second;
				for (auto& objectPtr : targetList)
				{
					if (objectPtr->removeTag_ == false)
						resultList.push_back(static_cast<Obj*>(objectPtr.get()));
				}
			}
		}
		return resultList;
	}


	template<typename Obj>
	inline Obj* Room::append(Obj* _objectPointer)
	{
		auto [iter, isFirst] =
			objectPointerArray_.try_emplace(typecode(Obj), std::list<std::shared_ptr<Object>>());

		if (isFirst)
		{
			suku::SukuObjectTypeTree::getInstance().append<Obj>();
		}
		std::list<std::shared_ptr<Object>>& objList = (*iter).second;
		std::shared_ptr<Object> newObj(_objectPointer);
		newObj->kindId_ = typecode(Obj);
		newObj->inRoom_ = this;
		objList.push_back(newObj);

		reviseStateArray_[newObj->reviseStateId_].push_back(newObj);
		updateStateArray_[newObj->updateStateId_].push_back(newObj);
		recheckStateArray_[newObj->recheckStateId_].push_back(newObj);
		paintArray_[newObj->paintId_].push_back(newObj);

		collisionPool_->addObject(newObj.get());

		if (isFirst)
		{
			Obj::classInitialize();
		}

		return static_cast<Obj*>(newObj.get());
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
		_object.setPlaceAndSave(_object.x - _object.getSpriteFrame()->centerX, _object.y - _object.getSpriteFrame()->centerY);
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
		_object.setPlaceAndSave(_object.x - _object.getSpriteFrame()->centerX, _object.y - _object.getSpriteFrame()->centerY);
		createFill(_object, _fillwidth, _fillheight, _footx, _footy);
	}

	template<typename Obj>
	inline Obj* Room::getCrashedObject(Object* _sourceObj)
	{
		return collisionPool_->getCrashedObject<Obj>(_sourceObj);
	}

	template<typename Obj>
	inline std::list<Obj*> Room::getCrashedObjectList(Object* _sourceObj)
	{
		return collisionPool_->getCrashedObjectList<Obj>(_sourceObj);
	}
}