#include "room.h"
#include "object.h"
#include "../suku_draw/effect.h"
#include "../suku_draw/draw_core.h"
#include "../suku_foundation/game_window.h"
#include "room_collision_pool.h"

namespace suku
{
	Room* nowRoom;

	Object* Room::findObj(Typecode _kindId, size_t _pos)
	{
		// WIP
		return nullptr;
	}

	void Room::remove(Object* _object)
	{
		_object->removeTag_ = true;
	}

	void Room::destroy(Object* _object)
	{
		_object->removeTag_ = true;
		_object->destroyTag_ = true;
	}

	void Room::removeImmediately(Object* _object)
	{
		collisionPool_->removeObject(_object);
		preUpdateArray_[_object->preUpdateId()].remove_if([_object](std::shared_ptr<Object>& objPtr)
			{ return objPtr.get() == _object; });
		updateArray_[_object->updateId()].remove_if([_object](std::shared_ptr<Object>& objPtr)
			{ return objPtr.get() == _object; });
		postUpdateArray_[_object->postUpdateId()].remove_if([_object](std::shared_ptr<Object>& objPtr)
			{ return objPtr.get() == _object; });
		paintArray_[_object->paintId()].remove_if([_object](std::shared_ptr<Object>& objPtr)
			{ return objPtr.get() == _object; });
	}

	void Room::destroyImmediately(Object* _object)
	{
		collisionPool_->removeObject(_object);
		objectPointerArray_[_object->kindId()].remove_if([_object](std::shared_ptr<Object>& objPtr)
			{ return objPtr.get() == _object; });
		preUpdateArray_[_object->preUpdateId()].remove_if([_object](std::shared_ptr<Object>& objPtr)
			{ return objPtr.get() == _object; });
		updateArray_[_object->updateId()].remove_if([_object](std::shared_ptr<Object>& objPtr)
			{ return objPtr.get() == _object; });
		postUpdateArray_[_object->postUpdateId()].remove_if([_object](std::shared_ptr<Object>& objPtr)
			{ return objPtr.get() == _object; });
		paintArray_[_object->paintId()].remove_if([_object](std::shared_ptr<Object>& objPtr)
			{ return objPtr.get() == _object; });
	}

	void Room::setObjectRevisePriority(Object* _object, double _newId)
	{
		auto& originalArray = preUpdateArray_[_object->preUpdateId_];
		auto& targetArray = preUpdateArray_[_newId];
		for (auto iter = originalArray.begin(); iter != originalArray.end(); iter++)
		{
			if ((*iter).get() == _object)
			{
				targetArray.push_back(std::move(*iter));
				originalArray.erase(iter);
				_object->preUpdateId_ = _newId;
				return;
			}
		}
		// not found in original array
		targetArray.push_back(std::shared_ptr<Object>(_object));
		_object->preUpdateId_ = _newId;
	}

	void Room::setObjectUpdatePriority(Object* _object, double _newId)
	{
		auto& originalArray = updateArray_[_object->updateId_];
		auto& targetArray = updateArray_[_newId];
		for (auto iter = originalArray.begin(); iter != originalArray.end(); iter++)
		{
			if ((*iter).get() == _object)
			{
				targetArray.push_back(std::move(*iter));
				originalArray.erase(iter);		
				_object->updateId_ = _newId;
				break;
			}
		}
		// not found in original array
		targetArray.push_back(std::shared_ptr<Object>(_object));
		_object->updateId_ = _newId;
	}

	void Room::setObjectRecheckPriority(Object* _object, double _newId)
	{
		auto& originalArray = postUpdateArray_[_object->postUpdateId_];
		auto& targetArray = postUpdateArray_[_newId];
		for (auto iter = originalArray.begin(); iter != originalArray.end(); iter++)
		{
			if ((*iter).get() == _object)
			{
				targetArray.push_back(std::move(*iter));
				originalArray.erase(iter);
				_object->postUpdateId_ = _newId;
				break;
			}
		}
		// not found in original array
		targetArray.push_back(std::shared_ptr<Object>(_object));
		_object->postUpdateId_ = _newId;
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
					obj->updateFunction();

					iter++;
				}
			}
		}

		for (auto& [type, objArray] : preUpdateArray_)
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
					obj->preUpdate();

					iter++;
				}
			}
		}

		for (auto& [type, objArray] : updateArray_)
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
					obj->update();

					iter++;
				}
			}
		}

		for (auto& [type, objArray] : objectPointerArray_)
		{
			for (auto& obj : objArray)
			{
				obj->var["xBefore"] << obj->x;
				obj->var["yBefore"] << obj->y;
				obj->x += obj->totalHspeed();
				obj->y += obj->totalVspeed();
			}
		}

		for (auto& [type, objArray] : postUpdateArray_)
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
					obj->postUpdate();

					iter++;
				}
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
					if (!obj->onPaint())
						obj->paintBody();

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

	void Room::additionalFramePaint(float _offsetRate)
	{
		displayLayer_.beginDraw();
		onPaintStart();

		for (auto& x : paintArray_)
			for (auto& obj : x.second)
			{
				obj->additionalFramePaint(_offsetRate);
			}

		onPaintEnd();
		auto pic = displayLayer_.endDraw();
		static EffectTransform scaleEffect(ScaleMode::HighQualityCubic, false);
		scaleEffect.setTransform(GameWindow::getPixelMappingTransform());
		scaleEffect.setInput(pic);
		scaleEffect.paint();
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
			//_room.onEntering()...?
			nowRoom = &_room;
		}
		else
		{
			_room.onEntering();
			nowRoom = &_room;
		}
	}
}