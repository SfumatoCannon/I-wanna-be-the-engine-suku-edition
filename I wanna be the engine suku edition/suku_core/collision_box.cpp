#include "collision_box.h"

using namespace suku::memory;
namespace suku
{

	BitmapCollisionBox::BitmapCollisionBox(Bitmap* _pBitmap, float _alphaThreshold)
	{
		if (_pBitmap == nullptr)
		{
			hitWidth = hitHeight = 0;
			hitX = hitY = 0;
			return;
		}
		hitWidth = _pBitmap->getWidth();
		hitHeight = _pBitmap->getHeight();
		hitX = hitY = 0;
		hitArea = Array2D<bool>(hitWidth, hitHeight);
		getHitAreaFromBitmap(hitArea, *_pBitmap, _alphaThreshold);
	}

	BitmapCollisionBox::~BitmapCollisionBox()
	{
	}

	bool BitmapCollisionBox::isCrashed(Transform _transform, const BitmapCollisionBox& _other, Transform _otherTransform)const
	{
		if (hitArea.isEmpty() || _other.hitArea.isEmpty())
			return false;
		UINT i, j;
		float px, py;
		Transform targetTransform = _transform + _otherTransform.invertTransform();
		for (i = 0; i < hitWidth; i++)
			for (j = 0; j < hitHeight; j++)
			{
				if (hitArea(i, j))
				{
					px = (float)(hitX + i);
					py = (float)(hitY + j);
					targetTransform.transformPoint(&px, &py);
					px -= (float)(_other.hitX);
					py -= (float)(_other.hitY);
					px = round(px);
					py = round(py);
					if (_other.hitArea((int)px, (int)py))
						return true;
				}
			}
		return false;
	}

	bool BitmapCollisionBox::isCrashed(Transform _transform, const ShapeCollisionBox& _other, Transform _otherTransform)const
	{
		if (hitArea.isEmpty())
			return false;
		static Shape pixelShape = SquareShape(1, 0, 0);
		D2D1_GEOMETRY_RELATION result;
		UINT i, j;
		for (i = 0; i < hitWidth; i++)
			for (j = 0; j < hitHeight; j++)
			{
				if (hitArea(i, j))
				{
					// compute a local transform so we can take address of matrix
					Transform t = (_transform + translation((float)i, (float)j)).invertTransform() + _otherTransform;
					pixelShape.currentGeometry->CompareWithGeometry(

						_other.shape.currentGeometry.Get(),
						&t.getMatrix(),
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

	bool ShapeCollisionBox::isCrashed(Transform _transform, const BitmapCollisionBox& _other, Transform _otherTransform)const
	{
		if (_other.hitArea.isEmpty())
			return false;
		static Shape pixelShape = SquareShape(1, 0, 0);
		D2D1_GEOMETRY_RELATION result;
		UINT i, j;
		for (i = 0; i < _other.hitWidth; i++)
			for (j = 0; j < _other.hitHeight; j++)
			{
				if (_other.hitArea(i, j))
				{
					Transform t = (_otherTransform + translation((float)i, (float)j)).invertTransform() + _transform;
					pixelShape.currentGeometry->CompareWithGeometry(
						shape.currentGeometry.Get(),
						&t.getMatrix(),
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
		Transform targetTransform = _transform.invertTransform() + _otherTransform;
		const auto& matrix = targetTransform.getMatrix();
		constexpr float areaThreshold = 0.5f; // Minimum area threshold for collision detection

		HRESULT hr = shape.currentGeometry->CompareWithGeometry(
			_other.shape.currentGeometry.Get(),
			&matrix,
			&result
		);
		if (FAILED(hr))
		{
			WARNINGWINDOW("Failed to compare geometries for collision detection.");
			return false;
		}
		if (result == D2D1_GEOMETRY_RELATION_DISJOINT)
		{
			return false;
		}

		ID2D1Factory1* factory = graphics::D2DFactoryGlobal::getD2DFactory();

		Microsoft::WRL::ComPtr<ID2D1PathGeometry> intersection;
		hr = factory->CreatePathGeometry(intersection.GetAddressOf());
		if (FAILED(hr))
		{
			WARNINGWINDOW("Failed to create path geometry for collision detection.");
			return false;
		}

		Microsoft::WRL::ComPtr<ID2D1GeometrySink> sink;
		hr = intersection->Open(sink.GetAddressOf());
		if (FAILED(hr))
		{
			WARNINGWINDOW("Failed to open geometry sink for collision detection.");
			return false;
		}

		hr = shape.currentGeometry->CombineWithGeometry(
			_other.shape.currentGeometry.Get(),
			D2D1_COMBINE_MODE_INTERSECT,
			&matrix,
			D2D1_DEFAULT_FLATTENING_TOLERANCE,
			sink.Get()
		);
		if (FAILED(hr))
		{
			return false;
		}

		hr = sink->Close();
		if (FAILED(hr))
		{
			WARNINGWINDOW("Failed to close geometry sink for collision detection.");
			return false;
		}

		FLOAT area = 0.0f;
		hr = intersection->ComputeArea(
			nullptr,
			D2D1_DEFAULT_FLATTENING_TOLERANCE,
			&area
		);
		if (FAILED(hr))
		{
			return false;
		}

		return area >= areaThreshold;
	}

	bool ShapeCollisionBox::isCrashed(Transform _transform, const CollisionBox& _other, Transform _otherTransform)const
	{
		return _other.isCrashed(_otherTransform, *this, _transform);
	}
}