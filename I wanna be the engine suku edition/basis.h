#pragma once
#include "suku_drawing.h"
#include "suku_maths.h"
#include "game_saving.h"
#include "global_value.h"

namespace suku
{
	class CollisionBox;
	class BitmapCollisionBox;
	class ShapeCollisionBox;
	class SpriteZ;
	class BitmapSpriteZ;
	class ShapeSpriteZ;
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
		BitmapCollisionBox(IWICBitmap* _bitmap, int _hitX, int _hitY, BYTE _alphaThreshold = 0);
		BitmapCollisionBox(Bitmap* _pBitmap, int _hitX, int _hitY, BYTE _alphaThreshold = 0);
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

	class SpriteZ
	{
	public:
		UINT height, width;
		float centerX, centerY;
		CollisionBox* hitArea;
		SpriteZ() : height(0), width(0), centerX(0), centerY(0), hitArea(nullptr) {}

		virtual void paint(float _x, float _y,
			float _xscale = 1.0, float _yscale = 1.0, float _alpha = 1.0, float _angle = 0.0) = 0;
		virtual void paint(float _x, float _y,
			Transform _transform, float _alpha = 1.0) = 0;
		virtual void paint(Transform _transform, float _alpha = 1.0) = 0;
		bool isCrashed(Transform _transform, const SpriteZ& _other, Transform _otherTransform)const;
		bool isCrashed(Transform _transform, const SpriteZ* _other, Transform _otherTransform)const;
	};

	class BitmapSpriteZ :public SpriteZ
	{
	public:

		BitmapSpriteZ(const Shape& _collisionBox,
			UINT _width, UINT _height, float _centerX, float _centerY,
			LPCTSTR _path);
		BitmapSpriteZ(UINT _width, UINT _height,
			int _hitboxX, int _hitboxY,
			UINT _hitboxWidth, UINT _hitboxWeight,
			float _centerX, float _centerY,
			bool _getAutoHitbox = false, LPCTSTR _path = nullptr);
		BitmapSpriteZ(LPCTSTR _path, float _centerX = 0, float _centerY = 0);
		BitmapSpriteZ(BitmapCollisionBox _collisionBox,
			UINT _width, UINT _height, float _centerX = 0, float _centerY = 0, LPCTSTR _path = nullptr);
		BitmapSpriteZ();

		void catchBitmap(LPCTSTR _path);
		//void catchWicBitmap(LPCTSTR _path);
		//void catchD2dBitmap();

		virtual void paint(float _x, float _y,
			float _xScale = 1.0, float _yScale = 1.0, float _alpha = 1.0, float _angle = 0.0) override;
		virtual void paint(float _x, float _y,
			Transform _transform, float _alpha = 1.0) override;
		virtual void paint(Transform _transform, float _alpha = 1.0) override;

	private:
		Bitmap* pBitmap_;
		//IWICBitmap* wicBitmap_;
		//ID2D1Bitmap* d2d1Bitmap_;
	};

	class ShapeSpriteZ :public SpriteZ
	{
	public:
		Shape shape;
		ID2D1Brush* fillBrush;
		ID2D1Brush* outlineBrush;
		ID2D1StrokeStyle* outlineStrokeStyle;
		float outlineWidth;
		ShapeSpriteZ(const Shape& _shape);
		void setShapeTransform(Transform _transform);
		void paint(float _x, float _y,
			float _xScale = 1.0, float _yScale = 1.0, float _angle = 0.0);
		void paint(float _x, float _y, Transform _paintingTransform);
		void paint(Transform _paintingTransform);
		void setFillColor(int _R, int _G, int _B, float _alpha = 1.0);
		void setOutlineColor(int _R, int _G, int _B, float _alpha = 1.0);
		void setOutlineWidth(int _width);

		virtual void paint(float _x, float _y,
			float _xScale = 1.0, float _ycale = 1.0, float _alpha = 1.0, float _angle = 0.0) override;
		virtual void paint(float _x, float _y,
			Transform _transform, float _alpha = 1.0) override;
		virtual void paint(Transform _transform, float _alpha = 1.0) override;
	};

	class Sprite
	{
	public:
		std::vector<BitmapSpriteZ> bodyList;
		Sprite();
		//Sprite(BitmapSpriteZ _spriteZ);
		//Sprite(BitmapSpriteZ _spriteZ1, BitmapSpriteZ _spriteZ2, ...);
		void operator= (Sprite& _sprite)const = delete;
		void operator= (BitmapSpriteZ _spriteZ);
		Sprite operator+ (BitmapSpriteZ _spriteZ);
		void setSpeed(unsigned short _speed);
		void push(const BitmapSpriteZ& _spriteZ);
		BitmapSpriteZ* getState(unsigned short _wp);
	private:
		unsigned short speed_;
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
		size_t tag()const { return tag_; }

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
		BitmapSpriteZ* nowState()const;

		void remove();
		void destroy();

		void updateFunction();
		virtual void onAppearing();
		virtual void onRestarting();
		virtual void reviseState();
		virtual void updateState();
		virtual void recheckState();

		inline float totalHspeed()const;
		inline float totalVspeed()const;

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

		void setRoom(Room* _room);
		void save();
		void spawn();
	protected:
		friend class Room;

		Room* inRoom_;
		size_t kindId_;
		size_t tag_;
		double reviseStateId_, updateStateId_, recheckStateId_, paintId_;
		std::list<Object*>::iterator objectIterator_;
		std::list<Object*>::iterator objectParentIterator_;
		Var insideObjectIterator_;	//Var type: std::list<Obj*>::iterator
		std::list<Object*>::iterator tagIterator_;
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
		std::map<size_t, std::list<Object*>> tagArray;
		std::map<double, std::list<Object*>> reviseStateArray;
		std::map<double, std::list<Object*>> updateStateArray;
		std::map<double, std::list<Object*>> recheckStateArray;
		std::map<double, std::list<Object*>> paintArray;

		float playerStartX, playerStartY;
		//std::list<Object*> object_painting_array[VALUE_MAXPAINT];

		Room();
		void setPlayerStart(float _x, float _y);

		template<typename Obj> std::list<Obj*>* objectList();
		template<typename Obj> Obj* findObj(Obj _objectForType, size_t _pos)const;
		Object* findObj(size_t _kindId, size_t _pos);

		//Object* findObjWithPosition(unsigned short _kind, double _x, double _y);
		//Object* findObjWithCenterPosition(unsigned short _kind, double _x, double _y);

		template<typename Obj> Obj* append(const Obj* _objectPointer);
		template<typename Obj> Obj* create(Obj _object);
		template<typename Obj, typename ... ObjNext> void create(Obj _firstobject, ObjNext...	_objectnext);
		template<typename Obj> void createFill(Obj _object,
			float _fillwidth, float _fillheight, float _footx, float _footy);
		template<typename Obj> Obj* createCenter(Obj _object);
		template<typename Obj, typename ... ObjNext> void createCenter(Obj _firstobject, ObjNext... _objectnext);
		template<typename Obj> void createCenterFill(Obj _object,
			float _fillwidth, float _fillheight, float _footx, float _footy);

		void update();
		void paint();
		void additionalFramePaint(float _offset);
		void reset();
		//void paintBody();
		virtual void recheckState();
	};






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

		if (objParentList == nullptr)
		{
			objParentList = new std::list<Object*>;
			Var objParentListVar;
			objParentListVar << objParentList;
			objectPointerArray[typecode(Object)] = objParentListVar;
		}
		if (objList == nullptr)
		{
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

		newObj->tagIterator_ =
			tagArray[newObj->tag_].insert(tagArray[newObj->tag_].end(), newObj);


		newObj->reviseStateIterator_ =
			reviseStateArray[newObj->reviseStateId_].insert(reviseStateArray[newObj->reviseStateId_].end(), newObjParent);

		newObj->updateStateIterator_ =
			updateStateArray[newObj->updateStateId_].insert(updateStateArray[newObj->updateStateId_].end(), newObjParent);

		newObj->recheckStateIterator_ =
			recheckStateArray[newObj->recheckStateId_].insert(recheckStateArray[newObj->recheckStateId_].end(), newObjParent);

		newObj->paintIterator_ =
			paintArray[newObj->paintId_].insert(paintArray[newObj->paintId_].end(), newObjParent);

		return newObj;
	}

	template<typename Obj>
	inline Obj* Room::create(Obj _object)
	{
		const type_info& type = typeid(Obj);
		const type_info& typeObject = typeid(Object);
		Obj* newObj = new Obj;
		*newObj = _object;
		Object* newObjParent = &(*newObj);
		std::list<Obj*>* objList = objectList<Obj>();
		std::list<Object*>* objParentList = objectList<Object>();

		if (objParentList == nullptr)
		{
			objParentList = new std::list<Object*>;
			Var objParentListVar;
			objParentListVar << objParentList;
			objectPointerArray[typecode(Object)] = objParentListVar;
		}
		if (objList == nullptr)
		{
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

		newObj->tagIterator_ =
			tagArray[newObj->tag_].insert(tagArray[newObj->tag_].end(), newObj);


		newObj->reviseStateIterator_ =
			reviseStateArray[newObj->reviseStateId_].insert(reviseStateArray[newObj->reviseStateId_].end(), newObjParent);

		newObj->updateStateIterator_ =
			updateStateArray[newObj->updateStateId_].insert(updateStateArray[newObj->updateStateId_].end(), newObjParent);

		newObj->recheckStateIterator_ =
			recheckStateArray[newObj->recheckStateId_].insert(recheckStateArray[newObj->recheckStateId_].end(), newObjParent);

		newObj->paintIterator_ =
			paintArray[newObj->paintId_].insert(paintArray[newObj->paintId_].end(), newObjParent);

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
}
