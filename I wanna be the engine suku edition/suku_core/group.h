#pragma once
#include <list>
#include <map>
#include <functional>

namespace suku
{
	class Object;
	class Room;

	class Group
	{
	public:
		Group operator&(Group& _group);
		Group operator|(Group& _group);
		Group operator^(Group& _group);
		size_t getSize();
		size_t append(Object* _object);
		size_t append(Object* _firstObject, Object* _secondObject, ...);
		size_t append(std::list<Object*> _objectList);
		size_t appendSelect(Room* _room, std::function<bool(Object*)> _function);
		size_t appendSelect(Group* _group, std::function<bool(Object*)> _function);
		size_t appendSelect(std::list<Object*> _objectList, std::function<bool(Object*)> _function);
		bool isInclude(Object* _object);
		void foreach(std::function<void(Object*)> _function);
		void removeObj(size_t _pos);
		void removeObj(Object* _obj);
		Object* findObj(size_t _pos);
		void setAction(std::function<void(Object*, int)> _function);
	private:
		std::list<Object*> member_;
		std::map<Object*, std::list<Object*>::iterator> memberIterator_;
	};
}