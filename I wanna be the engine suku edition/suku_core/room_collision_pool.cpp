#include "room_collision_pool.h"

namespace suku
{
    std::pair<int, int> RoomCollisionPool::getChunk(float _x, float _y)
    {
        return { static_cast<int>(_x / RoomCollisionPool::chunkSize),
            static_cast<int>(_y / RoomCollisionPool::chunkSize) };
    }

    void RoomCollisionPool::addObject(Object* _obj)
    {
        std::pair<int, int> chunk = getChunk(_obj->x, _obj->y);
        chunk_[chunk].push_back(_obj);
        objectChunk_[_obj] = chunk;
        objectIterator_[_obj] = --chunk_[chunk].end();
		objectUpdateTag_[_obj] = false;
    }

    void RoomCollisionPool::removeObject(Object* _obj)
    {
		std::pair<int, int> chunk = objectChunk_[_obj];
        chunk_[chunk].erase(objectIterator_[_obj]);
        objectChunk_.erase(_obj);
		objectIterator_.erase(_obj);
    }
}