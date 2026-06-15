#pragma once
#include <map>
#include <suku_foundation/type_tree.h>
#include <memory>
#include "room.h"


namespace suku
{
	class Room;

	class RoomPool
	{
	public:
		template<RoomType T> static Room* getRegisteredRoom();
		template<RoomType T> static void releaseRoom();
		template<RoomType T> static Room* getRoom();
	private:
		inline static std::map<Typecode, std::unique_ptr<Room>> roomPool_;
	};

	template<RoomType T> void gotoRoom();
}

//void gotoRoom(Room& _room)
//{
//	if (_room.hasCreated == false)
//	{
//		_room.onCreate();
//		//_room.onEntering()...?
//		nowRoom = &_room;
//	}
//	else
//	{
//		_room.onEntering();
//		nowRoom = &_room;
//	}
//}

#include "room_pool.inl"