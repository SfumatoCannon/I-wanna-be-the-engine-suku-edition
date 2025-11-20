#include "room.h"
#include "object.h"

namespace suku
{
	Room* nowRoom;

	std::vector<std::pair<Typecode, int>> reviseStateArray_;
	std::vector<std::pair<Typecode, int>> updateStateArray_;
	std::vector<std::pair<Typecode, int>> recheckStateArray_;


	Room::Room()
	{
		hasCreated = false;
		objectPointerArray_.clear();
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

	void Room::remove(Object* _object)
	{
		if (_object->isUpdating_)
		{
			_object->removeTag_ = true;
			return;
		}
		else
		{
			objectPointerRemoveArray_[_object->kindId_].push_back(_object->objectIterator_);
			objectPointerArray_[_object->kindId_].erase(_object->objectIterator_);
			reviseStateArray_[_object->reviseStateId_].erase(_object->reviseStateIterator_);
			updateStateArray_[_object->updateStateId_].erase(_object->updateStateIterator_);
			recheckStateArray_[_object->recheckStateId_].erase(_object->recheckStateIterator_);
			paintArray_[_object->paintId_].erase(_object->paintIterator_);
		}
	}

	void Room::modifyObjectRevisePriority(Object* _object, double _newId)
	{
		reviseStateArray_[_object->reviseStateId_].erase(_object->reviseStateIterator_);
		_object->reviseStateId_ = _newId;
		_object->reviseStateIterator_ =
			reviseStateArray_[_newId].insert(reviseStateArray_[_newId].end(), _object);
	}

	void Room::modifyObjectUpdatePriority(Object* _object, double _newId)
	{
		updateStateArray_[_object->updateStateId_].erase(_object->updateStateIterator_);
		_object->updateStateId_ = _newId;
		_object->updateStateIterator_ =
			updateStateArray_[_newId].insert(updateStateArray_[_newId].end(), _object);
	}

	void Room::modifyObjectRecheckPriority(Object* _object, double _newId)
	{
		recheckStateArray_[_object->recheckStateId_].erase(_object->recheckStateIterator_);
		_object->recheckStateId_ = _newId;
		_object->recheckStateIterator_ =
			recheckStateArray_[_newId].insert(recheckStateArray_[_newId].end(), _object);
	}

	void Room::modifyObjectPaintPriority(Object* _object, double _newId)
	{
		paintArray_[_object->paintId_].erase(_object->paintIterator_);
		_object->paintId_ = _newId;
		_object->paintIterator_ =
			paintArray_[_newId].insert(paintArray_[_newId].end(), _object);
	}

	void Room::update()
	{
		onUpdateStart();

		Object* previousObj = nullptr;

		for (auto& obj : getObjectList<Object>())
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

		for (auto& x : reviseStateArray_)
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

		for (auto& x : updateStateArray_)
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

		for (auto& x : recheckStateArray_)
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

		for (auto& obj : getObjectList<Object>())
		{
			obj->x += obj->totalHspeed();
			obj->y += obj->totalVspeed();
		}

		onUpdateEnd();
	}

	void Room::paint()
	{
		onPaintStart();

		for (auto& x : paintArray_)
			for (auto& obj : x.second)
			{
				if (!obj->onPaint())
					obj->paintBody();
			}

		onPaintEnd();
	}

	void Room::additionalFramePaint(float _offset)
	{
		for (auto& x : paintArray_)
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
		std::list<Object*> allObjList = getObjectList<Object>();
		for (auto& objPointer : allObjList)
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
		for (auto& i : objectPointerArray_[typeid(Object).hash_code()])
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