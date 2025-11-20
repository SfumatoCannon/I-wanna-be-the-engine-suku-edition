#pragma once
#include "../framework.h"
#include "../suku_foundation/includes.h"

namespace suku
{
	class Object;
	class Room;
	extern Room* nowRoom;

	template<typename Father, typename Son>
	inline void linkCollisionType();
	template<typename Son>
	void unlinkCollisionType();

	class Room
	{
	private:
		std::map<Typecode, std::list<Object*>> objectPointerArray_;
		std::map<Typecode, std::list<std::list<Object*>::iterator>> objectPointerRemoveArray_;
		std::map<double, std::list<Object*>> reviseStateArray_;
		std::map<double, std::list<Object*>> updateStateArray_;
		std::map<double, std::list<Object*>> recheckStateArray_;
		std::map<double, std::list<Object*>> paintArray_;
		template<typename Obj> void createObjectList();
	public:
		//std::map<size_t, std::list<Var>> objectPointerArray_;

		float playerStartX, playerStartY;
		bool hasCreated;
		//std::list<Object*> object_painting_array[VALUE_MAXPAINT];

		Room();
		void setPlayerStart(float _x, float _y);

		template<typename Obj> std::list<Obj*> getObjectList();
		Object* findObj(Typecode _kindId, size_t _pos);

		//Object* findObjWithPosition(int _kind, double _x, double _y);
		//Object* findObjWithCenterPosition(int _kind, double _x, double _y);

		template<typename Obj> Obj* append(Obj* _objectPointer);
		template<typename Obj> Obj* create(Obj& _object);
		template<typename Obj> Obj* create(Obj&& _object);
		template<typename Obj, typename ... ObjNext> void create(Obj _firstobject, ObjNext...	_objectnext);
		template<typename Obj> void createFill(Obj _object,
			float _fillwidth, float _fillheight, float _footx, float _footy);
		template<typename Obj> Obj* createCenter(Obj _object);
		template<typename Obj, typename ... ObjNext> void createCenter(Obj _firstobject, ObjNext... _objectnext);
		template<typename Obj> void createCenterFill(Obj _object,
			float _fillwidth, float _fillheight, float _footx, float _footy);

		void remove(Object* _object);

		void modifyObjectRevisePriority(Object* _object, double _newId);
		void modifyObjectUpdatePriority(Object* _object, double _newId);
		void modifyObjectRecheckPriority(Object* _object, double _newId);
		void modifyObjectPaintPriority(Object* _object, double _newId);

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
}

#include "room.inl"