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
		static Room* getNowRoom() { return nowRoom_; }
		template<suku_room T> static void releaseRoom();
		template<suku_room T> static Room* getRoom();
		template<suku_room T> static void gotoRoom();
	private:
		inline static std::map<Typecode, std::unique_ptr<Room>> roomPool_;
		inline static Room* nowRoom_ = nullptr;
	};

	template<suku_room T> Room* getNowRoom() { return RoomPool::getNowRoom(); }
	template<suku_room T> void gotoRoom() { RoomPool::gotoRoom<T>(); }

}

#include "room_pool.inl"