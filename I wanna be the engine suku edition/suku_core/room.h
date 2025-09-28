#pragma once
#include "../framework.h"
#include "../suku_foundation.h"
#include "../game_saving.h"

namespace suku
{
	class Object;
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
}

#include "room.inl"