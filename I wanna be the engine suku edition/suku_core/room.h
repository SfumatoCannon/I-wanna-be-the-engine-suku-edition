#pragma once
#include "../suku_draw/paint_layer.h"
#include "../suku_foundation/type_tree.h"
#include <map>
#include <memory>

namespace suku
{
	class Object;
	class Room;
	class RoomCollisionPool;

	extern Room* nowRoom;

	class Room
	{
	public:
		bool hasCreated = false;
		Room() { collisionPool_ = std::make_unique<RoomCollisionPool>(); }

		template<typename Obj> std::list<Obj*> getObjectList();
		Object* findObj(Typecode _kindId, size_t _pos);

		template<typename Obj> Obj* append(std::shared_ptr<Obj> _objectPointer);
		template<typename Obj> Obj* create(Obj&& _object);
		template<typename Obj, typename ... Args> Obj* create(Args&&... args);
		template<typename ... Objs> void create(Objs&&... _objects);
		template<typename Obj> void createFill(Obj _object,
			float _fillwidth, float _fillheight, float _footx, float _footy);
		template<typename Obj> Obj* createCenter(Obj _object);
		template<typename Obj, typename ... ObjNext> void createCenter(Obj _firstobject, ObjNext... _objectnext);
		template<typename Obj> void createCenterFill(Obj _object,
			float _fillwidth, float _fillheight, float _footx, float _footy);

		void remove(Object* _object);
		void destroy(Object* _object);
		void removeImmediately(Object* _object);
		void destroyImmediately(Object* _object);

		void setObjectRevisePriority(Object* _object, double _newId);
		void setObjectUpdatePriority(Object* _object, double _newId);
		void setObjectRecheckPriority(Object* _object, double _newId);
		void setObjectPaintPriority(Object* _object, double _newId);

		template<typename Obj = Object> Obj* getCrashedObject(Object* _sourceObj);
		template<typename Obj = Object> std::list<Obj*> getCrashedObjectList(Object* _sourceObj);

		virtual void onCreate() { displayLayer_.newLayer(800, 608); }
		virtual void onEntering() {}
		virtual void onRestart() {}
		virtual void onPaintStart() { displayLayer_.clear(); }
		virtual void onPaintEnd() {}
		virtual void onUpdateStart() {}
		virtual void onUpdateEnd() {}

		void update();
		void paint();
		void additionalFramePaint(float _offset);
		void reset();
	private:
		PaintLayer displayLayer_;
		std::map<Typecode, std::list<std::shared_ptr<Object>>> objectPointerArray_;
		std::unique_ptr<RoomCollisionPool> collisionPool_;
		std::map<double, std::list<std::shared_ptr<Object>>> preUpdateArray_;
		std::map<double, std::list<std::shared_ptr<Object>>> updateArray_;
		std::map<double, std::list<std::shared_ptr<Object>>> postUpdateArray_;
		std::map<double, std::list<std::shared_ptr<Object>>> paintArray_;
		template<typename Obj> void createObjectList();
	};

	void gotoRoom(Room& _room);
}

#include "room.inl"