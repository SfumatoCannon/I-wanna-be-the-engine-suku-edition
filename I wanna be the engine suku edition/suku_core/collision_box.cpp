#include "collision_box.h"

namespace suku
{
	BitmapCollisionBox::BitmapCollisionBox(bool** _hitArea, int _hitX, int _hitY, UINT _hitWidth, UINT _hitHeight)
	{
		hitArea = _hitArea;
		hitX = _hitX, hitY = _hitY;
		hitWidth = _hitWidth, hitHeight = _hitHeight;
	}

	BitmapCollisionBox::BitmapCollisionBox(Bitmap* _pBitmap, float _alphaThreshold)
	{
		if (_pBitmap == nullptr)
		{
			hitWidth = hitHeight = 0;
			hitX = hitY = 0;
			hitArea = nullptr;
			return;
		}
		hitWidth = _pBitmap->getWidth();
		hitHeight = _pBitmap->getHeight();
		hitX = hitY = 0;
		hitArea = new_memory_2d<bool>(hitWidth, hitHeight);
		getHitAreaFromBitmap(hitArea, *_pBitmap, _alphaThreshold);
	}

	BitmapCollisionBox::~BitmapCollisionBox()
	{
		delete_memory_2d(hitArea, hitWidth, hitHeight);
	}

	bool BitmapCollisionBox::isCrashed(Transform _transform, const BitmapCollisionBox& _other, Transform _otherTransform)const
	{
		if (hitArea == nullptr || _other.hitArea == nullptr)
			return false;
		UINT i, j;
		float px, py;
		Transform targetTransform = _transform + _otherTransform.invertTransform();
		for (i = 0; i < hitWidth; i++)
			for (j = 0; j < hitHeight; j++)
			{
				if (hitArea[i][j])
				{
					px = (float)(hitX + i);
					py = (float)(hitY + j);
					targetTransform.transformPoint(&px, &py);
					px -= (float)(_other.hitX);
					py -= (float)(_other.hitY);
					px = round(px);
					py = round(py);
					if (*(_other.hitArea + (int)py * _other.hitWidth + (int)px))
						return true;
				}
			}
		return false;
	}

	bool BitmapCollisionBox::isCrashed(Transform _transform, const ShapeCollisionBox& _other, Transform _otherTransform)const
	{
		if (hitArea == nullptr)
			return false;
		static Shape pixelShape = SquareShape(1, 0, 0);
		D2D1_GEOMETRY_RELATION result;
		UINT i, j;
		for (i = 0; i < hitWidth; i++)
			for (j = 0; j < hitHeight; j++)
			{
				if (hitArea[i][j])
				{
					pixelShape.currentGeometry->CompareWithGeometry(

						_other.shape.currentGeometry,
						((_transform + translation((float)i, (float)j)).invertTransform()
							+ _otherTransform).matrix,
						&result);
					if (result != D2D1_GEOMETRY_RELATION_DISJOINT)
						return true;
				}
			}
		return false;
	}

	bool BitmapCollisionBox::isCrashed(Transform _transform, const CollisionBox& _other, Transform _otherTransform)const
	{
		return _other.isCrashed(_otherTransform, *this, _transform);
	}

	ShapeCollisionBox::ShapeCollisionBox(const Shape& _shape)
	{
		shape = _shape;
	}

	bool ShapeCollisionBox::isCrashed(Transform _transform, const BitmapCollisionBox& _other, Transform _otherTransform)const
	{
		if (_other.hitArea == nullptr)
			return false;
		static Shape pixelShape = SquareShape(1, 0, 0);
		D2D1_GEOMETRY_RELATION result;
		UINT i, j;
		for (i = 0; i < _other.hitWidth; i++)
			for (j = 0; j < _other.hitHeight; j++)
			{
				if (_other.hitArea[i][j])
				{
					pixelShape.currentGeometry->CompareWithGeometry(
						shape.currentGeometry,
						((_otherTransform + translation((float)i, (float)j)).invertTransform()
							+ _transform).matrix,
						&result);
					if (result != D2D1_GEOMETRY_RELATION_DISJOINT)
						return true;
				}
			}
		return false;
	}

	bool ShapeCollisionBox::isCrashed(Transform _transform, const ShapeCollisionBox& _other, Transform _otherTransform)const
	{
		D2D1_GEOMETRY_RELATION result;
		Transform targetTransform = _otherTransform * _transform.invertTransform();
		shape.currentGeometry->CompareWithGeometry(
			_other.shape.currentGeometry,
			targetTransform.matrix,
			&result
		);
		if (result != D2D1_GEOMETRY_RELATION_DISJOINT)
			return true;
		else
			return false;
	}

	bool ShapeCollisionBox::isCrashed(Transform _transform, const CollisionBox& _other, Transform _otherTransform)const
	{
		return _other.isCrashed(_otherTransform, *this, _transform);
	}
}