#pragma once
#include "../framework.h"
#include "../suku_draw/includes.h"


namespace suku
{
	class ICollisionBox;
	class BitmapCollisionBox;
	class ShapeCollisionBox;

	class ICollisionBox
	{
	public:
		virtual bool isCrashed(Transform _transform, const BitmapCollisionBox& _other, Transform _otherTransform)const = 0;
		virtual bool isCrashed(Transform _transform, const ShapeCollisionBox& _other, Transform _otherTransform)const = 0;
		virtual bool isCrashed(Transform _transform, const ICollisionBox& _other, Transform _otherTransform)const = 0;
	};

	using namespace suku::memory;
	class BitmapCollisionBox :public ICollisionBox
	{
	public:
		Array2D<bool> hitArea;
		int hitX, hitY;
		UINT hitWidth, hitHeight;
		BitmapCollisionBox(Array2D<bool> _hitArea, int _hitX, int _hitY, UINT _hitWidth, UINT _hitHeight) :hitArea(_hitArea), hitX(_hitX), hitY(_hitY), hitWidth(_hitWidth), hitHeight(_hitHeight) {}
		BitmapCollisionBox(Bitmap* _pBitmap, float _alphaThreshold = 0.0f);
		~BitmapCollisionBox();

		virtual bool isCrashed(Transform _transform, const BitmapCollisionBox& _other, Transform _otherTransform)const override;
		virtual bool isCrashed(Transform _transform, const ShapeCollisionBox& _other, Transform _otherTransform)const override;
		virtual bool isCrashed(Transform _transform, const ICollisionBox& _other, Transform _otherTransform)const override;
	};

	class ShapeCollisionBox :public ICollisionBox
	{
	public:
		Shape shape;
		ShapeCollisionBox(const Shape& _shape) :shape(_shape) {}
		//~ShapeCollisionBox();

		virtual bool isCrashed(Transform _transform, const BitmapCollisionBox& _other, Transform _otherTransform)const override;
		virtual bool isCrashed(Transform _transform, const ShapeCollisionBox& _other, Transform _otherTransform)const override;
		virtual bool isCrashed(Transform _transform, const ICollisionBox& _other, Transform _otherTransform)const override;
	};
}