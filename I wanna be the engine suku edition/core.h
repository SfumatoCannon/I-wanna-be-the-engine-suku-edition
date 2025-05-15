#pragma once
#include "suku_sounds.h"
#include "suku_drawing.h"
#include "suku_foundation.h"
#include "game_saving.h"
#include "global_value.h"

#define SPR_INIT(sprPointer, func) static Sprite* temp_##sprPointer = sprPointer = new Sprite func;

namespace suku
{
	class CollisionBox;
	class BitmapCollisionBox;
	class ShapeCollisionBox;
	class SpriteZero;
	class BitmapSpriteZero;
	class ShapeSpriteZero;
	class Sprite;
	class Object;
	class Group;
	class Room;

	extern Room* nowRoom;
	extern TypeTree collisionInheritTree;

	class CollisionBox
	{
	public:
		virtual void release() = 0;
		virtual bool isCrashed(Transform _transform, const BitmapCollisionBox& _other, Transform _otherTransform)const = 0;
		virtual bool isCrashed(Transform _transform, const ShapeCollisionBox& _other, Transform _otherTransform)const = 0;
		virtual bool isCrashed(Transform _transform, const CollisionBox& _other, Transform _otherTransform)const = 0;
	};

	class BitmapCollisionBox :public CollisionBox
	{
	public:
		bool** hitArea;
		int hitX, hitY;
		UINT hitWidth, hitHeight;
		BitmapCollisionBox(bool** _hitArea, int _hitX, int _hitY, UINT _hitWidth, UINT _hitHeight);
		BitmapCollisionBox(IWICBitmap* _bitmap, float _alphaThreshold = 0.0f);
		BitmapCollisionBox(Bitmap* _pBitmap, float _alphaThreshold = 0.0f);
		~BitmapCollisionBox();

		virtual void release()override { free2D(hitArea, hitWidth, hitHeight); }

		virtual bool isCrashed(Transform _transform, const BitmapCollisionBox& _other, Transform _otherTransform)const override;
		virtual bool isCrashed(Transform _transform, const ShapeCollisionBox& _other, Transform _otherTransform)const override;
		virtual bool isCrashed(Transform _transform, const CollisionBox& _other, Transform _otherTransform)const override;
	};

	class ShapeCollisionBox :public CollisionBox
	{
	public:
		Shape shape;
		ShapeCollisionBox(const Shape& _shape);
		//~ShapeCollisionBox();

		virtual void release()override { shape.originalGeometry->Release(), shape.currentGeometry->Release(); }
		virtual bool isCrashed(Transform _transform, const BitmapCollisionBox& _other, Transform _otherTransform)const override;
		virtual bool isCrashed(Transform _transform, const ShapeCollisionBox& _other, Transform _otherTransform)const override;
		virtual bool isCrashed(Transform _transform, const CollisionBox& _other, Transform _otherTransform)const override;
	};

	class SpriteZero
	{
	public:
		UINT height, width;
		float centerX, centerY;
		CollisionBox* hitArea;
		SpriteZero() : height(0), width(0), centerX(0), centerY(0), hitArea(nullptr) {}

		virtual void paint(float _x, float _y,
			float _xscale = 1.0, float _yscale = 1.0, float _alpha = 1.0, float _angle = 0.0) {}
		virtual void paint(float _x, float _y,
			Transform _transform, float _alpha = 1.0) {}
		virtual void paint(Transform _transform, float _alpha = 1.0) {}
		bool isCrashed(Transform _transform, const SpriteZero& _other, Transform _otherTransform)const;
		bool isCrashed(Transform _transform, const SpriteZero* _other, Transform _otherTransform)const;
	};

	class BitmapSpriteZero :public SpriteZero
	{
	public:
		BitmapSpriteZero();
		BitmapSpriteZero(const Shape& _collisionBox, const Bitmap& _bitmap, float _centerX = 0.0f, float _centerY = 0.0f);
		BitmapSpriteZero(UINT _width, UINT _height, const Shape& _collisionBox, float _centerX = 0.0f, float _centerY = 0.0f);
		BitmapSpriteZero(UINT _width, UINT _height, const BitmapCollisionBox& _collisionBox,
			float _centerX = 0, float _centerY = 0, String _path = String());

		BitmapSpriteZero(String _path, const Shape& _collisionBox, float _centerX = 0.0f, float _centerY = 0.0f);
		BitmapSpriteZero(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height,
			const Shape& _collisionBox, float _centerX = 0.0f, float _centerY = 0.0f);


		BitmapSpriteZero(String _path, float _centerX = 0, float _centerY = 0, float _alphaThreshold = 0.0f);
		BitmapSpriteZero(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height,
			float _centerX = 0.0f, float _centerY = 0.0f, float _alphaThreshold = 0.0f);


		void catchBitmap(String _path, UINT _startX = 0, UINT _startY = 0);
		void catchBitmapAndSize(String _path);

		virtual void paint(float _x, float _y,
			float _xScale = 1.0, float _yScale = 1.0, float _alpha = 1.0, float _angle = 0.0) override;
		virtual void paint(float _x, float _y,
			Transform _transform, float _alpha = 1.0) override;
		virtual void paint(Transform _transform, float _alpha = 1.0) override;

	private:
		Bitmap* pBitmap_;
	};

	class ShapeSpriteZero :public SpriteZero
	{
	public:
		Shape shape;
		ID2D1Brush* fillBrush;
		ID2D1Brush* outlineBrush;
		ID2D1StrokeStyle* outlineStrokeStyle;
		float outlineWidth;
		ShapeSpriteZero(const Shape& _shape, ID2D1Brush* _fillBrush = nullptr,
			ID2D1Brush* _outlineBrush = nullptr, float _outlineWidth = 1.0f, ID2D1StrokeStyle* _outlineStrokeStyle = nullptr);
		ShapeSpriteZero(const Shape& _shape, const Color& _fillColor);
		ShapeSpriteZero(const Shape& _shape, const Color& _fillColor,
			const Color& _outlineColor, float _outlineWidth = 1.0f, ID2D1StrokeStyle* _outlineStrokeStyle = nullptr);
		~ShapeSpriteZero();
		void setShapeTransform(Transform _transform);
		void paint(float _x, float _y,
			float _xScale = 1.0f, float _yScale = 1.0f, float _angle = 0.0f);
		void paint(float _x, float _y, Transform _paintingTransform);
		void paint(Transform _paintingTransform);
		void setFillColor(const Color& _color);
		void setOutlineColor(const Color& _color);
		void setOutlineWidth(int _width);

		virtual void paint(float _x, float _y,
			float _xScale = 1.0f, float _yScale = 1.0f, float _alpha = 1.0f, float _angle = 0.0f) override;
		virtual void paint(float _x, float _y,
			Transform _transform, float _alpha = 1.0) override;
		virtual void paint(Transform _transform, float _alpha = 1.0) override;
	};

	class Sprite
	{
	public:
		std::vector<SpriteZero*> bodyList;
		Sprite();
		template<typename SprZ> Sprite(const SprZ& _spriteZ);
		template<typename SprZ, typename... SprZNext> Sprite(int _flipTime, const SprZ& _spriteZ, const SprZNext&... _spriteZNext);
		
		//Load Sprite Directly from the long sprite bitmap; width and height will be auto calculated.
		Sprite(String _path, UINT _amount, int _flipTime, 
			const Shape& _collisionBox, float _centerX = 0.0f, float _centerY = 0.0f);
		Sprite(String _path, UINT _amount, int _flipTime,
			float _centerX = 0.0f, float _centerY = 0.0f, float _alphaThreshold = 0.0f);
		//Load Sprite Directly from the long sprite bitmap, with given starting position and size.
		Sprite(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height, UINT _amount, int _flipTime,
			const Shape& _collisionBox, float _centerX = 0.0f, float _centerY = 0.0f);
		Sprite(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height, UINT _amount, int _flipTime,
			float _centerX = 0.0f, float _centerY = 0.0f, float _alphaThreshold = 0.0f);

		template<typename SprZ> void init(const SprZ& _spriteZ);
		template<typename SprZ, typename... SprZNext> void init(int _flipTime, const SprZ& _spriteZ, const SprZNext&... _spriteZNext);
		void operator= (Sprite& _sprite)const = delete;

		void setSpeed(int _speed);
		template<typename SprZ> void push(const SprZ& _spriteZ);
		template<typename SprZ, typename... SprZNext> void push(const SprZ& _spriteZ, const SprZNext&... _spriteZNext);
		SpriteZero* getState(int _wp);
	private:
		int flipTime_;
	};

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

		inline float totalHspeed()const;
		inline float totalVspeed()const;

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
	template<typename Father, typename Son> void linkCollisionType();
	template<typename Son> void unlinkCollisionType();

	class Group
	{
	private:
		std::list<Object*> member_;
		std::map<Object*, std::list<Object*>::iterator> memberIterator_;
	public:
		Group operator&(Group& _group);
		Group operator|(Group& _group);
		Group operator^(Group& _group);
		size_t getSize();
		size_t append(Object* _object);
		size_t append(Object* _firstObject, Object* _secondObject, ...);
		size_t append(std::list<Object*> _objectList);
		size_t appendSelectively(Room* _room, std::function<bool(Object*)> _function);
		size_t appendSelectively(Group* _group, std::function<bool(Object*)> _function);
		size_t appendSelectively(std::list<Object*> _objectList, std::function<bool(Object*)> _function);
		bool isInclude(Object* _object);
		void removeObj(size_t _pos);
		void removeObj(Object* _obj);
		Object* findObj(size_t _pos);
		void setAction(std::function<void(Object*, int)> _function);
	};


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

	template<typename SprZ>
	inline Sprite::Sprite(const SprZ& _spriteZ)
	{
		flipTime_ = 1;
		push(_spriteZ);
	}

	template<typename SprZ, typename ...SprZNext>
	inline Sprite::Sprite(int _flipTime, const SprZ& _spriteZ, const SprZNext & ..._spriteZNext)
	{
		flipTime_ = _flipTime;
		push(_spriteZ);
		push(_spriteZNext...);
	}

	template<typename SprZ>
	inline void Sprite::init(const SprZ& _spriteZ)
	{
		bodyList.clear();
		flipTime_ = 1;
		push(_spriteZ);
	}

	template<typename SprZ, typename ...SprZNext>
	inline void Sprite::init(int _flipTime, const SprZ& _spriteZ, const SprZNext & ..._spriteZNext)
	{
		bodyList.clear();
		flipTime_ = _flipTime;
		push(_spriteZ);
		push(_spriteZNext...);
	}

	template<typename SprZ>
	inline void Sprite::push(const SprZ& _spriteZ)
	{
		SprZ* newSpriteZ = new SprZ(_spriteZ);
		bodyList.push_back(newSpriteZ);
	}

	template<typename SprZ, typename ...SprZNext>
	inline void Sprite::push(const SprZ& _spriteZ, const SprZNext & ..._spriteZNext)
	{
		push(_spriteZ);
		push(_spriteZNext...);
	}
}
