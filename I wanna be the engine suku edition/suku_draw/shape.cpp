#include "pch.h"
#include "shape.h"
#include "bitmap.h"
#include "color.h"
#include "draw_core.h"
#include "suku_foundation/message.h"
#include <memory>
#include "brush.h"

namespace suku
{
	using Microsoft::WRL::ComPtr;
	using namespace suku::graphics;

	Shape::Shape()
	{
		originalGeometry = nullptr;
		transform = Transform();
		currentGeometry = nullptr;
	}

	Shape::Shape(const Shape& _x)
	{
		transform = _x.transform;
		if (_x.originalGeometry)
		{
			originalGeometry = _x.originalGeometry;
			if (_x.currentGeometry)
			{
				currentGeometry = _x.currentGeometry;
			}
			else
			{
				auto pD2DFactory = D2DFactoryGlobal::getD2DFactory();
				pD2DFactory->CreateTransformedGeometry(originalGeometry.Get(), transform.getMatrix(), currentGeometry.GetAddressOf());
			}
		}
		else
		{
			originalGeometry = nullptr;
			currentGeometry = nullptr;
		}
	}

	Shape::Shape(Shape&& _x) noexcept
	{
		transform = _x.transform;
		originalGeometry = std::move(_x.originalGeometry);
		currentGeometry = std::move(_x.currentGeometry);
		_x.originalGeometry.Reset();
		_x.currentGeometry.Reset();
	}

	Shape::Shape(const ComPtr<ID2D1Geometry>& _geometry, Transform _transform)
	{
		originalGeometry = nullptr;
		transform = Transform();
		currentGeometry = nullptr;
		setOriginalGeometry(_geometry);
	}

	Shape::~Shape()
	{
		originalGeometry.Reset();
		currentGeometry.Reset();
	}

	void Shape::setOriginalGeometry(const ComPtr<ID2D1Geometry>& _geometry)
	{
		originalGeometry.Reset();
		currentGeometry.Reset();
		if (_geometry)
		{
			originalGeometry = _geometry;
			auto pD2DFactory = D2DFactoryGlobal::getD2DFactory();
			pD2DFactory->CreateTransformedGeometry(originalGeometry.Get(), transform.getMatrix(), currentGeometry.GetAddressOf());
		}
		else
			originalGeometry = currentGeometry = nullptr;
	}

	void Shape::setTransform(Transform _transform)
	{
		transform = _transform;
		currentGeometry.Reset();
		auto pD2DFactory = D2DFactoryGlobal::getD2DFactory();
		pD2DFactory->CreateTransformedGeometry(originalGeometry.Get(), transform.getMatrix(), currentGeometry.GetAddressOf());
	}

	void Shape::setFill(const Brush& _brush)
	{
		fillBrush_ = _brush;
	}

	void Shape::setOutline(const Brush& _brush)
	{
		outlineBrush_ = _brush;
	}

	void Shape::setOpacity(float _opacity)
	{
		if (fillBrush_.isValid())
			fillBrush_.getD2DBrush()->SetOpacity(_opacity);
		if (outlineBrush_.isValid())
			outlineBrush_.getD2DBrush()->SetOpacity(_opacity);
	}

	void Shape::setFillOpacity(float _opacity)
	{
		if (fillBrush_.isValid())
			fillBrush_.getD2DBrush()->SetOpacity(_opacity);
	}

	void Shape::setOutlineOpacity(float _opacity)
	{
		if (outlineBrush_.isValid())
			outlineBrush_.getD2DBrush()->SetOpacity(_opacity);
	}

	void Shape::paintFill(float _x, float _y, const Brush& _fillBrush)const
	{
		if (currentGeometry != nullptr)
		{
			setPaintingTransform(translation(_x, _y));
			if (_fillBrush.isValid())
				drawGeometryFill(currentGeometry.Get(), _fillBrush.getD2DBrush().Get());
		}
	}

	void Shape::paintFill(float _x, float _y, Transform _paintingTransform, const Brush& _fillBrush)const
	{
		if (currentGeometry != nullptr)
		{
			setPaintingTransform(translation(_x, _y) + _paintingTransform);
			if (_fillBrush.isValid())
				drawGeometryFill(currentGeometry.Get(), _fillBrush.getD2DBrush().Get());
		}
	}

	void Shape::paintFill(Transform _paintingTransform, const Brush& _fillBrush)const
	{
		if (currentGeometry != nullptr)
		{
			setPaintingTransform(_paintingTransform);
			if (_fillBrush.isValid())
				drawGeometryFill(currentGeometry.Get(), _fillBrush.getD2DBrush().Get());
		}
	}

	void Shape::paintOutline(float _x, float _y, 
		const Brush& _outlineBrush, float _outlineWidth, 
		const ComPtr<ID2D1StrokeStyle>& outlineStrokeStyle)const
	{
		if (currentGeometry != nullptr)
		{
			setPaintingTransform(translation(_x, _y));
			if (_outlineBrush.isValid())
				drawGeometryOutline(currentGeometry.Get(), _outlineBrush.getD2DBrush().Get(), _outlineWidth, outlineStrokeStyle.Get());
		}
	}

	void Shape::paintOutline(float _x, float _y, Transform _paintingTransform, 
		const Brush& _outlineBrush, float _outlineWidth, 
		const ComPtr<ID2D1StrokeStyle>& outlineStrokeStyle)const
	{
		if (currentGeometry != nullptr)
		{
			setPaintingTransform(translation(_x, _y) + _paintingTransform);
			if (_outlineBrush.isValid())
				drawGeometryOutline(currentGeometry.Get(), _outlineBrush.getD2DBrush().Get(), _outlineWidth, outlineStrokeStyle.Get());
		}
	}

	void Shape::paintOutline(Transform _paintingTransform, 
		const Brush& _outlineBrush, float _outlineWidth, 
		const ComPtr<ID2D1StrokeStyle>& outlineStrokeStyle)const
	{
		if (currentGeometry != nullptr)
		{
			setPaintingTransform(_paintingTransform);
			if (_outlineBrush.isValid())
				drawGeometryOutline(currentGeometry.Get(), _outlineBrush.getD2DBrush().Get(), _outlineWidth, outlineStrokeStyle.Get());
		}
	}

	void Shape::paint(float _x, float _y, 
		const Brush& _fillBrush, const Brush& _outlineBrush, float _outlineWidth,
		const ComPtr<ID2D1StrokeStyle>& outlineStrokeStyle)const
	{
		if (currentGeometry != nullptr)
		{
			setPaintingTransform(translation(_x, _y));
			if (_outlineBrush.isValid())
				drawGeometryOutline(currentGeometry.Get(), _outlineBrush.getD2DBrush().Get(), _outlineWidth, outlineStrokeStyle.Get());
			if (_fillBrush.isValid())
				drawGeometryFill(currentGeometry.Get(), _fillBrush.getD2DBrush().Get());
		}
	}

	void Shape::paint(float _x, float _y, Transform _paintingTransform, 
		const Brush& _fillBrush, const Brush& _outlineBrush, float _outlineWidth, 
		const ComPtr<ID2D1StrokeStyle>& outlineStrokeStyle)const
	{
		if (currentGeometry != nullptr)
		{
			setPaintingTransform(translation(_x, _y) + _paintingTransform);
			if (_outlineBrush.isValid())
				drawGeometryOutline(currentGeometry.Get(), _outlineBrush.getD2DBrush().Get(), _outlineWidth, outlineStrokeStyle.Get());
			if (_fillBrush.isValid())
				drawGeometryFill(currentGeometry.Get(), _fillBrush.getD2DBrush().Get());
		}
	}

	void Shape::paint(Transform _paintingTransform, 
		const Brush& _fillBrush, const Brush& _outlineBrush, float _outlineWidth, 
		const ComPtr<ID2D1StrokeStyle>& outlineStrokeStyle)const
	{
		if (currentGeometry != nullptr)
		{
			setPaintingTransform(_paintingTransform);
			if (_outlineBrush.isValid())
				drawGeometryOutline(currentGeometry.Get(), _outlineBrush.getD2DBrush().Get(), _outlineWidth, outlineStrokeStyle.Get());
			if (_fillBrush.isValid())
				drawGeometryFill(currentGeometry.Get(), _fillBrush.getD2DBrush().Get());
		}
	}

	void Shape::paint(float _x, float _y)const
	{
		paint(_x, _y, fillBrush_, outlineBrush_, outlineWidth_, pOutlineStrokeStyle_);
	}

	void Shape::paint(float _x, float _y, Transform _paintingTransform)const
	{
		paint(_x, _y, _paintingTransform, fillBrush_, outlineBrush_, outlineWidth_, pOutlineStrokeStyle_);
	}

	void Shape::paint(Transform _paintingTransform)const
	{
		paint(_paintingTransform, fillBrush_, outlineBrush_, outlineWidth_, pOutlineStrokeStyle_);
	}

	bool Shape::isCrashed(Shape& _x)
	{
		D2D1_GEOMETRY_RELATION result;
		currentGeometry->CompareWithGeometry(_x.currentGeometry.Get(), nullptr, &result);
		if (result == D2D1_GEOMETRY_RELATION_DISJOINT)
			return false;
		else return true;
	}

	Shape& Shape::operator=(const Shape& _x)
	{
		transform = _x.transform;
		originalGeometry.Reset();
		currentGeometry.Reset();
		if (_x.originalGeometry)
		{
			originalGeometry = _x.originalGeometry;
			if (_x.currentGeometry)
			{
				currentGeometry = _x.currentGeometry;
			}
			else
			{
				auto pD2DFactory = D2DFactoryGlobal::getD2DFactory();
				pD2DFactory->CreateTransformedGeometry(originalGeometry.Get(), transform.getMatrix(), currentGeometry.GetAddressOf());
			}
		}
		else
		{
			originalGeometry = nullptr;
			currentGeometry = nullptr;
		}
		return (*this);
	}

	Shape& Shape::operator=(Shape&& _x) noexcept
	{
		transform = _x.transform;
		originalGeometry.Reset();
		currentGeometry.Reset();
		originalGeometry = std::move(_x.originalGeometry);
		currentGeometry = std::move(_x.currentGeometry);
		_x.originalGeometry.Reset();
		_x.currentGeometry.Reset();
		return (*this);
	}

	Shape Shape::operator-(const Shape& _x)
	{
		ComPtr<ID2D1GeometrySink> resGeometrySink = nullptr;
		ComPtr<ID2D1PathGeometry> resGeometry;
		HRESULT hr;
		auto pD2DFactory = D2DFactoryGlobal::getD2DFactory();
		pD2DFactory->CreatePathGeometry(resGeometry.GetAddressOf());
		hr = resGeometry->Open(resGeometrySink.GetAddressOf());
		if (SUCCEEDED(hr))
		{
			Transform inv = transform.invertTransform();
			D2D1::Matrix3x2F m1 = inv.getMatrix();
			D2D1::Matrix3x2F m2 = _x.transform.getMatrix();
			D2D1::Matrix3x2F combineMatrix = m1 * m2;
			hr = currentGeometry->CombineWithGeometry(
				_x.currentGeometry.Get(),
				D2D1_COMBINE_MODE_EXCLUDE,
				combineMatrix,
				NULL,
				resGeometrySink.Get()
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = resGeometrySink->Close();
		}
		if (resGeometrySink) resGeometrySink.Reset();

		return Shape(resGeometry.Get());
	}

	Shape Shape::operator&(const Shape& _x)
	{
		ComPtr<ID2D1GeometrySink> resGeometrySink = nullptr;
		ComPtr<ID2D1PathGeometry> resGeometry;
		HRESULT hr;
		auto pD2DFactory = D2DFactoryGlobal::getD2DFactory();
		pD2DFactory->CreatePathGeometry(resGeometry.GetAddressOf());
		hr = resGeometry->Open(resGeometrySink.GetAddressOf());
		if (SUCCEEDED(hr))
		{
			D2D1::Matrix3x2F m1 = transform.invertTransform().getMatrix();
			D2D1::Matrix3x2F m2 = _x.transform.getMatrix();
			D2D1::Matrix3x2F combineMatrix = m1 * m2;
			hr = currentGeometry->CombineWithGeometry(
				_x.currentGeometry.Get(),
				D2D1_COMBINE_MODE_INTERSECT,
				combineMatrix,
				NULL,
				resGeometrySink.Get()
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = resGeometrySink->Close();
		}
		if (resGeometrySink) resGeometrySink.Reset();

		return Shape(resGeometry.Get());
	}

	Shape Shape::operator|(const Shape& _x)
	{
		ComPtr<ID2D1GeometrySink> resGeometrySink = nullptr;
		ComPtr<ID2D1PathGeometry> resGeometry;
		HRESULT hr;
		auto pD2DFactory = D2DFactoryGlobal::getD2DFactory();
		pD2DFactory->CreatePathGeometry(resGeometry.GetAddressOf());
		hr = resGeometry->Open(resGeometrySink.GetAddressOf());
		if (SUCCEEDED(hr))
		{
			D2D1::Matrix3x2F m1 = transform.invertTransform().getMatrix();
			D2D1::Matrix3x2F m2 = _x.transform.getMatrix();
			D2D1::Matrix3x2F combineMatrix = m1 * m2;
			hr = currentGeometry->CombineWithGeometry(
				_x.currentGeometry.Get(),
				D2D1_COMBINE_MODE_UNION,
				combineMatrix,
				NULL,
				resGeometrySink.Get()
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = resGeometrySink->Close();
		}
		if (resGeometrySink) resGeometrySink.Reset();

		return Shape(resGeometry.Get());
	}

	Shape Shape::operator^(const Shape& _x)
	{
		ComPtr<ID2D1GeometrySink> resGeometrySink = nullptr;
		ComPtr<ID2D1PathGeometry> resGeometry;
		HRESULT hr;
		auto pD2DFactory = D2DFactoryGlobal::getD2DFactory();
		pD2DFactory->CreatePathGeometry(resGeometry.GetAddressOf());
		hr = resGeometry->Open(resGeometrySink.GetAddressOf());
		if (SUCCEEDED(hr))
		{
			Transform inv = transform.invertTransform();
			D2D1::Matrix3x2F m1 = inv.getMatrix();
			D2D1::Matrix3x2F m2 = _x.transform.getMatrix();
			D2D1::Matrix3x2F combineMatrix = m1 * m2;
			hr = currentGeometry->CombineWithGeometry(
				_x.currentGeometry.Get(),
				D2D1_COMBINE_MODE_XOR,
				combineMatrix,
				NULL,
				resGeometrySink.Get()
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = resGeometrySink->Close();
		}
		if (resGeometrySink) resGeometrySink.Reset();

		return Shape(resGeometry.Get());
	}

	SquareShape::SquareShape(float _length, float _startX, float _startY, Transform _transform)
		:length(_length), startX(_startX), startY(_startY)
	{
		ComPtr<ID2D1RectangleGeometry> newGeometry;
		HRESULT hr;
		auto pD2DFactory = D2DFactoryGlobal::getD2DFactory();
		hr = pD2DFactory->CreateRectangleGeometry(
			D2D1::RectF(
				_startX, _startY,
				_startX + _length, _startY + _length
			),
			newGeometry.GetAddressOf()
		);
		if (SUCCEEDED(hr))
			setOriginalGeometry(newGeometry.Get());
		setTransform(_transform);
	}

	RectangleShape::RectangleShape(float _width, float _height, float _startX, float _startY, Transform _transform)
		:width(_width), height(_height), startX(_startX), startY(_startY)
	{
		ComPtr<ID2D1RectangleGeometry> newGeometry;
		HRESULT hr;
		auto pD2DFactory = D2DFactoryGlobal::getD2DFactory();
		hr = pD2DFactory->CreateRectangleGeometry(
			D2D1::RectF(
				_startX, _startY,
				_startX + _width, _startY + _height
			),
			newGeometry.GetAddressOf()
		);
		if (SUCCEEDED(hr))
			setOriginalGeometry(newGeometry.Get());
		setTransform(_transform);
	}

	CircleShape::CircleShape(float _radius, float _startX, float _startY, Transform _transform)
		:radius(_radius), startX(_startX), startY(_startY)
	{
		ComPtr<ID2D1EllipseGeometry> newGeometry;
		HRESULT hr;
		auto pD2DFactory = D2DFactoryGlobal::getD2DFactory();
		hr = pD2DFactory->CreateEllipseGeometry(
			D2D1::Ellipse(
				D2D1::Point2F(
					_radius + _startX, _radius + _startY
				),
				_radius, _radius
			),
			newGeometry.GetAddressOf()
		);
		if (SUCCEEDED(hr))
			setOriginalGeometry(newGeometry.Get());
		setTransform(_transform);
	}

	EllipseShape::EllipseShape(float _radiusX, float _radiusY, float _startX, float _startY, Transform _transform)
		:radiusX(_radiusX), radiusY(_radiusY), startX(_startX), startY(_startY)
	{
		ComPtr<ID2D1EllipseGeometry> newGeometry;
		HRESULT hr;
		auto pD2DFactory = D2DFactoryGlobal::getD2DFactory();
		hr = pD2DFactory->CreateEllipseGeometry(
			D2D1::Ellipse(
				D2D1::Point2F(
					_radiusX + _startX, _radiusY + _startY
				),
				_radiusX, _radiusY
			),
			newGeometry.GetAddressOf()
		);
		if (SUCCEEDED(hr))
			setOriginalGeometry(newGeometry.Get());
		setTransform(_transform);
	}
} // namespace suku