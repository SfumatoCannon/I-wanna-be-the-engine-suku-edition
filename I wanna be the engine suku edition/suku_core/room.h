#pragma once
#include "../framework.h"
#include "../suku_foundation/includes.h"
#include <suku_draw/paint_layer.h>

namespace suku
{
	class Object;
	class Room;
	extern Room* nowRoom;

	class Room
	{
	public:
		float playerStartX = 0.0, playerStartY = 0.0;
		bool hasCreated = false;
		Room() = default;
		void setPlayerStart(float _x, float _y);

		template<typename Obj> std::list<Obj*> getObjectList();
		Object* findObj(Typecode _kindId, size_t _pos);

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

		virtual void onCreate() { displayLayer_.newLayer(800, 608); }
		virtual void onJoin() {}
		virtual void onRestart() {}
		virtual void onPaintStart() {}
		virtual void onPaintEnd() {}
		virtual void onUpdateStart() {}
		virtual void onUpdateEnd() {}

		void update();
		void paint();
		void additionalFramePaint(float _offset);
		void reset();
	private:
		PaintLayer displayLayer_;
		std::map<Typecode, std::list<Object*>> objectPointerArray_;
		std::map<Typecode, std::list<std::list<Object*>::iterator>> objectPointerRemoveArray_;
		std::map<double, std::list<Object*>> reviseStateArray_;
		std::map<double, std::list<Object*>> updateStateArray_;
		std::map<double, std::list<Object*>> recheckStateArray_;
		std::map<double, std::list<Object*>> paintArray_;
		template<typename Obj> void createObjectList();
	};

	void gotoRoom(Room& _room);
}

#include "room.inl"