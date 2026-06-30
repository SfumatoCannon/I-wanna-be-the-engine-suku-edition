#include "room_pool.h"
#include "room.h"

namespace suku
{
	template<suku_room T>
	inline void RoomPool::releaseRoom()
	{
		Typecode typecode = typecode(T);
		roomPool_.erase(typecode);
	}

	template<suku_room T>
	Room* RoomPool::getRoom()
	{
		Typecode typecode = typecode(T);
		auto it = roomPool_.find(typecode);
		if (it != roomPool_.end())
		{
			return it->second.get();
		}
		roomPool_[typecode] = std::make_unique<T>();
		return roomPool_[typecode].get();
	}

	template<suku_room T>
	inline void RoomPool::gotoRoom()
	{
		RoomPool::nowRoom_ = getRoom<T>();
		nowRoom_->onEntering();
	}
}
