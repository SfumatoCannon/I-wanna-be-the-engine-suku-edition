#pragma once
#include "../framework.h"
#include "../suku_foundation.h"
#include "transform.h"

namespace suku
{
	class Bitmap;
	class Color;

	class Shape
	{
	public:
		Transform transform;
		ID2D1TransformedGeometry* currentGeometry;
		ID2D1Geometry* originalGeometry;
		//ID2D1Brush* fillBrush;
		//ID2D1Brush* outlineBrush;
		//ID2D1StrokeStyle* outlineStrokeStyle;
		//float outlineWidth;

		//Shape& operator= (Shape& _x)const = delete;

		Shape();
		Shape(const Shape& _x);
		Shape(Shape&& _x)noexcept;
		Shape(ID2D1Geometry* _geometry, Transform _transform = Transform());
		//Shape(ID2D1Geometry* _geometry, ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush,
		//	float _edgeWidth = 1.0, ID2D1StrokeStyle* _outlineStrokeStyle = nullptr);
		void join();
		~Shape();

		void setOriginalGeometry(ID2D1Geometry* _geometry);
		void setTransform(Transform _transform);
		void setFill(Color _color);
		void setOutline(Color _color);

		void paint(float _x, float _y,
			ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush, float _outlineWidth = 1.0,
			ID2D1StrokeStyle* outlineStrokeStyle = nullptr);
		void paint(float _x, float _y, Transform _paintingTransform,
			ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush, float _outlineWidth = 1.0,
			ID2D1StrokeStyle* outlineStrokeStyle = nullptr);
		void paint(Transform _paintingTransform,
			ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush, float _outlineWidth = 1.0,
			ID2D1StrokeStyle* outlineStrokeStyle = nullptr);
		Bitmap* paintOnBitmap(const Bitmap& _bitmap, float _x, float _y,
			ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush, float _outlineWidth = 1.0,
			ID2D1StrokeStyle* outlineStrokeStyle = nullptr);
		void paint(float _x, float _y);
		void paint(float _x, float _y, Transform _paintingTransform);
		void paint(Transform _paintingTransform);


		bool isCrashed(Shape& _x);

		Shape& operator= (const Shape& _x);
		Shape& operator= (Shape&& _x)noexcept;
		Shape operator-(const Shape& _x);
		Shape operator&(const Shape& _x);
		Shape operator|(const Shape& _x);
		Shape operator^(const Shape& _x);

		bool operator==(const Shape& other) const = default;

	private:
		ID2D1Brush* fillBrush_ = nullptr;
		ID2D1Brush* outlineBrush_ = nullptr;
		float outlineWidth_ = 1.0f;
		ID2D1StrokeStyle* outlineStrokeStyle_ = nullptr;
	};

	class SquareShape :public Shape
	{
	public:
		const float length, startX, startY;
		SquareShape(float _length, float _startX = 0, float _startY = 0, Transform _transform = Transform());
	};

	class RectangleShape :public Shape
	{
	public:
		const float width, height, startX, startY;
		RectangleShape(float _width, float _height, float _startX = 0, float _startY = 0, Transform _transform = Transform());
	};

	class CircleShape :public Shape
	{
	public:
		const float radius, startX, startY;
		CircleShape(float _radius, float _startX = 0, float _startY = 0, Transform _transform = Transform());
	};

	class EllipseShape :public Shape
	{
	public:
		const float radiusX, radiusY, startX, startY;
		EllipseShape(float _radiusX, float _radiusY, float _startX = 0, float _startY = 0, Transform _transform = Transform());
	};

	ID2D1Brush* createSolidColorBrush(const Color _color);
}