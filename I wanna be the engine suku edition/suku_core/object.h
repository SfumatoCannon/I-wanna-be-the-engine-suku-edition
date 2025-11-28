#pragma once
#include "../framework.h"
#include "../suku_draw/includes.h"
#include "../suku_foundation/includes.h"

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
		bool isClearPainting;
		Room* inRoom()const { return inRoom_; }
		double reviseStateId()const { return reviseStateId_; }
		double updateStateId()const { return updateStateId_; }
		double recheckStateId()const { return recheckStateId_; }
		double paintId()const { return paintId_; }
		size_t kind()const { return kindId_; }

		void setReviseStateId(double _id);
		void setUpdateStateId(double _id);
		void setRecheckStateId(double _id);
		void setPaintId(double _id);

		Transform spriteTransform;
		float getCenterX();
		float getCenterY();
		Vector getCenterPosition();
		double getSpriteAngle();
		double getSpriteXScale();
		double getSpriteYScale();

		float alpha;
		std::map<std::string, Var> var;

		Var& operator[](std::string _str);
		Object(float _x = 0, float _y = 0);
		//Object(Object&& _others);
		SpriteElement* nowState()const;

		void remove();
		void destroy();

		void updateFunction();

		static void classInitialize() {}
		virtual void onAppearing() {}
		virtual void onRestarting();
		virtual void onDestroy() {}
		virtual bool onPaint() { return false; } //if return true, then terminate the sprite paint
		virtual void reviseState() {}
		virtual void updateState() {}
		virtual void recheckState() {}

		float totalHspeed()const;
		float totalVspeed()const;

		void setSpeed(float _hspeed, float _vspeed, float _hspeedTemp = 0.0f, float _vspeedTemp = 0.0f);
		void setSpeedTemp(float _hspeedTemp, float _vspeedTemp);

		void movingTo(float _xTo, float _yTo, int _time);
		void rotate(float _angle);
		void rotate(float _angle, double _rotatingCenterX, double _rotatingCenterY, bool _isRotatingItself = true);
		void rotateTo(float _angle, int _time);
		void rotateTo(float _angle, double _rotatingCenterX, double _rotatingCenterY, int _time, bool _isRotatingItself = true);
		void setAction(std::function<bool(Object*)> _function);
		void setDelayAction(int _time, std::function<void(Object*)> _function);

		void paintBody();
		void paintBody(float _x, float _y);

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
		double reviseStateId_, updateStateId_, recheckStateId_, paintId_;
		std::list<Object*>::iterator objectIterator_;
		std::list<Object*>::iterator reviseStateIterator_;
		std::list<Object*>::iterator updateStateIterator_;
		std::list<Object*>::iterator recheckStateIterator_;
		std::list<Object*>::iterator paintIterator_;
		bool isUpdating_;
		bool removeTag_;
		bool destroyTag_;
		Sprite* sprite_;
		int clock_;
		std::list<std::function<bool()> > functionList_;
	};
}

#include "object.inl"