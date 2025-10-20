#include "maths.h"
#include <cmath>
#include <ctime>
#include <windows.h>

namespace suku::maths
{
	double bRound(double _x)
	{
		int iX = (int)_x;
		double t = _x - iX;
		if (t == 0.5)
		{
			if (iX & 1)
				return iX + 1;
			else
				return iX;
		}
		else if (t > 0.5)
			return iX + 1;
		else
			return iX;
	}

	float bRound(float _x)
	{
		int iX = (int)_x;
		float t = _x - iX;
		if (t == 0.5f)
		{
			if (iX & 1)
				return iX + 1.0f;
			else
				return (float)iX;
		}
		else if (t > 0.5f)
			return iX + 1.0f;
		else
			return (float)iX;
	}


	long long randL()
	{
		static long long x = (long long)(time(0));
		static long long clock = 0;
		const long long a = 1103515245, c = 12345, m = 1ll << 31;
		x = (x * a + c) % m;
		clock += x;
		return (int)(x + clock);
	}

	int randI()
	{
		static long long x = (long long)(time(0));
		static long long clock = 0;
		const long long a = 1103515245, c = 12345, m = 1ll << 31;
		x = (x * a + c) % m;
		clock += x;
		return (int)(x + clock);
	}

	int randI(int _left, int _right)
	{
		return (int)randD((double)_left, (double)_right);
	}

	double randD(double _left, double _right)
	{
		return (double)abs(randI()) / INT_MAX * (_right - _left) + _left;
	}

	float randF(float _left, float _right)
	{
		return (float)abs(randI()) / INT_MAX * (_right - _left) + _left;
	}

	int gcd(int _a, int _b)
	{
		if (_a % _b == 0)
			return _b;
		else return gcd(_b, _a % _b);
	}

	int lcm(int _a, int _b)
	{
		return _a * _b / gcd(_a, _b);
	}

	double modR(double _a, double _b)
	{
		return _a - (int)(_a / _b) * _b;
	}

	float modR(float _a, float _b)
	{
		return _a - (int)(_a / _b) * _b;
	}

	unsigned long long hash(std::string _str)
	{
		size_t length = _str.length();
		unsigned long long res = 0llu;
		for (size_t i = 0; i < length; i++)
			res = res * 131 + (unsigned long long)_str[i];
		return res;
	}

	double radToDeg(double _radian)
	{
		return _radian * 180.0 / PI;
	}

	double degToRad(double _degree)
	{
		return _degree * PI / 180.0;
	}

	double maxD(std::list<double> _numlist)
	{
		double result = *(_numlist.begin());
		for (auto i : _numlist)
			if (i > result)
				result = i;
		return result;
	}

	double minD(std::list<double> _numlist)
	{
		double result = *(_numlist.begin());
		for (auto i : _numlist)
			if (i < result)
				result = i;
		return result;
	}

	double getDistance(double _x1, double _y1, double _x2, double _y2)
	{
		return sqrt((_x1 - _x2) * (_x1 - _x2) + (_y1 - _y2) * (_y1 - _y2));
	}

	void dotToLine(double _x1, double _y1, double _x2, double _y2, double* _k, double* _b)
	{
		if (_x1 == _x2) return;
		*_k = (_y1 - _y2) / (_x1 - _x2);
		*_b = _y1 - (*_k) * _x1;
	}

	int getRelationOfLines(double _k1, double _b1, double _k2, double _b2)
	{
		if (_k1 == _k2)
		{
			if (_b1 == _b2)
				return COINCIDENT_LINES;
			else return PARALLEL_LINES;
		}
		else return INTERSECTING_LINES;
	}

	void getIntersectionOfLines(double _k1, double _b1, double _k2, double _b2, double* _x, double* _y)
	{
		if (_k1 == _k2) return;
		*_x = (_b2 - _b1) / (_k1 - _k2);
		*_y = _k1 * (*_x) + _b1;
	}

	bool isSegmentsIntersect(double _x11, double _y11, double _x12, double _y12, double _x21, double _y21, double _x22, double _y22)
	{
		double k1, b1, k2, b2;
		double intersectionX, intersectionY;
		if (_x11 == _x12 && _x21 == _x22)
		{
			std::swap(_x11, _y11);
			std::swap(_x12, _y12);
			std::swap(_x21, _y21);
			std::swap(_x22, _y22);
		}
		else if (_x11 == _x12)
		{
			dotToLine(_x21, _y21, _x22, _y22, &k2, &b2);
			intersectionY = k2 * _x11 + b2;
			if (_x21 > _x22)
				std::swap(_x21, _x22);
			if (_y11 > _y12)
				std::swap(_y21, _y22);
			return _x21 <= _x11 && _x11 <= _x22 && _y11 <= intersectionY && intersectionY <= _y12;
		}
		else if (_x21 == _x22)
		{
			dotToLine(_x11, _y11, _x12, _y12, &k1, &b1);
			intersectionY = k1 * _x21 + b1;
			if (_x11 > _x12)
				std::swap(_x11, _x12);
			if (_y21 > _y22)
				std::swap(_y21, _y22);
			return _x11 <= _x21 && _x21 <= _x12 && _y21 <= intersectionY && intersectionY <= _y22;
		}
		dotToLine(_x11, _y11, _x12, _y12, &k1, &b1);
		dotToLine(_x21, _y21, _x22, _y22, &k2, &b2);
		if (k1 == k2)
		{
			if (b1 != b2)
				return false;
			else
			{
				if (_x11 > _x12)
					std::swap(_x11, _x12);
				if (_x21 > _x22)
					std::swap(_x21, _x22);
				if (_x11 < _x21)
					return _x21 < _x12;
				else return _x11 < _x22;
			}
		}
		else
		{
			getIntersectionOfLines(k1, b1, k2, b2, &intersectionX, &intersectionY);
			if (_x11 > _x12)
				std::swap(_x11, _x12);
			if (_x21 > _x22)
				std::swap(_x21, _x22);
			return _x11 < intersectionX && intersectionX < _x12
				&& _x21 < intersectionX && intersectionX < _x22;
		}
	}

	void rectangularToPolar(double _width, double _height, double* _angle, double* _length)
	{
		*_length = sqrt(_width * _width + _height * _height);
		if ((*_length) == 0)
			*_angle = 0;
		else if (_width >= 0.0)
			*_angle = radToDeg(asin(_height / (*_length)));
		else if (_height >= 0.0)
			*_angle = radToDeg(acos(_width / (*_length)));
		else *_angle = radToDeg(PI - asin(_height / (*_length)));
	}

	void polarToRectangular(double _angle, double _length, double* _width, double* _height)
	{
		*_width = cos(degToRad(modR(_angle, 360.0))) * _length;
		*_height = sin(degToRad(modR(_angle, 360.0))) * _length;
	}

	void rotateDot(double _rotateCenterX, double _rotateCenterY, double _angle, double* _x, double* _y)
	{
		double x1 = *_x, y1 = *_y;
		double dAngle = 0.0, dLength = 0.0;
		rectangularToPolar(x1 - _rotateCenterX, y1 - _rotateCenterY, &dAngle, &dLength);
		dAngle += _angle;
		polarToRectangular(dAngle, dLength, &x1, &y1);
		*_x = x1 + _rotateCenterX;
		*_y = y1 + _rotateCenterY;
	}

	bool isCrashedRect(double _x1, double _y1, double _width1, double _height1, double _x2, double _y2, double _width2, double _height2)
	{
		return _x1 < _x2 + _width2 && _x2 < _x1 + _width1 && _y1 < _y2 + _height2 && _y2 < _y1 + _height1;
	}

	double distanceOfRect(double _x1, double _y1, double _width1, double _height1, double _x2, double _y2, double _width2, double _height2)
	{
		if (isCrashedRect(_x1, _y1, _width1, _height1, _x2, _y2, _width2, _height2))
			return 0.0;
		else if ((_x1 <= _x2 && _x1 + _width1 - 1 >= _x2) || (_x2 <= _x1 && _x2 + _width2 - 1 >= _x1))
			return min(fabs(_y1 - (_y2 + _height2)), fabs(_y2 - (_y1 + _height1)));
		else if ((_y1 <= _y2 && _y1 + _height1 - 1 >= _y2) || (_y2 <= _y1 && _y2 + _height2 - 1 >= _y1))
			return min(fabs(_x1 - (_x2 + _width2)), fabs(_x2 - (_x1 + _width1)));
		else return min(min(getDistance(_x1, _y1, _x2 + _width2, _y2 + _height2),
			getDistance(_x1 + _width1, _y1 + _height1, _x2, _y2)),
			min(getDistance(_x1, _y1 + _height1, _x2 + _width2, _y2),
				getDistance(_x1 + _width1, _y1, _x2, _y2 + _height2)));
	}
}