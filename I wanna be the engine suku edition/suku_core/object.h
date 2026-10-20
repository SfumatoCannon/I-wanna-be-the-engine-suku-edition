#pragma once
#include "../framework.h"
#include "../suku_draw/transform.h"
#include "../suku_foundation/var.h"
#include "../suku_foundation/suku_type.h"
#include "../suku_foundation/maths.h"
#include "suku_core_concepts.h"
#include "suku_property.h"
#include <functional>
#include <list>

namespace suku
{
	template<suku_property_type T> class Property;
	class Sprite;
	class SpriteElement;
	class Room;
	class Object;
	class PaintLayer;

	class Object
	{

	public:
		Property<real> x, y;
		real spawnX, spawnY;
		Property<real> vspeed = 0, hspeed = 0;
		real vspeedTemp = 0, hspeedTemp = 0;
		Property<real> xScale = 1, yScale = 1;
		Property<real> angle = 0;
		real centerOffsetX = 0, centerOffsetY = 0;

		unsigned int spriteBasicIndex = 0;
		double spriteAnimationRate = 1.0;

		bool isInRoom()const { return inRoom_ != nullptr && isInRoom_; }
		Room* inRoom()const { return inRoom_; }
		double preUpdateId()const { return preUpdateId_; }
		double updateId()const { return updateId_; }
		double postUpdateId()const { return postUpdateId_; }
		double paintId()const { return paintId_; }

		virtual const Typecode getTypecode() const { return typecode(*this); }

		void setPreUpdateId(double _id);
		void setUpdateId(double _id);
		void setPostUpdateId(double _id);
		void setPaintId(double _id);
		void setTargetPaintLayer(PaintLayer& _layer);
		PaintLayer* getTargetPaintLayer() { return targetPaintLayer_; }

		double getWidth();
		double getHeight();
		Vector getSize();

		real getScaleX() { return xScale; }
		auto getScaleX(this const auto& _self, real& _scaleX_out) { _scaleX_out = _self.xScale; return &_self; }
		real getScaleY() { return yScale; }
		auto getScaleY(this const auto& _self, real& _scaleY_out) { _scaleY_out = _self.yScale; return &_self; }
		Vector getScale() { return { xScale, yScale }; }
		auto getScale(this const auto& _self, Vector& _scale_out) { _scale_out = { _self.xScale, _self.yScale }; return &_self; }

		auto setScaleX(this auto& _self, real _scaleX) { _self.xScale = _scaleX; return &_self; }
		auto setScaleX(this auto& _self, std::pair<real, const Transition&> _pair) { _self.xScale = _pair; return &_self; }
		auto setScaleY(this auto& _self, real _scaleY) { _self.yScale = _scaleY; return &_self; }
		auto setScaleY(this auto& _self, std::pair<real, const Transition&> _pair) { _self.yScale = _pair; return &_self; }
		auto setScale(this auto& _self, real _scaleX, real _scaleY) { _self.xScale = _scaleX; _self.yScale = _scaleY; return &_self; }
		auto setScale(this auto& _self, std::pair<real, const Transition&> _pairX, std::pair<real, const Transition&> _pairY) { _self.xScale = _pairX; _self.yScale = _pairY; return &_self; }

		auto setCenterOffsetX(this auto& _self, real _offsetX) { _self.centerOffsetX = _offsetX; return &_self; }
		auto setCenterOffsetY(this auto& _self, real _offsetY) { _self.centerOffsetY = _offsetY; return &_self; }
		auto setCenterOffset(this auto& _self, real _offsetX, real _offsetY) { _self.centerOffsetX = _offsetX; _self.centerOffsetY = _offsetY; return &_self; }

		Transform getTransform()const;

		auto setPosition(this auto& _self, real _x, real _y) { _self.x = _x; _self.y = _y; return &_self; }

		bool isVisible = true;
		Property<float> opacity = 1;

		Var& operator[](const std::string& _str) { return var_[_str]; }

		Object(real _x = 0, real _y = 0, Room* _inRoom = nullptr);

		Sprite* getSprite()const { return sprite_; }
		SpriteElement* getSpriteFrame()const;
		UINT getSpriteIndex()const;
		void setSpriteIndex(UINT _index) { spriteBasicIndex = _index; }
		double getSpriteAnimationRate()const { return spriteAnimationRate; }
		double getSpriteFlipTime()const;
		void setSpriteAnimationRate(double _rate) { spriteAnimationRate = _rate; }
		void setSpriteFlipTime(double _speed);
		long long getClockTick()const { return clock_; }

		void remove();
		void destroy();

		void updateFunction();

		static void classInitialize() {}
		virtual void onRoomEnter() {}
		virtual void onRoomStart() {}
		virtual void onRoomRestart();
		virtual void onRemove() {}
		virtual void onSave() {}
		virtual void onPaint() { paintBody(); }
		virtual void onUpdateStart() {}
		virtual void onUpdate() {}
		virtual void onUpdateEnd() {}

		float totalHspeed()const;
		float totalVspeed()const;

		void setSpeed(float _hspeed, float _vspeed, float _hspeedTemp = 0.0f, float _vspeedTemp = 0.0f);
		void setSpeedTemp(float _hspeedTemp, float _vspeedTemp);

		void movingTo(float _xTo, float _yTo, int _time);
		
		void addAction(std::function<bool(Object*)> _actionFunc);
		void addDelayAction(int _time, std::function<bool(Object*)> _actionFunc);
		void addTimelineAction(std::vector<std::pair<int, std::function<bool(Object*)>>> _actionVec);
		void addTimelineAction(std::vector<int> _timeVec, std::vector<std::function<bool(Object*)>> _actionVec);
		void addTimelineAction(std::function<bool(Object*, int)> _actionFuncWithTime);
		void clearActions();

		void paintBody()const;
		void paintBody(float _alpha)const;
		void paintBody(float _x, float _y)const;
		void paintBody(float _x, float _y, float _alpha)const;
		void paintBody(float _x, float _y, Transform _spriteTransform)const;
		void paintBody(float _x, float _y, Transform _spriteTransform, float _alpha)const;

		void paintBodySmooth()const;
		void paintBodySmooth(float _alpha)const;
		void paintBodySmooth(float _x, float _y)const;
		void paintBodySmooth(float _x, float _y, float _alpha)const;
		void paintBodySmooth(float _x, float _y, Transform _spriteTransform)const;
		void paintBodySmooth(float _x, float _y, Transform _spriteTransform, float _alpha)const;

		void paintBody(Transform _spriteTransform)const;
		void paintBody(Transform _spriteTransform, float _alpha)const;

		template<suku_object Obj> bool isCrashed(const Obj& _obj)const;
		template<suku_object Obj> bool isCrashed(const Obj& _obj, float _x, float _y)const;
		template<suku_object Obj> bool isCrashed(const Obj& _obj, float _x, float _y, float _objX, float _objY)const;
		template<suku_object Obj> bool isCrashed(const Obj& _obj, Vector _position)const;
		template<suku_object Obj> bool isCrashed(const Obj& _obj, Vector _position, Vector _objPosition)const;
		template<suku_object Obj> bool isCrashed(const std::list<Obj*>& _objList)const;
		template<suku_object Obj> bool isCrashed(const std::list<Obj*>& _objList, float _x, float _y)const;
		void moveContactOld(const Object& _obj, bool _isPredict = false);
		void moveContactOld(const std::list<Object*>& _objList, bool _isPredict = false);
		void moveContactNew(const Object& _obj, bool _isPredict = false);
		void moveContactNew(const std::list<Object*>& _objList, bool _isPredict = false);
		Vector getContactSpeed(const Object& _obj, float _hspeed, float _vspeed);
		Vector getContactSpeed(const Object& _obj, float _hspeed, float _vspeed, float _x, float _y);
		Vector getContactSpeed(const std::list<Object*>& _objList, float _hspeed, float _vspeed);
		Vector getContactSpeed(const std::list<Object*>& _objList, float _hspeed, float _vspeed, float _x, float _y);
		void contactToUp(Object _obj, bool _isPredict = false, bool _isMoveDirectly = false);
		void contactToDown(Object _obj, bool _isPredict = false, bool _isMoveDirectly = false);
		void contactToLeft(Object _obj, bool _isPredict = false, bool _isMoveDirectly = false);
		void contactToRight(Object _obj, bool _isPredict = false, bool _isMoveDirectly = false);
		void moveContact(Object _obj, bool _isPredict = false, bool _isMoveDirectly = false);

		template<suku_object Obj> Obj* selectObject(std::function<bool(Obj*)> _function);
		template<suku_object Obj> std::list<Obj*> selectObjectList(std::function<bool(Obj*)> _function);
		template<suku_object Obj> Obj* getCrashedObject(bool _isPredict = false);
		template<suku_object Obj> std::list<Obj*> getCrashedObjectList(bool _isPredict = false);
		template<suku_object Obj> Obj* getCrashedObject(float _x, float _y, bool _isPredict = false);
		template<suku_object Obj> std::list<Obj*> getCrashedObjectList(float _x, float _y, bool _isPredict = false);

		void save();
		void spawn();

		bool isFrozen()const { return isFrozen_; }
		void setFrozen(bool _isFrozen) { isFrozen_ = _isFrozen; }
		void setOpacity(float _opacity) { opacity = _opacity; }

	protected:
		template<suku_property_type T> friend class Property;
		friend class Room;

		Transform extraTransform_;

		std::map<std::string, Var> var_;

		long long clock_ = 0;
		Room* inRoom_ = nullptr;
		double	preUpdateId_ = 0.0,
			updateId_ = 0.0,
			postUpdateId_ = 0.0,
			paintId_ = 0.0;
		bool isInRoom_ = false;
		bool isFrozen_ = false;

		Sprite* sprite_ = nullptr;
		std::list<std::function<bool()> > actionList_;
	private:
		bool removeTag_ = false;
		bool destroyTag_ = false;
		PaintLayer* targetPaintLayer_ = nullptr;
		Transform extraTransformLastFrame_ = Transform();
	};
}

#include "object.inl"