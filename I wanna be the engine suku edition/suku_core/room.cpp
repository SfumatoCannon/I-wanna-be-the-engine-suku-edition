#include "room.h"
#include "object.h"

namespace suku
{
	Room* nowRoom;
	TypeTree collisionInheritTree;

	std::vector<std::pair<Typecode, int>> reviseStateArray;
	std::vector<std::pair<Typecode, int>> updateStateArray;
	std::vector<std::pair<Typecode, int>> recheckStateArray;


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

	Object* Room::findObj(Typecode _kindId, size_t _pos)
	{
		// WIP
		return nullptr;
	}

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