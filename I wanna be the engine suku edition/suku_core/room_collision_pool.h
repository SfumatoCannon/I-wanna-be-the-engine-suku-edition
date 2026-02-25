#pragma once
#include "room.h"
#include "object.h"
#include <map>
#include <utility>

namespace suku
{
	class RoomCollisionPool
	{
	public:
		static constexpr int chunkSize = 128;
		static std::pair<int, int> getChunk(float _x, float _y);

		RoomCollisionPool() = default;
		void addObject(Object* _obj);
		void removeObject(Object* _obj);
		void updateObject(Object* _obj);
		//template<typename T> std::list<T*> getObjectsInSameChunk(Object* _sourceObj);
		//std::list<Object*> getObjectsInSameChunk(Object* _sourceObj);
		//template<typename T> std::list<T*> getCrashedObject(Object* _sourceObj);
		//std::list<Object*> getCrashedObject(Object* _sourceObj);
	private:
		std::map<std::pair<int, int>, std::list<Object*>> chunk_;
		std::map<Object*, std::pair<int, int>> objectChunk_;
		std::map<Object*, std::list<Object*>::iterator> objectIterator_;
		std::map<Object*, bool> objectUpdateTag_;
		std::list<Object*> oversizedObjectList_;
	};
}