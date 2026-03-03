#pragma once
#include "room_collision_pool.h"
#include "object.h"

namespace suku
{
	template<typename Obj>
	Obj* RoomCollisionPool::getCrashedObject(Object* _sourceObj)
	{
		for (auto& [chunk, list] : chunk_)
		{
			for (Object* obj : list)
			{
				if (Obj* castedObj = dynamic_cast<Obj*>(obj))
				{
					if (obj != _sourceObj && _sourceObj->isCrashed(*castedObj))
						return castedObj;
				}
			}
		}
		return nullptr;
	}

	template<typename Obj>
	std::list<Obj*> RoomCollisionPool::getCrashedObjectList(Object* _sourceObj)
	{
		std::list<Obj*> resultList;
		for (auto& [chunk, list] : chunk_)
		{
			for (Object* obj : list)
			{
				if (Obj* castedObj = dynamic_cast<Obj*>(obj))
				{
					if (obj != _sourceObj && _sourceObj->isCrashed(*castedObj))
						resultList.push_back(castedObj);
				}
			}
		}
		return resultList;
	}
}