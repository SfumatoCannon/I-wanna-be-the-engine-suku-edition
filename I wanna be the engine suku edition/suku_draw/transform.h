#pragma once
#include "../framework.h"
#include "../suku_foundation/includes.h"

namespace suku
{
	class Transform
	{
	public:
		D2D1::Matrix3x2F matrix;

		Transform();
		Transform(D2D1::Matrix3x2F _matrix);

		void transformPoint(float* _x, float* _y);
		Vector transformPoint(float _x, float _y);
		Transform invertTransform();

		Transform operator +(Transform _x);	//recommend using this
		Transform operator *(Transform _x);
		void operator =(Transform _x);
	};

	Transform translation(float _shiftX, float _shiftY);
	Transform rotation(float _rotateCenterX, float _rotateCenterY, float _angle);
	Transform scale(float _centerX, float _centerY, float _scaleX, float _scaleY);
	Transform skew(float _centerX, float _centerY, float _angleX, float _angleY);
}