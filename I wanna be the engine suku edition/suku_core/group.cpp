#include "group.h"
#include "object.h"
#include "room.h"

namespace suku
{
	Group Group::operator&(Group& _group)
	{
		Group result;
		for (auto& i : member_)
			if (_group.isInclude(i))
				result.append(i);
		return result;
	}

	Group Group::operator|(Group& _group)
	{
		Group result;
		for (auto& i : member_)
			result.append(i);
		for (auto& i : _group.member_)
			result.append(i);
		return result;
	}

	Group Group::operator^(Group& _group)
	{
		Group result;
		for (auto& i : member_)
			result.append(i);
		for (auto& i : _group.member_)
		{
			if (isInclude(i))
				result.removeObj(i);
			else result.append(i);
		}
		return result;
	}

	size_t Group::getSize()
	{
		return member_.size();
	}

	size_t Group::append(Object* _object)
	{
		if (!isInclude(_object))
		{
			memberIterator_[_object] = member_.insert(member_.end(), _object);
		}
		return member_.size();
	}

	size_t Group::append(Object* _firstObject, Object* _secondObject, ...)
	{
		va_list objArg;
		va_start(objArg, _secondObject);
		append(_firstObject);
		append(_secondObject);
		Object* obj;
		while (1)
		{
			obj = va_arg(objArg, Object*);
			if (obj == nullptr)
				break;
			append(obj);
		}
		va_end(objArg);
		return member_.size();
	}

	size_t Group::append(std::list<Object*> _objectList)
	{
		for (auto& obj : _objectList)
			append(obj);
		return member_.size();
	}
	/*
	size_t Group::appendSelect(Room* _room, std::function<bool(Object*)> _function, int _kind)
	{
		for (auto i = _room->kindStart[_kind]; i != _room->kindEnd[_kind]; i++)
			if ((*i)->id != 0 && !id_[*i] && _function(*i))
			{
				member_.push_back(*i);
				id_[*i] = member_.size();
			}
		return member_.size();
	}*/

	size_t Group::appendSelect(Room* _room, std::function<bool(Object*)> _function)
	{
		std::list<Object*> objectList = _room->getObjectList<Object>();
		for (auto& objPointer : objectList)
		{
			if (_function(objPointer))
				append(objPointer);
		}
		return member_.size();
	}

	/*size_t Group::appendSelect(Group* _group, std::function<bool(Object*)> _function, int _kind)
	{
		size_t size = _group->getSize();
		Object* obj;
		for (int i = 1; i <= size; i++)
		{
			obj = _group->findObjWithGroupId(i);
			if (obj->kind == _kind && _function(obj))
				create(obj);
		}
		return member_.size();
	}*/

	size_t Group::appendSelect(Group* _group, std::function<bool(Object*)> _function)
	{
		size_t size = _group->getSize();
		Object* obj;
		for (size_t i = 1; i <= size; i++)
		{
			obj = _group->findObj(i);
			if (_function(obj))
				append(obj);
		}
		return member_.size();
	}

	size_t Group::appendSelect(std::list<Object*> _objectList, std::function<bool(Object*)> _function)
	{
		for (auto& obj : _objectList)
		{
			if (_function(obj))
				append(obj);
		}
		return member_.size();
	}

	bool Group::isInclude(Object* _object)
	{
		return memberIterator_.find(_object) != memberIterator_.end();
	}

	void Group::foreach(std::function<void(Object*)> _function)
	{
		for (auto i = member_.begin(); i != member_.end(); i++)
		{
			if (i != member_.end())
				_function(*i);
		}
	}

	void Group::removeObj(size_t _pos)
	{
		removeObj(findObj(_pos));
	}

	void Group::removeObj(Object* _object)
	{
		auto targetNode = memberIterator_.find(_object);
		if (targetNode != memberIterator_.end())
		{
			std::list<Object*>::iterator objectIterator = memberIterator_[_object];
			member_.erase(objectIterator);
			memberIterator_.erase(targetNode);
		}
	}

	Object* Group::findObj(size_t _pos)
	{
		size_t counter = 0;
		for (auto& obj : member_)
		{
			counter++;
			if (counter == _pos)
				return obj;
		}
		return nullptr;
	}

	void Group::setAction(std::function<void(Object*, int)> _function)
	{
		int j = 0;
		for (auto i = member_.begin(); i != member_.end(); i++)
		{
			j++;
			if (i != member_.end())
				_function(*i, j);
		}
	}
}