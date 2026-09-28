#include "pch.h"
#include "transform.h"
#include <suku_foundation/maths.h>

namespace suku
{
	Transform::Transform()
	{
		matrix_ = D2D1::Matrix3x2F::Identity();
	}

	Transform::Transform(D2D1::Matrix3x2F _matrix)
	{
		matrix_ = _matrix;
	}

	void Transform::transformPoint(float* _pX, float* _pY)
	{
		auto transformedPoint = matrix_.TransformPoint(D2D1::Point2F(*_pX, *_pY));
		*_pX = transformedPoint.x;
		*_pY = transformedPoint.y;
	}

	Vector Transform::transformPoint(float _x, float _y)
	{
		auto transformedPoint = matrix_.TransformPoint(D2D1::Point2F(_x, _y));
		return { transformedPoint.x, transformedPoint.y };
	}

	Transform Transform::invertTransform()
	{
		D2D1::Matrix3x2F invertedMatrix = matrix_;
		if (invertedMatrix.IsInvertible())
		{
			invertedMatrix.Invert();
			return Transform(invertedMatrix);
		}
		else
			return Transform();
	}

	Transform Transform::operator+(const Transform& _x)const
	{
		//Transform resultTransform;
		//resultTransform.matrix_.SetProduct(matrix_, _x.matrix_);
		//return resultTransform;
		return Transform(_x.matrix_ * matrix_);
	}

	Transform Transform::operator*(const Transform& _x)const
	{
		//Transform resultTransform;
		//resultTransform.matrix_.SetProduct(matrix_, _x.matrix_);
		//return resultTransform;
		return Transform(matrix_ * _x.matrix_);
	}

	void Transform::operator=(Transform _x)
	{
		matrix_ = _x.matrix_;
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

	Transform linearInterpolate(Transform _from, Transform _to, float _t)
	{
		D2D1::Matrix3x2F fromMatrix = _from.getMatrix();
		D2D1::Matrix3x2F toMatrix = _to.getMatrix();
		return Transform(D2D1::Matrix3x2F(
			fromMatrix._11 + (toMatrix._11 - fromMatrix._11) * _t,
			fromMatrix._12 + (toMatrix._12 - fromMatrix._12) * _t,
			fromMatrix._21 + (toMatrix._21 - fromMatrix._21) * _t,
			fromMatrix._22 + (toMatrix._22 - fromMatrix._22) * _t,
			fromMatrix._31 + (toMatrix._31 - fromMatrix._31) * _t,
			fromMatrix._32 + (toMatrix._32 - fromMatrix._32) * _t
		));
	}
}