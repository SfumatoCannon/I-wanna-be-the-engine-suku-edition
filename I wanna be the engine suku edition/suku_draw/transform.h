#pragma once
#include <d2d1helper.h>

namespace suku
{
	class Vector;

	class Transform
	{
	public:
		const D2D1::Matrix3x2F& getMatrix() const { return matrix_;}

		Transform();
		Transform(D2D1::Matrix3x2F _matrix);
		static Transform Identity() { return Transform(); }

		void transformPoint(float* _x, float* _y);
		Vector transformPoint(float _x, float _y);
		Transform invertTransform();
		Vector getScale();

		Transform operator +(const Transform& _x)const;	//recommend using this
		Transform operator *(const Transform& _x)const;
		void operator =(Transform _x);
	private:
		D2D1::Matrix3x2F matrix_;
	};

	Transform translation(float _shiftX, float _shiftY);
	Transform rotation(float _rotateCenterX, float _rotateCenterY, float _angle);
	Transform scale(float _centerX, float _centerY, float _scaleX, float _scaleY);
	Transform skew(float _centerX, float _centerY, float _angleX, float _angleY);

	Transform linearInterpolate(Transform _from, Transform _to, float _t);
}