#include "room.h"
#include "object.h"
#include "../suku_draw/effect.h"
#include "../suku_draw/draw_core.h"
#include "../suku_foundation/game_window.h"
#include "room_collision_pool.h"

namespace suku
{
	Room* nowRoom;
	
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
		_object->removeTag_ = true;
	}

	void Room::setObjectRevisePriority(Object* _object, double _newId)
	{
		auto& originalArray = reviseStateArray_[_object->reviseStateId_];
		auto& targetArray = reviseStateArray_[_newId];
		for (auto iter = originalArray.begin(); iter != originalArray.end(); iter++)
		{
			if ((*iter).get() == _object)
			{
				targetArray.push_back(std::move(*iter));
				originalArray.erase(iter);
				_object->reviseStateId_ = _newId;
				return;
			}
		}
		// not found in original array
		targetArray.push_back(std::shared_ptr<Object>(_object));
		_object->reviseStateId_ = _newId;
	}

	void Room::setObjectUpdatePriority(Object* _object, double _newId)
	{
		auto& originalArray = updateStateArray_[_object->updateStateId_];
		auto& targetArray = updateStateArray_[_newId];
		for (auto iter = originalArray.begin(); iter != originalArray.end(); iter++)
		{
			if ((*iter).get() == _object)
			{
				targetArray.push_back(std::move(*iter));
				originalArray.erase(iter);		
				_object->updateStateId_ = _newId;
				break;
			}
		}
		// not found in original array
		targetArray.push_back(std::shared_ptr<Object>(_object));
		_object->updateStateId_ = _newId;
	}

	void Room::setObjectRecheckPriority(Object* _object, double _newId)
	{
		auto& originalArray = recheckStateArray_[_object->recheckStateId_];
		auto& targetArray = recheckStateArray_[_newId];
		for (auto iter = originalArray.begin(); iter != originalArray.end(); iter++)
		{
			if ((*iter).get() == _object)
			{
				targetArray.push_back(std::move(*iter));
				originalArray.erase(iter);
				_object->recheckStateId_ = _newId;
				break;
			}
		}
		// not found in original array
		targetArray.push_back(std::shared_ptr<Object>(_object));
		_object->recheckStateId_ = _newId;
	}

	void Room::setObjectPaintPriority(Object* _object, double _newId)
	{
		auto& originalArray = paintArray_[_object->paintId_];
		auto& targetArray = paintArray_[_newId];
		for (auto iter = originalArray.begin(); iter != originalArray.end(); iter++)
		{
			if ((*iter).get() == _object)
			{
				targetArray.push_back(std::move(*iter));
				originalArray.erase(iter);
				_object->paintId_ = _newId;
				break;
			}
		}
		// not found in original array
		targetArray.push_back(std::shared_ptr<Object>(_object));
		_object->paintId_ = _newId;
	}

	void Room::update()
	{
		onUpdateStart();

		for (auto& [type, objArray] : objectPointerArray_)
		{
			for (auto iter = objArray.begin(); iter != objArray.end();)
			{
				Object* obj = (*iter).get();
				if (obj->removeTag_)
				{
					collisionPool_->removeObject(obj);
					if (obj->destroyTag_)
					{
						iter = objArray.erase(iter);
					}
				}
				else
				{
					obj->hspeedTemp = obj->vspeedTemp = 0;
					obj->var["xBefore"] << obj->x;
					obj->var["yBefore"] << obj->y;
					obj->isUpdating_ = true;
					obj->updateFunction();
					obj->isUpdating_ = false;

					iter++;
				}
			}
		}

		for (auto& [type, objArray] : reviseStateArray_)
		{
			for (auto iter = objArray.begin(); iter != objArray.end();)
			{
				Object* obj = (*iter).get();
				if (obj->removeTag_)
				{
					iter = objArray.erase(iter);
				}
				else
				{
					obj->isUpdating_ = true;
					obj->reviseState();
					obj->isUpdating_ = false;

					iter++;
				}
			}
		}

		for (auto& [type, objArray] : updateStateArray_)
		{
			for (auto iter = objArray.begin(); iter != objArray.end();)
			{
				Object* obj = (*iter).get();
				if (obj->removeTag_)
				{
					iter = objArray.erase(iter);
				}
				else
				{
					obj->isUpdating_ = true;
					obj->updateState();
					obj->isUpdating_ = false;

					iter++;
				}
			}
		}

		for (auto& [type, objArray] : recheckStateArray_)
		{
			for (auto iter = objArray.begin(); iter != objArray.end();)
			{
				Object* obj = (*iter).get();
				if (obj->removeTag_)
				{
					iter = objArray.erase(iter);
				}
				else
				{
					obj->isUpdating_ = true;
					obj->recheckState();
					obj->isUpdating_ = false;

					iter++;
				}
			}
		}

		for (auto& [type, objArray] : objectPointerArray_)
		{
			for (auto& obj : objArray)
			{
				obj->x += obj->totalHspeed();
				obj->y += obj->totalVspeed();
			}
		}

		onUpdateEnd();
	}

	void Room::paint()
	{
		displayLayer_.beginDraw();
		onPaintStart();

		for (auto& [type, objArray] : paintArray_)
		{
			for (auto iter = objArray.begin(); iter != objArray.end();)
			{
				Object* obj = (*iter).get();
				if (obj->removeTag_)
				{
					iter = objArray.erase(iter);
				}
				else
				{
					obj->isUpdating_ = true;
					if (!obj->onPaint())
						obj->paintBody();
					obj->isUpdating_ = false;

					iter++;
				}
			}
		}

		onPaintEnd();
		auto pic = displayLayer_.endDraw();
		static EffectTransform scaleEffect(ScaleMode::HighQualityCubic, false);
		scaleEffect.setTransform(GameWindow::getPixelMappingTransform());
		scaleEffect.setInput(pic);
		scaleEffect.paint();
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