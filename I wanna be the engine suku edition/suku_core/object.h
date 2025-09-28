#pragma once
#include "../framework.h"
#include "../suku_draw/includes.h"
#include "../game_saving.h"
#include "sprite.h"

namespace suku
{
	extern TypeTree collisionInheritTree;
	class Room;

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

		template<typename Obj> Obj* getInsideObject();
		void setReviseStateId(double _id);
		void setUpdateStateId(double _id);
		void setRecheckStateId(double _id);
		void setPaintId(double _id);

		Transform spriteTransform;
		float centerX();
		float centerY();
		Vector center();
		double getSpriteAngle();
		double getSpriteXScale();
		double getSpriteYScale();

		float alpha;
		std::map<std::string, Var> var;

		Var& operator[](std::string _str);
		Object(float _x = 0, float _y = 0);
		//Object(Object&& _others);
		SpriteZero* nowState()const;

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

		bool isCrashed(const Object& _obj)const;
		bool isCrashed(const Object& _obj, float _x, float _y)const;
		bool isCrashed(const Object& _obj, float _x, float _y, float _objX, float _objY)const;
		bool isCrashed(const std::list<Object*>& _objList)const;
		bool isCrashed(const std::list<Object*>& _objList, float _x, float _y)const;
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
		//void contactToObject();
		void moveContact(Object _obj, bool _isPredict = false, bool _isMoveDirectly = false);

		template<typename Obj> Obj* getCrashedObjectPrecisely(bool _isPredict = false);
		template<typename Obj> Object* getCrashedObject(bool _isPredict = false);
		template<typename Obj> std::list<Obj*> getCrashedObjectListPrecisely(bool _isPredict = false);
		template<typename Obj> std::list<Object*> getCrashedObjectList(bool _isPredict = false);
		template<typename Obj> Obj* getCrashedObjectPrecisely(float _x, float _y, bool _isPredict = false);
		template<typename Obj> Object* getCrashedObject(float _x, float _y, bool _isPredict = false);
		template<typename Obj> std::list<Obj*> getCrashedObjectListPrecisely(float _x, float _y, bool _isPredict = false);
		template<typename Obj> std::list<Object*> getCrashedObjectList(float _x, float _y, bool _isPredict = false);

		//Object* getCrashedObjectPrecisely(size_t _tag, float _x, float _y, bool _isPredict = false);
		//std::list<Object*> getCrashedObjectListPrecisely(size_t _tag, float _x, float _y, bool _isPredict = false);

		void save();
		void spawn();
	protected:
		friend class Room;

		Room* inRoom_;
		size_t kindId_;
		double reviseStateId_, updateStateId_, recheckStateId_, paintId_;
		std::list<Object*>::iterator objectIterator_;
		std::list<Object*>::iterator objectParentIterator_;
		Var insideObjectIterator_;	//Var type: std::list<Obj*>::iterator
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

	template<typename Obj>
	inline Obj* Object::getInsideObject()
	{
		if (!inRoom_)
			return nullptr;
		typename std::list<Obj*>::iterator res;
		insideObjectIterator_ >> res;
		return (*res);
	}

	template<typename Obj>
	inline Obj* Object::getCrashedObjectPrecisely(bool _isPredict)
	{
		if (!inRoom_)
			return nullptr;
		typename std::list<Obj*>* targetList = inRoom_->objectList<Obj>();
		if (targetList != nullptr)
		{
			if (_isPredict)
			{
				for (auto& objPointer : *targetList)
				{
					if (objPointer != this && isCrashed(*objPointer, x, y, objPointer->x, objPointer->y))
						return objPointer;
				}
			}
			else
			{
				for (auto& objPointer : *targetList)
				{
					if (objPointer != this && isCrashed(*objPointer, x, y))
						return objPointer;
				}
			}
		}
		return nullptr;
	}

	template<typename Obj>
	inline Object* Object::getCrashedObject(bool _isPredict)
	{
		if (!inRoom_)
			return nullptr;
		std::list<TypeNode*> targetTypeList = collisionInheritTree.getAllChildList<Obj>();
		std::list<Object*>* targetList;
		while (!targetTypeList.empty())
		{
			targetList = &(inRoom_->objectParentPointerArray[targetTypeList.front()->code]);
			targetTypeList.pop_front();
			if (targetList != nullptr)
			{
				if (_isPredict)
				{
					for (auto& objPointer : *targetList)
					{
						if (objPointer != this && isCrashed(*objPointer, x, y, objPointer->x, objPointer->y))
							return objPointer;
					}
				}
				else
				{
					for (auto& objPointer : *targetList)
					{
						if (objPointer != this && isCrashed(*objPointer, x, y))
							return objPointer;
					}
				}
			}
		}
		return nullptr;
	}

	template<typename Obj>
	inline std::list<Obj*> Object::getCrashedObjectListPrecisely(bool _isPredict)
	{
		typename std::list<Obj*> resultList;
		if (!inRoom_)
			return resultList;
		typename std::list<Obj*>* targetList = inRoom_->objectList<Obj>();
		if (targetList != nullptr)
		{
			if (_isPredict)
			{
				for (auto& objPointer : targetList)
				{
					if (objPointer != this && isCrashed(*objPointer, x, y, objPointer->x, objPointer->y))
						resultList.push_back(objPointer);
				}
			}
			else
			{
				for (auto& objPointer : targetList)
				{
					if (objPointer != this && isCrashed(*objPointer, x, y))
						resultList.push_back(objPointer);
				}
			}
		}
		return resultList;
	}

	template<typename Obj>
	inline std::list<Object*> Object::getCrashedObjectList(bool _isPredict)
	{
		if (!inRoom_)
			return nullptr;
		std::list<TypeNode*> targetTypeList = collisionInheritTree.getAllChildList<Obj>();
		std::list<Object*>* targetList;
		std::list<Object*> resultList;
		while (!targetTypeList.empty())
		{
			targetList = &(inRoom_->objectParentPointerArray[targetTypeList.front()->code]);
			targetTypeList.pop_front();
			if (targetList != nullptr)
			{
				if (_isPredict)
				{
					for (auto& objPointer : *targetList)
					{
						if (objPointer != this && isCrashed(*objPointer, x, y, objPointer->x, objPointer->y))
							resultList.push_back(objPointer);
					}
				}
				else
				{
					for (auto& objPointer : *targetList)
					{
						if (objPointer != this && isCrashed(*objPointer, x, y))
							resultList.push_back(objPointer);
					}
				}
			}
		}
		return resultList;
	}

	template<typename Obj>
	inline Obj* Object::getCrashedObjectPrecisely(float _x, float _y, bool _isPredict)
	{
		if (!inRoom_)
			return nullptr;
		typename std::list<Obj*>* targetList = inRoom_->objectList<Obj>();
		if (targetList != nullptr)
		{
			if (_isPredict)
			{
				for (auto& objPointer : *targetList)
				{
					if (objPointer != this && isCrashed(*objPointer, _x, _y, objPointer->x, objPointer->y))
						return objPointer;
				}
			}
			else
			{
				for (auto& objPointer : *targetList)
				{
					if (objPointer != this && isCrashed(*objPointer, _x, _y))
						return objPointer;
				}
			}
		}
		return nullptr;
	}

	template<typename Obj>
	inline Object* Object::getCrashedObject(float _x, float _y, bool _isPredict)
	{
		if (!inRoom_)
			return nullptr;
		std::list<TypeNode*> targetTypeList = collisionInheritTree.getAllChildList<Obj>();
		std::list<Object*>* targetList;
		while (!targetTypeList.empty())
		{
			targetList = &(inRoom_->objectParentPointerArray[targetTypeList.front()->code]);
			targetTypeList.pop_front();
			if (targetList != nullptr)
			{
				if (_isPredict)
				{
					for (auto& objPointer : *targetList)
					{
						if (objPointer != this && isCrashed(*objPointer, _x, _y, objPointer->x, objPointer->y))
							return objPointer;
					}
				}
				else
				{
					for (auto& objPointer : *targetList)
					{
						if (objPointer != this && isCrashed(*objPointer, _x, _y))
							return objPointer;
					}
				}
			}
		}
		return nullptr;
	}

	template<typename Obj>
	inline std::list<Obj*> Object::getCrashedObjectListPrecisely(float _x, float _y, bool _isPredict)
	{
		typename std::list<Obj*> resultList;
		if (!inRoom_)
			return resultList;
		typename std::list<Obj*>* targetList = inRoom_->objectList<Obj>();
		if (targetList != nullptr)
		{
			if (_isPredict)
			{
				for (auto& objPointer : targetList)
				{
					if (objPointer != this && isCrashed(*objPointer, _x, _y, objPointer->x, objPointer->y))
						resultList.push_back(objPointer);
				}
			}
			else
			{
				for (auto& objPointer : targetList)
				{
					if (objPointer != this && isCrashed(*objPointer, _x, _y))
						resultList.push_back(objPointer);
				}
			}
		}
		return resultList;
	}

	template<typename Obj>
	inline std::list<Object*> Object::getCrashedObjectList(float _x, float _y, bool _isPredict)
	{
		if (!inRoom_)
			return std::list<Object*>();
		std::list<TypeNode*> targetTypeList = collisionInheritTree.getAllChildList<Obj>();
		std::list<Object*>* targetList;
		std::list<Object*> resultList;
		while (!targetTypeList.empty())
		{
			targetList = &(inRoom_->objectParentPointerArray[targetTypeList.front()->code]);
			targetTypeList.pop_front();
			if (targetList != nullptr)
			{
				if (_isPredict)
				{
					for (auto& objPointer : *targetList)
					{
						if (objPointer != this && isCrashed(*objPointer, _x, _y, objPointer->x, objPointer->y))
							resultList.push_back(objPointer);
					}
				}
				else
				{
					for (auto& objPointer : *targetList)
					{
						if (objPointer != this && isCrashed(*objPointer, _x, _y))
							resultList.push_back(objPointer);
					}
				}
			}
		}
		return resultList;
	}
}