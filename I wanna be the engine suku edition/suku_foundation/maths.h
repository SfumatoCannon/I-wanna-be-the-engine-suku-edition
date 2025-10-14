#pragma once
#include <string>
#include <list>
#include <vector>
#include <stdarg.h>

#define COINCIDENT_LINES 1
#define PARALLEL_LINES 2
#define	INTERSECTING_LINES 3

#define PI 3.14159265358979323846
namespace suku
{
	double bRound(double _x);
	float bRound(float _x);

	long long randL();
	int randI();
	int randI(int _left, int _right);
	double randD(double _left, double _right);
	float randF(float _left, float _right);
	template<typename T> T randInList(T _a, T _b, ...);

	int gcd(int _a, int _b);
	int lcm(int _a, int _b);

	// or use fmod() in <cmath>
	double modR(double _a, double _b);	// a % b
	// or use fmod() in <cmath>
	float modR(float _a, float _b);	// a % b

	unsigned long long hash(std::string _str);

	double radToDeg(double _radian);
	double degToRad(double _degree);

	double maxD(std::list<double> _numlist);
	double minD(std::list<double> _numlist);

	double getDistance(double _x1, double _y1, double _x2, double _y2);

	void dotToLine(double _x1, double _y1, double _x2, double _y2, double* _k, double* _b);
	int getRelationOfLines(double _k1, double _b1, double _k2, double _b2);
	void getIntersectionOfLines(double _k1, double _b1, double _k2, double _b2,
		double* _x, double* _y);
	bool isSegmentsIntersect(double _x11, double _y11, double _x12, double _y12,
		double _x21, double _y21, double _x22, double _y22);

	void rectangularToPolar(double width, double height, double* angle, double* length);
	void polarToRectangular(double angle, double length, double* width, double* height);
	void rotateDot(double rotateCenterX, double rotateCenterY, double angle, double* x, double* y);

	bool isCrashedRect(double x1, double y1, double width1, double height1,
		double x2, double y2, double width2, double height2);
	double distanceOfRect(double _x1, double _y1, double _width1, double _height1,
		double _x2, double _y2, double _width2, double _height2);

	template<typename T>
	inline T randInList(T _a, T _b, ...)
	{
		std::vector<T> vec;
		vec.push_back(_a);
		vec.push_back(_b);
		va_list argList;
		va_start(argList, _b);
		T x;
		while (1)
		{
			x = va_arg(argList, T);
			if (x == NULL)
				break;
			else vec.push_back(x);
		}
		va_end(argList);
		int id = randI() % vec.size();
		return vec[id];
	}
}