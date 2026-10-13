#include "room_pool.h"

namespace suku
{
	Room* getNowRoom()
	{
		return RoomPool::getNowRoom();
	}

	void RoomPool::gotoRoom(Typecode _roomid)
	{
		RoomTypecodeManager::gotoRoom(_roomid);
	}

	void RoomPool::gotoNewGameRoom()
	{
		pAction_ = &actionOnNewGame_;
	}

	void RoomPool::gotoTitleRoom()
	{
		pAction_ = &actionOnTitle_;
	}

	void RoomPool::execute()
	{
		if (pAction_)
		{
			(*pAction_)();
			pAction_ = nullptr;
		}
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
		RoomPool::pAction_ = &(*iter).second;
	}
}