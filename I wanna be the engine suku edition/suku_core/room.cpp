#include "room.h"
#include "object.h"
#include "../suku_draw/effect.h"
#include "../suku_draw/draw_core.h"
#include "../suku_gamewindow/game_window.h"
#include "object_collision_pool.h"
#include <suku_foundation/maths.h>
#include <suku_objects/map_loader.h>

namespace suku
{
	Room::Room() : Room(constants::window::widthLogical, constants::window::heightLogical) {}

	Room::Room(unsigned int _width, unsigned int _height)
		: width_(_width), height_(_height),
		camera(constants::window::widthLogical, constants::window::heightLogical),
		collisionPool_(std::make_unique<ObjectCollisionPool>())
	{
		static unsigned int roomIdCounter = 0;
		roomIdCounter++;
		roomId_ = roomIdCounter;
		displayLayer.newLayer(constants::window::widthLogical, constants::window::heightLogical);
		camera.setBorder(0, 0, _height, _width);
	}

	void Room::loadFromJtoolData(std::string_view _data)
	{
		MapLoader::loadFromJtoolData(this, _data);
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
		auto& originalArray = paintArray_[_object->getTargetPaintLayer()][_object->paintId_];
		auto& targetArray = paintArray_[_object->getTargetPaintLayer()][_newId];
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

	void Room::setObjectPaintLayer(Object* _object, PaintLayer* _paintLayer)
	{
		if (_paintLayer == _object->getTargetPaintLayer())
			return;
		auto& originalArray = paintArray_[_object->getTargetPaintLayer()][_object->paintId_];
		auto& targetArray = paintArray_[_paintLayer][_object->paintId_];
		for (auto iter = originalArray.begin(); iter != originalArray.end(); iter++)
		{
			if ((*iter).get() == _object)
			{
				targetArray.push_back(std::move(*iter));
				originalArray.erase(iter);
				_object->targetPaintLayer_ = _paintLayer;
				break;
			}
		}
		// not found in original array
		targetArray.push_back(std::shared_ptr<Object>(_object));
		_object->targetPaintLayer_ = _paintLayer;
	}

	void Room::enter()
	{
		std::list<Object*> allObjList = getObjectList<Object>();
		for (auto& objPointer : allObjList)
		{
			objPointer->onRoomStart();
			objPointer->onRoomEnter();
		}
		onStart();
		onEnter();
	}

	void Room::restart()
	{
		std::list<Object*> allObjList = getObjectList<Object>();
		for (auto& objPointer : allObjList)
		{
			objPointer->onRoomStart();
			objPointer->onRoomRestart();
		}
		onStart();
		onRestart();
	}

	void Room::update()
	{
		if (isUpdatePaused_)
		{
			onUpdateStart();
			Object* obj = updateControllerObject_;
			obj->isPositionTransitionalFrame_ = true;
			obj->isSpriteTransformTransitionalFrame_ = true;
			obj->hspeedTemp = obj->vspeedTemp = 0;
			obj->updateFunction();
			obj->spriteTransformLastFrame_ = obj->transform;
			obj->onPreUpdate();
			obj->onUpdate();
			obj->xLastFrame_ = obj->x;
			obj->yLastFrame_ = obj->y;
			obj->x += obj->totalHspeed();
			obj->y += obj->totalVspeed();
			obj->onPostUpdate();
			obj->x.addTick();
			obj->y.addTick();
			return;
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
					obj->isPositionTransitionalFrame_ = true;
					obj->isSpriteTransformTransitionalFrame_ = true;
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
				obj->spriteTransformLastFrame_ = obj->transform;
				if (!obj->isFrozen())
				{
					obj->onPreUpdate();
				}
				iter++;
			}
		}

		for (auto& [id, objArray] : updateArray_)
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
					obj->onUpdate();
				}
				iter++;
			}
		}

		for (auto& [id, objArray] : objectPointerArray_)
		{
			for (auto& obj : objArray)
			{
				if (!obj->isFrozen())
				{
					obj->xLastFrame_ = obj->x;
					obj->yLastFrame_ = obj->y;
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
					obj->onPostUpdate();
					obj->x.addTick();
					obj->y.addTick();
				}
				iter++;
			}
		}

		onUpdateEnd();

		camera.update();
		camera.x.addTick();
		camera.y.addTick();
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

					if (!obj->onPaint())
						obj->paintBody();

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

					if (obj->onPaint())
					{
						iter++;
						continue;
					}

					float posX, posY;
					Transform transform;
					if (obj->isPositionTransitionalFrame_)
					{
						posX = obj->x.getInterpolatedFrameState(_frameOffsetRate);
						posY = obj->y.getInterpolatedFrameState(_frameOffsetRate);
					}
					else
					{
						posX = obj->x.getFrameState();
						posY = obj->y.getFrameState();
					}
					if (obj->isSpriteTransformTransitionalFrame_)
					{
						Transform objSpriteTransformLastFrame = obj->spriteTransformLastFrame_;
						transform = linearInterpolate(objSpriteTransformLastFrame, obj->transform, _frameOffsetRate);
					}
					else
					{
						transform = obj->transform;
					}

					obj->paintBody(posX, posY, transform);

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

		displayLayer.setBasicTransform(
			translation(-camera.x, -camera.y)
			+ rotation(camera.getCenterX(), camera.getCenterY(), -camera.angle)
		);

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

		displayLayer.setBasicTransform(
			translation(-cameraX, -cameraY)
			+ rotation(camera.getCenterX(), camera.getCenterY(), -cameraAngle)
		);
		
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
		background_ = _background;
	}

	void Room::paintBackground()
	{
		if (hasBackground_ && background_.isValid())
		{
			auto [backgroundWidth, backgroundHeight] = background_.getSize();
			double backgroundStartX = maths::modR((double)backgroundOffsetX, backgroundWidth);
			double backgroundStartY = maths::modR((double)backgroundOffsetY, backgroundHeight);
			if (backgroundStartX > 0)
				backgroundStartX -= backgroundWidth;
			if (backgroundStartY > 0)
				backgroundStartY -= backgroundHeight;
			for (double x = backgroundStartX; x < width_; x += backgroundWidth)
			{
				for (double y = backgroundStartY; y < height_; y += backgroundHeight)
				{
					background_.paint(x, y);
				}
			}
		}
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
}