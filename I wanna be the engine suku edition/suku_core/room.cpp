#include "room.h"
#include "object.h"

namespace suku
{
	Room* nowRoom;
	TypeTree collisionInheritTree;

	std::vector<std::pair<size_t, int>> reviseStateArray;
	std::vector<std::pair<size_t, int>> updateStateArray;
	std::vector<std::pair<size_t, int>> recheckStateArray;


	Room::Room()
	{
		hasCreated = false;
		objectPointerArray.clear();
		playerStartX = 0.0;
		playerStartY = 0.0;
	}

	void Room::setPlayerStart(float _x, float _y)
	{
		playerStartX = _x;
		playerStartY = _y;
	}

	Object* Room::findObj(size_t _kindId, size_t _pos)
	{
		size_t x = 0;
		Object* objPointer = nullptr;
		std::list<Object*>* targetList = objectList<Object>();
		for (auto& i : *targetList)
		{
			x++;
			if (x == _pos)
				return i;
		}
		return nullptr;
	}
	/*
	Object* Room::findObjWithPosition(int _kind, double _x, double _y)
	{
		double minDistance = 100000000.0;
		Object* result = nullptr;
		for (auto i = kindStart[_kind]; i != kindEnd[_kind]; i++)
		{
			if (getDistance((*i)->x, (*i)->y, _x, _y) < minDistance)
			{
				minDistance = getDistance((*i)->x, (*i)->y, _x, _y);
				result = (*i);
			}
		}
		return result;
	}

	Object* Room::findObjWithCenterPosition(int _kind, double _x, double _y)
	{
		double minDistance = 100000000.0;
		Object* result = nullptr;
		for (auto i = kindStart[_kind]; i != kindEnd[_kind]; i++)
		{
			if ((*i)->nowState() == nullptr)
				continue;
			if (getDistance((*i)->nowState()->centerX, (*i)->nowState()->centerY, _x, _y) < minDistance)
			{
				minDistance = getDistance((*i)->x, (*i)->y, _x, _y);
				result = (*i);
			}
		}
		return result;
	}*/


	void Room::update()
	{
		onUpdateStart();

		Object* previousObj = nullptr;
		std::list<Object*>* objectList;
		objectPointerArray[typecode(Object)] >> objectList;

		for (auto& obj : *objectList)
		{
			if (previousObj && previousObj->removeTag_)
			{
				previousObj->removeTag_ = false;
				previousObj->remove();
				if (previousObj->destroyTag_)
				{
					delete previousObj;
					previousObj = nullptr;
				}
			}
			obj->hspeedTemp = obj->vspeedTemp = 0;
			obj->var["xBefore"] << obj->x;
			obj->var["yBefore"] << obj->y;
			obj->isUpdating_ = true;
			obj->updateFunction();
			obj->isUpdating_ = false;
			previousObj = obj;
		}
		if (previousObj && previousObj->removeTag_)
		{
			previousObj->removeTag_ = false;
			previousObj->remove();
			if (previousObj->destroyTag_)
			{
				delete previousObj;
				previousObj = nullptr;
			}
		}

		for (auto& x : reviseStateArray)
			for (auto& obj : x.second)
			{
				if (previousObj && previousObj->removeTag_)
				{
					previousObj->removeTag_ = false;
					previousObj->remove();
					if (previousObj->destroyTag_)
					{
						delete previousObj;
						previousObj = nullptr;
					}
				}
				obj->isUpdating_ = true;
				obj->reviseState();
				obj->isUpdating_ = false;
				previousObj = obj;
			}
		if (previousObj && previousObj->removeTag_)
		{
			previousObj->removeTag_ = false;
			previousObj->remove();
			if (previousObj->destroyTag_)
			{
				delete previousObj;
				previousObj = nullptr;
			}
		}

		for (auto& x : updateStateArray)
			for (auto& obj : x.second)
			{
				if (previousObj && previousObj->removeTag_)
				{
					previousObj->removeTag_ = false;
					previousObj->remove();
					if (previousObj->destroyTag_)
					{
						delete previousObj;
						previousObj = nullptr;
					}
				}
				obj->isUpdating_ = true;
				obj->updateState();
				obj->isUpdating_ = false;
				previousObj = obj;
			}
		if (previousObj && previousObj->removeTag_)
		{
			previousObj->removeTag_ = false;
			previousObj->remove();
			if (previousObj->destroyTag_)
			{
				delete previousObj;
				previousObj = nullptr;
			}
		}

		for (auto& x : recheckStateArray)
			for (auto& obj : x.second)
			{
				if (previousObj && previousObj->removeTag_)
				{
					previousObj->removeTag_ = false;
					previousObj->remove();
					if (previousObj->destroyTag_)
					{
						delete previousObj;
						previousObj = nullptr;
					}
				}
				obj->isUpdating_ = true;
				obj->recheckState();
				obj->isUpdating_ = false;
				previousObj = obj;
			}
		if (previousObj && previousObj->removeTag_)
		{
			previousObj->removeTag_ = false;
			previousObj->remove();
			if (previousObj->destroyTag_)
			{
				delete previousObj;
				previousObj = nullptr;
			}
		}

		for (auto& obj : *objectList)
		{
			obj->x += obj->totalHspeed();
			obj->y += obj->totalVspeed();
		}

		onUpdateEnd();
	}

	void Room::paint()
	{
		onPaintStart();

		for (auto& x : paintArray)
			for (auto& obj : x.second)
			{
				if (!obj->onPaint())
					obj->paintBody();
			}

		onPaintEnd();
	}

	void Room::additionalFramePaint(float _offset)
	{
		for (auto& x : paintArray)
			for (auto& obj : x.second)
			{
				float deltaX = obj->x - obj->var["xBefore"].getValue<float>();
				float deltaY = obj->y - obj->var["yBefore"].getValue<float>();
				obj->paintBody(obj->var["xBefore"].getValue<float>() + deltaX * _offset,
					obj->var["yBefore"].getValue<float>() + deltaY * _offset);
			}
	}

	void Room::reset()
	{
		std::list<Object*>* allObjList = objectList<Object>();
		for (auto& objPointer : *allObjList)
		{
			objPointer->onRestarting();
		}
		//player->spawn();
	}

	/*
	void Room::paintBody()
	{
		int j;
		std::list<Object*>::iterator k;
		std::list<Object*> object_painting_array[VALUE_MAXPAINT];
		//for (j = 0; j < VALUE_MAXPAINT; j++)
		//	object_painting_array[j].clear();
		for (auto& i : objectPointerArray[typeid(Object).hash_code()])
		{
			Object* obj;
			i >> obj;
			object_painting_array[obj->paintId_].push_back(obj);
		}
		//object_painting_array[player->paintId_].push_back(player);
		for (j = 0; j < VALUE_MAXPAINT; j++)
			for (k = object_painting_array[j].begin(); k != object_painting_array[j].end(); k++)
				(*k)->paintBody();
	}*/

	void gotoRoom(Room& _room)
	{
		if (_room.hasCreated == false)
		{
			_room.onCreate();
			//_room.onJoin()...?
			nowRoom = &_room;
		}
		else
		{
			_room.onJoin();
			nowRoom = &_room;
		}
	}
}