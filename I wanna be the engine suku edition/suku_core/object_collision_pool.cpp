#include "object_collision_pool.h"
#include "object.h"

namespace suku
{
    std::pair<int, int> ObjectCollisionPool::getChunk(float _x, float _y)
    {
        return { static_cast<int>(_x / ObjectCollisionPool::chunkSize),
            static_cast<int>(_y / ObjectCollisionPool::chunkSize) };
    }

    void ObjectCollisionPool::addObject(Object* _obj)
    {
        std::pair<int, int> chunk = getChunk(_obj->x, _obj->y);
        chunk_[chunk].push_back(_obj);
        objectChunk_[_obj] = chunk;
        objectIterator_[_obj] = --chunk_[chunk].end();
		objectUpdateTag_[_obj] = false;
    }

    void ObjectCollisionPool::removeObject(Object* _obj)
    {
		std::pair<int, int> chunk = objectChunk_[_obj];
        chunk_[chunk].erase(objectIterator_[_obj]);
        objectChunk_.erase(_obj);
		objectIterator_.erase(_obj);
    }

    void ObjectCollisionPool::updateObject(Object* _obj)
    {
        // no existence check for performance
		std::pair<int, int> newChunk = getChunk(_obj->x, _obj->y);
        if (newChunk != objectChunk_[_obj])
        {
            std::pair<int, int> oldChunk = objectChunk_[_obj];
            chunk_[oldChunk].erase(objectIterator_[_obj]);
            chunk_[newChunk].push_back(_obj);
            objectChunk_[_obj] = newChunk;
            objectIterator_[_obj] = --chunk_[newChunk].end();
		}
    }

  //  Object* ObjectCollisionPool::getCrashedObject(Object* _sourceObj)
  //  {
		//// temporary implementation, will be optimized later
		//for (auto& [chunk, list] : chunk_)
  //      {
  //          for (Object* obj : list)
  //          {
  //              if (obj != _sourceObj && _sourceObj->isCrashed(*obj))
  //                  return obj;
  //          }
  //      }
  //      return nullptr;
  //  }

  //  std::list<Object*> ObjectCollisionPool::getCrashedObjectList(Object* _sourceObj)
  //  {
  //      // temporary implementation, will be optimized later
		//std::list<Object*> resultList;
  //      for (auto& [chunk, list] : chunk_)
  //      {
  //          for (Object* obj : list)
  //          {
  //              if (obj != _sourceObj && _sourceObj->isCrashed(*obj))
  //                  resultList.push_back(obj);
  //          }
  //      }
		//return resultList;
  //  }
}