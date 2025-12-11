#pragma once
#include "../framework.h"
#include "../suku_draw/includes.h"


namespace suku
{
	class CollisionBox;
	class BitmapCollisionBox;
	class ShapeCollisionBox;

	class CollisionBox
	{
	public:
		virtual bool isCrashed(Transform _transform, const BitmapCollisionBox& _other, Transform _otherTransform)const = 0;
		virtual bool isCrashed(Transform _transform, const ShapeCollisionBox& _other, Transform _otherTransform)const = 0;
		virtual bool isCrashed(Transform _transform, const CollisionBox& _other, Transform _otherTransform)const = 0;
	};

	using namespace suku::memory;
	class BitmapCollisionBox :public CollisionBox
	{
	public:
		Array2D<bool> hitArea;
		int hitX, hitY;
		UINT hitWidth, hitHeight;
		BitmapCollisionBox(Array2D<bool> _hitArea, int _hitX, int _hitY, UINT _hitWidth, UINT _hitHeight);
		BitmapCollisionBox(Bitmap* _pBitmap, float _alphaThreshold = 0.0f);
		~BitmapCollisionBox();

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

		virtual bool isCrashed(Transform _transform, const BitmapCollisionBox& _other, Transform _otherTransform)const override;
		virtual bool isCrashed(Transform _transform, const ShapeCollisionBox& _other, Transform _otherTransform)const override;
		virtual bool isCrashed(Transform _transform, const CollisionBox& _other, Transform _otherTransform)const override;
	};
}