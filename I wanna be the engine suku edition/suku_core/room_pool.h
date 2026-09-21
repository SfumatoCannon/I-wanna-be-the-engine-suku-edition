#pragma once
#include <map>
#include <suku_foundation/suku_type.h>
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
		template<suku_room T> static void setNewGameRoom();
		static void gotoNewGameRoom();
	private:
		inline static std::map<Typecode, std::unique_ptr<Room>> roomPool_;
		inline static Room* nowRoom_ = nullptr;
		inline static SoundController* BGMController_ = nullptr;
		inline static Sound* nowBGM_ = nullptr;
		inline static std::function<void()> actionOnNewGame_;
	};

	class RoomTypecodeManager
	{
	public:
		template<suku_room T> static void registerRoom();
		static void gotoRoom(Typecode _typecode);
	private:
		inline static std::map<Typecode, std::function<void()>> gotoRoomExecuteMap_;
		RoomTypecodeManager() = default;
	};

	template<suku_room T>
	class RoomTypecodeRegisterClass
	{
	public:
		RoomTypecodeRegisterClass() { RoomTypecodeManager::registerRoom<T>(); }
	};

#define ROOM_SAVABLE(suku_room) static inline RoomTypecode<suku_room> savable_registered;

	Room* getNowRoom();
	template<suku_room T> inline void gotoRoom() { RoomPool::gotoRoom<T>(); }
	inline void gotoRoom(Typecode _roomTypecode) { RoomTypecodeManager::gotoRoom(_roomTypecode); }
}

#include "room_pool.inl"