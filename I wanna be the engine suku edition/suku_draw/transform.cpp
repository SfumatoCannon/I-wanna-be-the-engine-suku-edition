#include "transform.h"

namespace suku
{
	Transform::Transform()
	{
		matrix = D2D1::Matrix3x2F::Identity();
	}

	Transform::Transform(D2D1::Matrix3x2F _matrix)
	{
		matrix = _matrix;
	}

	void Transform::transformPoint(float* _pX, float* _pY)
	{
		auto transformedPoint = matrix.TransformPoint(D2D1::Point2F(*_pX, *_pY));
		*_pX = transformedPoint.x;
		*_pY = transformedPoint.y;
	}

	Vector Transform::transformPoint(float _x, float _y)
	{
		auto transformedPoint = matrix.TransformPoint(D2D1::Point2F(_x, _y));
		return { transformedPoint.x, transformedPoint.y };
	}

	Transform Transform::invertTransform()
	{
		D2D1::Matrix3x2F invertedMatrix = matrix;
		if (invertedMatrix.IsInvertible())
		{
			invertedMatrix.Invert();
			return Transform(invertedMatrix);
		}
		else
			return Transform();
	}

	Transform Transform::operator+(Transform _x)
	{
		//Transform resultTransform;
		//resultTransform.matrix.SetProduct(matrix, _x.matrix);
		//return resultTransform;
		return Transform(_x.matrix * matrix);
	}

	Transform Transform::operator*(Transform _x)
	{
		//Transform resultTransform;
		//resultTransform.matrix.SetProduct(matrix, _x.matrix);
		//return resultTransform;
		return Transform(matrix * _x.matrix);
	}

	void Transform::operator=(Transform _x)
	{
		matrix = _x.matrix;
	}

	Transform translation(float _shiftX, float _shiftY)
	{
		return Transform(D2D1::Matrix3x2F::Translation(_shiftX, _shiftY));
	}

	Transform rotation(float _rotateCenterX, float _rotateCenterY, float _angle)
	{
		return Transform(D2D1::Matrix3x2F::Rotation(_angle, D2D1::Point2F(_rotateCenterX, _rotateCenterY)));
	}

	Transform scale(float _centerX, float _centerY, float _scaleX, float _scaleY)
	{
		return Transform(D2D1::Matrix3x2F::Scale(
			D2D1::SizeF(_scaleX, _scaleY),
			D2D1::Point2F(_centerX, _centerY)
		));
	}

	Transform skew(float _centerX, float _centerY, float _angleX, float _angleY)
	{
		return Transform(D2D1::Matrix3x2F::Skew(
			_angleX, _angleY,
			D2D1::Point2F(_centerX, _centerY)
		));
	}
}