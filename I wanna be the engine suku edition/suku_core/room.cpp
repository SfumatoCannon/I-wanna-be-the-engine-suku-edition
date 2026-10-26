#include "room.h"
#include "object.h"
#include "../suku_draw/effect.h"
#include "../suku_draw/draw_core.h"
#include "../suku_gamewindow/game_window.h"
#include "object_collision_pool.h"
#include <suku_foundation/maths.h>
#include <suku_objects/map_loader.h>
#include <suku_draw/shape.h>
#include <suku_foundation/suku_save.h>
#include <suku_foundation/input.h>
#include <algorithm>
#include "room_pool.h"
#include "tile.h"

namespace suku
{
	Room::Room() : Room(constants::window::widthLogical, constants::window::heightLogical) {}

	Room::Room(unsigned int _width, unsigned int _height)
		: id_(typecode(Room)),
		width_(_width), height_(_height),
		camera(constants::window::widthLogical, constants::window::heightLogical),
		collisionPool_(std::make_unique<ObjectCollisionPool>())
	{
		displayLayer.newLayer(constants::window::widthLogical, constants::window::heightLogical);
		camera.setBorder(0, 0, _width, _height);
	}

	void Room::loadFromJtoolData(std::string_view _data)
	{
		MapLoader::loadFromJtoolData(this, _data);
	}

	bool Room::isSavable()
	{
		return RoomTypecodeManager::isExist(id_);
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
		paintArray_[_object->getTargetPaintLayer()][_object->paintId()].remove_if([_object](std::shared_ptr<Object>& objPtr)
			{ return objPtr.get() == _object; });
	}

	void Room::destroyImmediately(Object* _object)
	{
		collisionPool_->removeObject(_object);
		objectPointerArray_[_object->getTypecode()].remove_if([_object](std::shared_ptr<Object>& objPtr)
			{ return objPtr.get() == _object; });
		preUpdateArray_[_object->preUpdateId()].remove_if([_object](std::shared_ptr<Object>& objPtr)
			{ return objPtr.get() == _object; });
		updateArray_[_object->updateId()].remove_if([_object](std::shared_ptr<Object>& objPtr)
			{ return objPtr.get() == _object; });
		postUpdateArray_[_object->postUpdateId()].remove_if([_object](std::shared_ptr<Object>& objPtr)
			{ return objPtr.get() == _object; });
		paintArray_[_object->getTargetPaintLayer()][_object->paintId()].remove_if([_object](std::shared_ptr<Object>& objPtr)
			{ return objPtr.get() == _object; });
	}

	std::shared_ptr<Object> Room::findObjectSharedPtr(Object* _object)
	{
		if (_object == nullptr)
			return nullptr;

		auto iter = objectPointerArray_.find(_object->getTypecode());
		if (iter == objectPointerArray_.end())
			return nullptr;

		auto objectList = iter->second;
		for (auto& o : objectList)
		{
			if (o.get() == _object)
				return o;
		}
		return nullptr;
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

		if (auto objectPtr = findObjectSharedPtr(_object))
			targetArray.push_back(std::move(objectPtr));
		_object->preUpdateId_ = _newId;
	}

	void Room::setObjectUpdatePriority(Object* _object, double _newId)
	{
		auto& originalArray = updateArray_[_object->updateId_];
		auto& targetArray = updateArray_[_newId];
		bool found = false;
		for (auto iter = originalArray.begin(); iter != originalArray.end(); iter++)
		{
			if ((*iter).get() == _object)
			{
				targetArray.push_back(std::move(*iter));
				originalArray.erase(iter);
				found = true;
				break;
			}
		}
		if (!found)
		{
			if (auto objectPtr = findObjectSharedPtr(_object))
				targetArray.push_back(std::move(objectPtr));
		}
		_object->updateId_ = _newId;
	}

	void Room::setObjectRecheckPriority(Object* _object, double _newId)
	{
		auto& originalArray = postUpdateArray_[_object->postUpdateId_];
		auto& targetArray = postUpdateArray_[_newId];
		bool found = false;
		for (auto iter = originalArray.begin(); iter != originalArray.end(); iter++)
		{
			if ((*iter).get() == _object)
			{
				targetArray.push_back(std::move(*iter));
				originalArray.erase(iter);
				found = true;
				break;
			}
		}
		if (!found)
		{
			if (auto objectPtr = findObjectSharedPtr(_object))
				targetArray.push_back(std::move(objectPtr));
		}
		_object->postUpdateId_ = _newId;
	}

	void Room::setObjectPaintPriority(Object* _object, double _newId)
	{
		auto& originalArray = paintArray_[_object->getTargetPaintLayer()][_object->paintId_];
		auto& targetArray = paintArray_[_object->getTargetPaintLayer()][_newId];
		bool found = false;
		for (auto iter = originalArray.begin(); iter != originalArray.end(); iter++)
		{
			if ((*iter).get() == _object)
			{
				targetArray.push_back(std::move(*iter));
				originalArray.erase(iter);
				found = true;
				break;
			}
		}
		if (!found)
		{
			if (auto objectPtr = findObjectSharedPtr(_object))
				targetArray.push_back(std::move(objectPtr));
		}
		_object->paintId_ = _newId;
	}

	void Room::setObjectPaintLayer(Object* _object, PaintLayer* _paintLayer)
	{
		if (_paintLayer == _object->getTargetPaintLayer())
			return;
		auto& originalArray = paintArray_[_object->getTargetPaintLayer()][_object->paintId_];
		auto& targetArray = paintArray_[_paintLayer][_object->paintId_];
		bool found = false;
		for (auto iter = originalArray.begin(); iter != originalArray.end(); iter++)
		{
			if ((*iter).get() == _object)
			{
				targetArray.push_back(std::move(*iter));
				originalArray.erase(iter);
				found = true;
				break;
			}
		}
		if (!found)
		{
			if (auto objectPtr = findObjectSharedPtr(_object))
				targetArray.push_back(std::move(objectPtr));
		}
		_object->targetPaintLayer_ = _paintLayer;
	}

	void Room::enter()
	{
		std::list<Object*> allObjList = getObjectList<Object>();
		onStart();
		onEnter();
		for (auto& objPointer : allObjList)
		{
			objPointer->onRoomStart();
			objPointer->onRoomEnter();
		}
	}

	void Room::restart()
	{
		std::list<Object*> allObjList = getObjectList<Object>();
		onStart();
		onRestart();
		for (auto& objPointer : allObjList)
		{
			objPointer->onRoomStart();
			objPointer->onRoomRestart();
		}
	}

	void Room::onEnter()
	{
		SaveFile* saveFile = SaveFile::getGlobalSaveFile();
		if (saveFile != nullptr)
		{
			clock_ = saveFile->get<unsigned int>("time");
		}
	}

	void Room::onSave()
	{
		for (auto& o : getObjectList<Object>())
			o->onSave();
	}

	void Room::update()
	{
		if (isUpdatePaused_)
		{
			onUpdateStart();
			Object* obj = updateControllerObject_;
			obj->hspeedTemp = obj->vspeedTemp = 0;
			obj->updateFunction();
			obj->extraTransformLastFrame_ = obj->extraTransform_;
			obj->onUpdateStart();
			obj->onUpdate();
			obj->x += obj->totalHspeed();
			obj->y += obj->totalVspeed();
			obj->onUpdateEnd();
			obj->x.addTick();
			obj->y.addTick();
			obj->xScale.addTick();
			obj->yScale.addTick();
			obj->angle.addTick();
			obj->opacity.addTick();
			return;
		}

		clock_++;
		if (static_cast<long>(clock_) % 50 == 0)
		{
			SaveFile* saveFile = SaveFile::getGlobalSaveFile();
			if (saveFile != nullptr)
			{
				saveFile->saveVar<unsigned int>("time", static_cast<unsigned int>(clock_));
			}
		}

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
					if (!obj->isFrozen())
					{
						obj->updateFunction();
					}
					iter++;
				}
			}
		}

		for (auto& [id, objArray] : preUpdateArray_)
		{
			for (auto iter = objArray.begin(); iter != objArray.end();)
			{
				Object* obj = (*iter).get();
				if (obj->removeTag_)
				{
					iter = objArray.erase(iter);
					continue;
				}
				obj->extraTransformLastFrame_ = obj->extraTransform_;
				if (!obj->isFrozen())
				{
					obj->onUpdateStart();
				}
				iter++;
			}
		}

		ActionManager::execute(this);
		//for (auto& [id, objArray] : updateArray_)
		//{
		//	for (auto iter = objArray.begin(); iter != objArray.end();)
		//	{
		//		Object* obj = (*iter).get();
		//		if (obj->removeTag_)
		//		{
		//			iter = objArray.erase(iter);
		//			continue;
		//		}
		//		if (!obj->isFrozen())
		//		{
		//			obj->onUpdate();
		//		}
		//		iter++;
		//	}
		//}

		for (auto& [id, objArray] : objectPointerArray_)
		{
			for (auto& obj : objArray)
			{
				if (!obj->isFrozen())
				{
					obj->x += obj->totalHspeed();
					obj->y += obj->totalVspeed();
				}
			}
		}

		for (auto& [id, objArray] : postUpdateArray_)
		{
			for (auto iter = objArray.begin(); iter != objArray.end();)
			{
				Object* obj = (*iter).get();
				if (obj->removeTag_)
				{
					iter = objArray.erase(iter);
					continue;
				}
				if (!obj->isFrozen())
				{
					//obj->onUpdateEnd();
					obj->x.addTick();
					obj->y.addTick();
					obj->xScale.addTick();
					obj->yScale.addTick();
					obj->angle.addTick();
					obj->opacity.addTick();
				}
				iter++;
			}
		}

		onUpdateEnd();

		camera.update();
		camera.x.addTick();
		camera.y.addTick();
		camera.xScale.addTick();
		camera.yScale.addTick();
		camera.angle.addTick();
	}

	RenderBitmap Room::paintOnLayer(PaintLayer& _layer)
	{
		auto& targetPaintArray = paintArray_[&_layer];
		_layer.beginDraw();
		onPaintStart(_layer);
		auto work = [&]() {
			for (auto& [id, objArray] : targetPaintArray)
			{
				for (auto iter = objArray.begin(); iter != objArray.end();)
				{
					Object* obj = (*iter).get();
					if (obj->removeTag_)
					{
						iter = objArray.erase(iter);
						continue;
					}

					if (!obj->isVisible)
					{
						iter++;
						continue;
					}

					obj->onPaint();

					iter++;
				}
			}
			};
		work();
		if (&_layer == &displayLayer)
		{
			targetPaintArray = paintArray_[nullptr];
			work();
		}
		onPaintEnd(_layer);
		return _layer.endDraw();
	}

	RenderBitmap Room::paintOnLayer(PaintLayer& _layer, float _frameOffsetRate)
	{
		auto& targetPaintArray = paintArray_[&_layer];
		_layer.beginDraw();
		onPaintStart(_layer);
		auto work = [&]() {

			for (auto& [id, objArray] : targetPaintArray)
			{
				for (auto iter = objArray.begin(); iter != objArray.end();)
				{
					Object* obj = (*iter).get();
					if (obj->removeTag_)
					{
						iter = objArray.erase(iter);
						continue;
					}

					if (!obj->isVisible)
					{
						iter++;
						continue;
					}

					Property<real> oX, oY;
					Property<real> oScaleX, oScaleY;
					Property<real> oAngle;
					Transform oExtraTransform;

					oX = obj->x.getInterpolatedFrameState(_frameOffsetRate);
					oY = obj->y.getInterpolatedFrameState(_frameOffsetRate);
					oScaleX = obj->xScale.getInterpolatedFrameState(_frameOffsetRate);
					oScaleY = obj->yScale.getInterpolatedFrameState(_frameOffsetRate);
					oAngle = obj->angle.getInterpolatedFrameState(_frameOffsetRate);
					oExtraTransform = linearInterpolate(obj->extraTransformLastFrame_, obj->extraTransform_, _frameOffsetRate);

					std::swap(obj->x, oX);
					std::swap(obj->y, oY);
					std::swap(obj->xScale, oScaleX);
					std::swap(obj->yScale, oScaleY);
					std::swap(obj->angle, oAngle);
					std::swap(obj->extraTransform_, oExtraTransform);

					obj->onPaint();

					std::swap(obj->x, oX);
					std::swap(obj->y, oY);
					std::swap(obj->xScale, oScaleX);
					std::swap(obj->yScale, oScaleY);
					std::swap(obj->angle, oAngle);
					std::swap(obj->extraTransform_, oExtraTransform);

					iter++;
				}
			}
			};
		work();
		if (&_layer == &displayLayer)
		{
			targetPaintArray = paintArray_[nullptr];
			work();
		}
		onPaintEnd(_layer);
		return _layer.endDraw();
	}

	void Room::paint()
	{
		onPaintStart();

		displayLayer.setBasicTransform(camera.getTransformToLayer());

		auto pic = paintOnLayer(displayLayer);
		static EffectTransform scaleEffect(ScaleMode::HighQualityCubic, false);
		scaleEffect.setTransform(GameWindow::getPixelMappingTransform());
		scaleEffect.setInput(pic);
		scaleEffect.paint();

		onPaintEnd();
	}

	void Room::additionalFramePaint(float _offsetRate)
	{
		if (isUpdatePaused_)
			_offsetRate = 0.0f;

		onPaintStart();

		float cameraX = camera.x.getInterpolatedFrameState(_offsetRate);
		float cameraY = camera.y.getInterpolatedFrameState(_offsetRate);
		float cameraAngle = camera.angle.getInterpolatedFrameState(_offsetRate);

		displayLayer.setBasicTransform(camera.getTransformToLayer(_offsetRate));

		auto pic = paintOnLayer(displayLayer, _offsetRate);
		static EffectTransform scaleEffect(ScaleMode::HighQualityCubic, false);
		scaleEffect.setTransform(GameWindow::getPixelMappingTransform());
		scaleEffect.setInput(pic);
		scaleEffect.paint();

		onPaintEnd();
	}

	void Room::pause(Object* _controllerObject)
	{
		isUpdatePaused_ = true;
		updateControllerObject_ = _controllerObject;
	}

	void Room::resume()
	{
		isUpdatePaused_ = false;
	}

	void Room::setBGM(Sound* _bgm)
	{
		hasBGM_ = true;
		bgm_ = _bgm;
	}

	void Room::setBackground(Bitmap _background)
	{
		if (!_background.isValid())
			return;

		hasBackground_ = true;
		backgroundBrush_ = std::make_unique<Brush>(_background);
		backgroundShape_ = std::make_unique<RectangleShape>(width_, height_);
	}

	void Room::setBackground(Color _color)
	{
		hasBackground_ = true;
		backgroundBrush_ = std::make_unique<Brush>(_color);
		backgroundShape_ = std::make_unique<RectangleShape>(width_, height_);
	}

	void Room::setTilePack(TilePack& _tilepack)
	{
		TilePack::resetAll();
		_tilepack.use();
	}

	void Room::addTilePack(TilePack& _tilepack)
	{
		_tilepack.use();
	}

	void Room::removeTilePack(TilePack& _tilepack)
	{
		_tilepack.unuse();
	}

	void Room::paintBackground()
	{
		if (hasBackground_ && backgroundBrush_ && backgroundShape_)
		{
			backgroundBrush_->setTransform(translation(backgroundOffsetX, backgroundOffsetY));
			backgroundShape_->paint(0, 0, *backgroundBrush_.get(), Brush::emptyBrush());
		}
	}
}