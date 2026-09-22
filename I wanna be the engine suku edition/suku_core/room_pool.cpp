#include "room_pool.h"

namespace suku
{
	Room* getNowRoom()
	{
		return RoomPool::getNowRoom();
	}

	void RoomPool::gotoNewGameRoom()
	{
		actionOnNewGame_();
	}

	bool RoomTypecodeManager::isExist(Typecode _typecode)
	{
		return gotoRoomExecuteMap_.find(_typecode) != gotoRoomExecuteMap_.end();
	}

	void RoomTypecodeManager::gotoRoom(Typecode _typecode)
	{
		auto iter = gotoRoomExecuteMap_.find(_typecode);
		if (iter == gotoRoomExecuteMap_.end())
		{
			ERRORWINDOW_GLOBAL(L"Failed to goto room: room typecode is invalid (" + std::to_wstring(_typecode) + L")");
			return;
		}
		(*iter).second();
	}
}