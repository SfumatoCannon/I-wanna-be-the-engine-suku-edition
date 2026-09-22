#include "room_pool.h"
#include "room.h"
#include <suku_core/tile.h>
#include <suku_foundation/save.h>


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
		roomPool_[typecode]->name_ = typeid(T).name();
		roomPool_[typecode]->id_ = typecode(T);
		return roomPool_[typecode].get();
	}

	template<suku_room T>
	inline Room* RoomPool::gotoRoom()
	{
		Tile::resetAll();
		RoomPool::nowRoom_ = getRoom<T>();
		if (nowRoom_->hasBGM())
		{
			if (nowRoom_->getBGM() == nullptr)
			{
				if (BGMController_ != nullptr)
				{
					BGMController_->stop();
					BGMController_ = nullptr;
				}
				nowBGM_ = nullptr;
			}
			else if (nowBGM_ != nowRoom_->getBGM())
			{
				if (BGMController_ != nullptr)
				{
					BGMController_->stop();
					BGMController_ = nullptr;
				}
				BGMController_ = nowRoom_->getBGM()->playWithLoop();
				nowBGM_ = nowRoom_->getBGM();
			}
		}
		nowRoom_->enter();
		return nowRoom_;
	}

	template<suku_room T>
	inline void RoomPool::setNewGameRoom()
	{
		newgameRoomId_ = typecode(T);
		actionOnNewGame_ = [=]() {
			SaveFile* savefile = getGlobalSaveFile();
			if (savefile && savefile->isExist())
			{
				savefile->clear();
			}
			RoomPool::gotoRoom<T>();
			SaveFile::save();
		};
	}

	template<suku_room T>
	inline void RoomTypecodeManager::registerRoom()
	{
		gotoRoomExecuteMap_.emplace(typecode(T), []() { RoomPool::gotoRoom<T>(); });
	}
}
