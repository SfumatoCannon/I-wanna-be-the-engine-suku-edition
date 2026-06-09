#pragma once
#include <map>
#include "room.h"


namespace suku
{
	template<typename RoomType>
	concept RoomType = std::derived_from<RoomType, Room>;

	class RoomPool
	{
	public:
		template<RoomType T> void createRoom();
		template<RoomType T> Room* getRoom();
	private:
		std::map<Typecode, std::unique_ptr<Room>> roomPool_;
	};
}