#pragma once
#include "../framework.h"

namespace suku
{
	class Object;
	class Room;

	class Group
	{
	private:
		std::list<Object*> member_;
		std::map<Object*, std::list<Object*>::iterator> memberIterator_;
	public:
		Group operator&(Group& _group);
		Group operator|(Group& _group);
		Group operator^(Group& _group);
		size_t getSize();
		size_t append(Object* _object);
		size_t append(Object* _firstObject, Object* _secondObject, ...);
		size_t append(std::list<Object*> _objectList);
		size_t appendSelectively(Room* _room, std::function<bool(Object*)> _function);
		size_t appendSelectively(Group* _group, std::function<bool(Object*)> _function);
		size_t appendSelectively(std::list<Object*> _objectList, std::function<bool(Object*)> _function);
		bool isInclude(Object* _object);
		void removeObj(size_t _pos);
		void removeObj(Object* _obj);
		Object* findObj(size_t _pos);
		void setAction(std::function<void(Object*, int)> _function);
	};
}