#pragma once
#include "../framework.h"
#include "../suku_foundation.h"
#include "../game_saving.h"
#include "object.h"

namespace suku
{
	class Room;
	extern Room* nowRoom;
	extern TypeTree collisionInheritTree;

	template<typename Father, typename Son>
	inline void linkCollisionType();
	template<typename Son>
	void unlinkCollisionType();

	class Room
	{

	public:
		//std::map<size_t, std::list<Var>> objectPointerArray;
		std::map<size_t, Var> objectPointerArray;	//Var type: std::list<Obj*>*
		std::map<size_t, std::list<Object*>> objectParentPointerArray;
		std::map<size_t, std::list<Var>> objectPointerRemoveArray; //Var type: std::list<Obj*>::iterator
		std::map<double, std::list<Object*>> reviseStateArray;
		std::map<double, std::list<Object*>> updateStateArray;
		std::map<double, std::list<Object*>> recheckStateArray;
		std::map<double, std::list<Object*>> paintArray;

		float playerStartX, playerStartY;
		bool hasCreated;
		//std::list<Object*> object_painting_array[VALUE_MAXPAINT];

		Room();
		void setPlayerStart(float _x, float _y);

		template<typename Obj> std::list<Obj*>* objectList();
		template<typename Obj> Obj* findObj(Obj _objectForType, size_t _pos)const;
		Object* findObj(size_t _kindId, size_t _pos);

		//Object* findObjWithPosition(int _kind, double _x, double _y);
		//Object* findObjWithCenterPosition(int _kind, double _x, double _y);

		template<typename Obj> Obj* append(const Obj* _objectPointer);
		template<typename Obj> Obj* create(Obj& _object);
		template<typename Obj> Obj* create(Obj&& _object);
		template<typename Obj, typename ... ObjNext> void create(Obj _firstobject, ObjNext...	_objectnext);
		template<typename Obj> void createFill(Obj _object,
			float _fillwidth, float _fillheight, float _footx, float _footy);
		template<typename Obj> Obj* createCenter(Obj _object);
		template<typename Obj, typename ... ObjNext> void createCenter(Obj _firstobject, ObjNext... _objectnext);
		template<typename Obj> void createCenterFill(Obj _object,
			float _fillwidth, float _fillheight, float _footx, float _footy);

		virtual void onCreate() {};
		virtual void onJoin() {};
		virtual void onRestart() {};
		virtual void onPaintStart() {};
		virtual void onPaintEnd() {};
		virtual void onUpdateStart() {};
		virtual void onUpdateEnd() {};

		void update();
		void paint();
		void additionalFramePaint(float _offset);
		void reset();
	};

	void gotoRoom(Room& _room);




	template<typename Obj>
	inline std::list<Obj*>* Room::objectList()
	{
		typename std::list<Obj*>* result;
		std::map<unsigned long long, Var>::iterator resultVar = objectPointerArray.find(typecode(Obj));
		if (resultVar != objectPointerArray.end())
		{
			objectPointerArray[typecode(Obj)] >> result;
			auto& removeList = objectPointerRemoveArray[typecode(Obj)];
			if (!removeList.empty())
			{
				for (auto& i : removeList)
					result->erase(i.getValue<typename std::list<Obj*>::iterator>());
				removeList.clear();
			}
			return result;
		}
		else return nullptr;
	}

	template<typename Obj>
	inline Obj* Room::findObj(Obj _objectForType, size_t _pos)const
	{
		std::list<Obj*>* objList = objectList<Obj>();
		if (objList != nullptr)
		{
			Obj* objectPointer = nullptr;
			size_t x = 0;
			for (auto& i : *objList)
			{
				x++;
				if (x == _pos)
					return i.getValue(objectPointer);
			}
		}
		return nullptr;
	}

	template<typename Obj>
	inline Obj* Room::append(const Obj* _objectPointer)
	{
		const type_info& type = typeid(Obj);
		const type_info& typeObject = typeid(Object);
		Obj* newObj = _objectPointer;
		Object* newObjParent = &(*newObj);
		std::list<Obj*>* objList = objectList<Obj>();
		std::list<Object*>* objParentList = objectList<Object>();

		bool isFirstObjInClass = false;

		if (objParentList == nullptr)
		{
			objParentList = new std::list<Object*>;
			Var objParentListVar;
			objParentListVar << objParentList;
			objectPointerArray[typecode(Object)] = objParentListVar;
		}
		if (objList == nullptr)
		{
			isFirstObjInClass = true;
			objList = new std::list<Obj*>;
			Var objListVar;
			objListVar << objList;
			objectPointerArray[typecode(Obj)] = objListVar;
			collisionInheritTree.append<Obj>();
		}

		newObj->kindId_ = type.hash_code();
		newObj->inRoom_ = this;

		newObj->insideObjectIterator_ << objList->insert(objList->end(), newObj);

		newObj->objectIterator_ = objParentList->insert(objParentList->end(), newObjParent);

		newObj->objectParentIterator_ =
			objectParentPointerArray[typecode(Obj)].insert(objectParentPointerArray[typecode(Obj)].end(), newObjParent);

		newObj->reviseStateIterator_ =
			reviseStateArray[newObj->reviseStateId_].insert(reviseStateArray[newObj->reviseStateId_].end(), newObjParent);

		newObj->updateStateIterator_ =
			updateStateArray[newObj->updateStateId_].insert(updateStateArray[newObj->updateStateId_].end(), newObjParent);

		newObj->recheckStateIterator_ =
			recheckStateArray[newObj->recheckStateId_].insert(recheckStateArray[newObj->recheckStateId_].end(), newObjParent);

		newObj->paintIterator_ =
			paintArray[newObj->paintId_].insert(paintArray[newObj->paintId_].end(), newObjParent);

		if (isFirstObjInClass)
		{
			Obj::classInitialize();
		}

		return newObj;
	}

	template<typename Obj>
	inline Obj* Room::create(Obj& _object)
	{
		const type_info& type = typeid(Obj);
		const type_info& typeObject = typeid(Object);
		Obj* newObj = new Obj(_object);
		Object* newObjParent = &(*newObj);
		std::list<Obj*>* objList = objectList<Obj>();
		std::list<Object*>* objParentList = objectList<Object>();

		bool isFirstObjInClass = false;
		if (objParentList == nullptr)
		{
			objParentList = new std::list<Object*>;
			Var objParentListVar;
			objParentListVar << objParentList;
			objectPointerArray[typecode(Object)] = objParentListVar;
		}
		if (objList == nullptr)
		{
			isFirstObjInClass = true;
			objList = new std::list<Obj*>;
			Var objListVar;
			objListVar << objList;
			objectPointerArray[typecode(Obj)] = objListVar;
			collisionInheritTree.append<Obj>();
		}

		newObj->kindId_ = type.hash_code();
		newObj->inRoom_ = this;

		newObj->insideObjectIterator_ << objList->insert(objList->end(), newObj);

		newObj->objectIterator_ = objParentList->insert(objParentList->end(), newObjParent);

		newObj->objectParentIterator_ =
			objectParentPointerArray[typecode(Obj)].insert(objectParentPointerArray[typecode(Obj)].end(), newObjParent);

		newObj->reviseStateIterator_ =
			reviseStateArray[newObj->reviseStateId_].insert(reviseStateArray[newObj->reviseStateId_].end(), newObjParent);

		newObj->updateStateIterator_ =
			updateStateArray[newObj->updateStateId_].insert(updateStateArray[newObj->updateStateId_].end(), newObjParent);

		newObj->recheckStateIterator_ =
			recheckStateArray[newObj->recheckStateId_].insert(recheckStateArray[newObj->recheckStateId_].end(), newObjParent);

		newObj->paintIterator_ =
			paintArray[newObj->paintId_].insert(paintArray[newObj->paintId_].end(), newObjParent);

		if (isFirstObjInClass)
		{
			Obj::classInitialize();
		}

		return newObj;
	}

	template<typename Obj>
	inline Obj* Room::create(Obj&& _object)
	{
		const type_info& type = typeid(Obj);
		const type_info& typeObject = typeid(Object);
		Obj* newObj = new Obj(std::move(_object));
		Object* newObjParent = &(*newObj);
		std::list<Obj*>* objList = objectList<Obj>();
		std::list<Object*>* objParentList = objectList<Object>();

		bool isFirstObjInClass = false;
		if (objParentList == nullptr)
		{
			objParentList = new std::list<Object*>;
			Var objParentListVar;
			objParentListVar << objParentList;
			objectPointerArray[typecode(Object)] = objParentListVar;
			collisionInheritTree.append<Object>();
		}
		if (objList == nullptr)
		{
			isFirstObjInClass = true;
			objList = new std::list<Obj*>;
			Var objListVar;
			objListVar << objList;
			objectPointerArray[typecode(Obj)] = objListVar;
			collisionInheritTree.append<Obj>();
		}

		newObj->kindId_ = type.hash_code();
		newObj->inRoom_ = this;

		newObj->insideObjectIterator_ << objList->insert(objList->end(), newObj);

		newObj->objectIterator_ = objParentList->insert(objParentList->end(), newObjParent);

		newObj->objectParentIterator_ =
			objectParentPointerArray[typecode(Obj)].insert(objectParentPointerArray[typecode(Obj)].end(), newObjParent);

		newObj->reviseStateIterator_ =
			reviseStateArray[newObj->reviseStateId_].insert(reviseStateArray[newObj->reviseStateId_].end(), newObjParent);

		newObj->updateStateIterator_ =
			updateStateArray[newObj->updateStateId_].insert(updateStateArray[newObj->updateStateId_].end(), newObjParent);

		newObj->recheckStateIterator_ =
			recheckStateArray[newObj->recheckStateId_].insert(recheckStateArray[newObj->recheckStateId_].end(), newObjParent);

		newObj->paintIterator_ =
			paintArray[newObj->paintId_].insert(paintArray[newObj->paintId_].end(), newObjParent);

		if (isFirstObjInClass)
		{
			Obj::classInitialize();
		}

		return newObj;
	}

	template<typename Obj, typename ...ObjNext>
	inline void Room::create(Obj _firstobject, ObjNext ..._objectnext)
	{
		create(_firstobject);
		create(_objectnext...);
	}

	template<typename Obj>
	inline void Room::createFill(Obj _object, float _fillwidth, float _fillheight, float _footx, float _footy)
	{
		float i, j;
		for (i = _object.x; i <= _object.x + _fillwidth; i += _footx)
			for (j = _object.y; j <= _object.y + _fillheight; j += _footy)
			{
				Obj a(_object);
				a.x = a.spawnX = i;
				a.y = a.spawnY = j;
				create(a);
			}
	}

	template<typename Obj>
	inline Obj* Room::createCenter(Obj _object)
	{
		_object.setPlaceAndSave(_object.x - _object.nowState()->centerX, _object.y - _object.nowState()->centerY);
		return create(_object);
	}

	template<typename Obj, typename ...ObjNext>
	inline void Room::createCenter(Obj _firstobject, ObjNext ..._objectnext)
	{
		createCenter(_firstobject);
		createCenter(_objectnext...);
	}

	template<typename Obj>
	inline void Room::createCenterFill(Obj _object, float _fillwidth, float _fillheight, float _footx, float _footy)
	{
		_object.setPlaceAndSave(_object.x - _object.nowState()->centerX, _object.y - _object.nowState()->centerY);
		createFill(_object, _fillwidth, _fillheight, _footx, _footy);
	}

	template<typename Father, typename Son>
	inline void linkCollisionType()
	{
		collisionInheritTree.unlink<Son>();
		collisionInheritTree.link<Father, Son>();
	}

	template<typename Son>
	void unlinkCollisionType()
	{
		collisionInheritTree.unlink<Son>();
	}
}