#pragma once
#include "object_collision_pool.h"
#include "object.h"

namespace suku
{
	template<typename Obj>
	Obj* ObjectCollisionPool::getCrashedObject(Object* _sourceObj)
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
	std::list<Obj*> ObjectCollisionPool::getCrashedObjectList(Object* _sourceObj)
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