#pragma once
#include "../framework.h"
#include "../suku_draw/transform.h"
#include "../suku_foundation/var.h"
#include "../suku_foundation/type_tree.h"
#include "../suku_foundation/maths.h"
#include <functional>
#include <list>

namespace suku
{
	class Sprite;
	class SpriteElement;
	class Room;
	class Object;

	template<typename T>
	concept suku_object = std::is_base_of_v<Object, T>;

	class Object
	{

	public:
		float x, y;
		float spawnX, spawnY;
		float xScale, yScale;
		float vspeed, hspeed;
		float vspeedTemp, hspeedTemp;
		Room* inRoom()const { return inRoom_; }
		double preUpdateId()const { return preUpdateId_; }
		double updateId()const { return updateId_; }
		double postUpdateId()const { return postUpdateId_; }
		double paintId()const { return paintId_; }
		size_t kindId()const { return kindId_; }

		void setInRoom(Room* _room);

		void setPreUpdateId(double _id);
		void setUpdateId(double _id);
		void setPostUpdateId(double _id);
		void setPaintId(double _id);

		Transform spriteTransform;
		float getWidth();
		float getHeight();
		Vector getSize();
		float getCenterX();
		float getCenterY();
		Vector getCenterPosition();
		double getSpriteAngle();
		double getSpriteXScale();
		double getSpriteYScale();

		float alpha;
		std::map<std::string, Var> var;

		Var& operator[](std::string _str) { return var[_str]; }
		Object(float _x = 0, float _y = 0);
		
		Sprite* getSprite()const { return sprite_; }
		SpriteElement* getSpriteFrame()const;
		UINT getSpriteFrameIndex()const;

		void remove();
		void destroy();

		void updateFunction();

		static void classInitialize() {}
		virtual void onRoomEntering() {}
		virtual void onRestarting();
		virtual void onRemove() {}
		virtual bool onPaint() { return false; } //if return true, then terminate the sprite paint
		virtual void preUpdate() {}
		virtual void update() {}
		virtual void postUpdate() {}

		float totalHspeed()const;
		float totalVspeed()const;

		void setSpeed(float _hspeed, float _vspeed, float _hspeedTemp = 0.0f, float _vspeedTemp = 0.0f);
		void setSpeedTemp(float _hspeedTemp, float _vspeedTemp);

		void movingTo(float _xTo, float _yTo, int _time);
		void rotate(float _angle);
		void rotate(float _angle, double _rotatingCenterX, double _rotatingCenterY, bool _isRotatingItself = true);
		void rotateTo(float _angle, int _time);
		void rotateTo(float _angle, double _rotatingCenterX, double _rotatingCenterY, int _time, bool _isRotatingItself = true);
		void addAction(std::function<bool(Object*)> _actionFunc);
		void addDelayAction(int _time, std::function<bool(Object*)> _actionFunc);
		void addTimelineAction(std::vector<std::pair<int, std::function<bool(Object*)>>> _actionVec);
		void addTimelineAction(std::vector<int> _timeVec, std::vector<std::function<bool(Object*)>> _actionVec);
		void addTimelineAction(std::function<bool(Object*, int)> _actionFuncWithTime);
		void clearActions();

		void paintBody()const;
		void paintBody(bool _isSmoothMode)const;
		void paintBody(float _x, float _y, bool _isSmoothMode = false)const;
		void paintBody(Transform _spriteTransform)const;
		void paintBody(float _x, float _y, Transform _spriteTransform)const;
		void paintBody(float _alpha, bool _isSmoothMode = false)const;
		void paintBody(float _x, float _y, float _alpha, bool _isSmoothMode = false)const;
		void paintBody(Transform _spriteTransform, float _alpha)const;
		void paintBody(float _x, float _y, Transform _spriteTransform, float _alpha)const;

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
	protected:
		friend class Room;

		Room* inRoom_;
		size_t kindId_;
		double preUpdateId_, updateId_, postUpdateId_, paintId_;
		bool removeTag_;
		bool destroyTag_;
		Sprite* sprite_;
		int clock_;
		std::list<std::function<bool()> > actionList_;
	};
}

#include "object.inl"