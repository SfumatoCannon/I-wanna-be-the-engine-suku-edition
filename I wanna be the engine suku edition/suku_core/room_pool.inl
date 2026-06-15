#include "room_pool.h"
#include "room.h"

namespace suku
{
	template<RoomType T>
	inline void RoomPool::releaseRoom()
	{
		Typecode typecode = typecode(T);
		roomPool_.erase(typecode);
	}

	template<RoomType T>
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

	template<RoomType T>
	inline void RoomPool::gotoRoom()
	{
		RoomPool::nowRoom_ = getRoom<T>();
		nowRoom_->onEntering();
	}
}
