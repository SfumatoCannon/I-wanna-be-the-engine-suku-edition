#pragma once
#include <suku_draw/paint_layer.h>
#include <suku_foundation/type_tree.h>
#include <map>
#include <memory>

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

		void setObjectRevisePriority(Object* _object, double _newId);
		void setObjectUpdatePriority(Object* _object, double _newId);
		void setObjectRecheckPriority(Object* _object, double _newId);
		void setObjectPaintPriority(Object* _object, double _newId);

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
		std::map<Typecode, std::list<std::shared_ptr<Object>>> objectPointerArray_;
		std::map<double, std::list<std::shared_ptr<Object>>> reviseStateArray_;
		std::map<double, std::list<std::shared_ptr<Object>>> updateStateArray_;
		std::map<double, std::list<std::shared_ptr<Object>>> recheckStateArray_;
		std::map<double, std::list<std::shared_ptr<Object>>> paintArray_;
		template<typename Obj> void createObjectList();
	};

	void gotoRoom(Room& _room);
}

#include "room.inl"