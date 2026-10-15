#pragma once
#include <d2d1.h>
#include <wrl/client.h>
#include "transform.h"
#include <memory>
#include "brush.h"

namespace suku
{
    using Microsoft::WRL::ComPtr;
	class Bitmap;
	class Color;

	class Shape
	{
	public:
		Transform transform;
		ComPtr<ID2D1TransformedGeometry> currentGeometry;
		ComPtr<ID2D1Geometry> originalGeometry;

		Shape();
		Shape(const Shape& _x);
		Shape(Shape&& _x)noexcept;
		Shape(const ComPtr<ID2D1Geometry>& _geometry, Transform _transform = Transform());
		~Shape();

		void setOriginalGeometry(const ComPtr<ID2D1Geometry>& _geometry);
		void setTransform(Transform _transform);
		void setFill(const Brush& _brush);
		void setOutline(const Brush& _brush);
		void setOpacity(float _opacity);
		void setFillOpacity(float _opacity);
		void setOutlineOpacity(float _opacity);

		void paintFill(float _x, float _y, const Brush& _fillBrush)const;
		void paintFill(float _x, float _y, Transform _paintingTransform, const Brush& _fillBrush)const;
		void paintFill(Transform _paintingTransform, const Brush& _fillBrush)const;
		void paintOutline(float  _x, float _y, const Brush& _outlineBrush, float _outlineWidth = 1.0, const ComPtr<ID2D1StrokeStyle>& outlineStrokeStyle = nullptr)const;
		void paintOutline(float  _x, float _y, Transform _paintingTransform, const Brush& _outlineBrush, float _outlineWidth = 1.0, const ComPtr<ID2D1StrokeStyle>& outlineStrokeStyle = nullptr)const;
		void paintOutline(Transform _paintingTransform, const Brush& _outlineBrush, float _outlineWidth = 1.0, const ComPtr<ID2D1StrokeStyle>& outlineStrokeStyle = nullptr)const;

		void paint(float _x, float _y,
			const Brush& _fillBrush, const Brush& _outlineBrush, float _outlineWidth = 1.0,
			const ComPtr<ID2D1StrokeStyle>& outlineStrokeStyle = nullptr)const;
		void paint(float _x, float _y, Transform _paintingTransform,
			const Brush& _fillBrush, const Brush& _outlineBrush, float _outlineWidth = 1.0,
			const ComPtr<ID2D1StrokeStyle>& outlineStrokeStyle = nullptr)const;
		void paint(Transform _paintingTransform,
			const Brush& _fillBrush, const Brush& _outlineBrush, float _outlineWidth = 1.0,
			const ComPtr<ID2D1StrokeStyle>& outlineStrokeStyle = nullptr)const;

		void paint(float _x, float _y)const;
		void paint(float _x, float _y, Transform _paintingTransform)const;
		void paint(Transform _paintingTransform)const;

		bool isCrashed(Shape& _x);

		Shape& operator= (const Shape& _x);
		Shape& operator= (Shape&& _x)noexcept;
		Shape operator-(const Shape& _x);
		Shape operator&(const Shape& _x);
		Shape operator|(const Shape& _x);
		Shape operator^(const Shape& _x);

		bool operator==(const Shape& other) const = default;

	private:
		Brush fillBrush_;
		Brush outlineBrush_;
		float outlineWidth_ = 1.0f;
		ComPtr<ID2D1StrokeStyle> pOutlineStrokeStyle_ = nullptr;
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

	class RoundedRectangleShape :public Shape
	{
	public:
		const float width, height, radiusX, radiusY, startX, startY;
		RoundedRectangleShape(float _width, float _height, float _radiusX, float _radiusY,
			float _startX = 0, float _startY = 0, Transform _transform = Transform());
	};
}