#include "shape.h"
#include "includes.h"
#include "../framework.h"

namespace suku
{

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
			originalGeometry->AddRef();
			if (_x.currentGeometry)
			{
				currentGeometry = _x.currentGeometry;
				currentGeometry->AddRef();
			}
			else
			{
				pD2DFactory->
					CreateTransformedGeometry(originalGeometry, transform.matrix, &currentGeometry);
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
		originalGeometry = _x.originalGeometry;
		currentGeometry = _x.currentGeometry;
		_x.originalGeometry = nullptr;
		_x.currentGeometry = nullptr;
	}

	Shape::Shape(ID2D1Geometry* _geometry, Transform _transform)
	{
		originalGeometry = nullptr;
		transform = Transform();
		currentGeometry = nullptr;
		setOriginalGeometry(_geometry);
	}

	void Shape::join()
	{
		addRef_safe(originalGeometry);
		addRef_safe(currentGeometry);
	}

	Shape::~Shape()
	{
		release_safe(originalGeometry);
		release_safe(currentGeometry);
	}

	void Shape::setOriginalGeometry(ID2D1Geometry* _geometry)
	{
		release_safe(originalGeometry);
		release_safe(currentGeometry);
		if (_geometry)
		{
			_geometry->AddRef();
			originalGeometry = _geometry;
			pD2DFactory->CreateTransformedGeometry(originalGeometry, transform.matrix, &currentGeometry);
		}
		else
			originalGeometry = currentGeometry = nullptr;
	}

	void Shape::setTransform(Transform _transform)
	{
		transform = _transform;
		release_safe(currentGeometry);
		pD2DFactory->CreateTransformedGeometry(originalGeometry, transform.matrix, &currentGeometry);
	}

	void Shape::setFill(Color _color)
	{

	}

	void Shape::setOutline(Color _color)
	{

	}

	void Shape::paint(float _x, float _y, ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush, float _outlineWidth,
		ID2D1StrokeStyle* outlineStrokeStyle)
	{
		if (currentGeometry != nullptr)
		{
			setPaintingTransform(translation(_x, _y));
			if (_outlineBrush != nullptr)
				pMainRenderTarget->DrawGeometry(currentGeometry, _outlineBrush, _outlineWidth, outlineStrokeStyle);
			if (_fillBrush != nullptr)
				pMainRenderTarget->FillGeometry(currentGeometry, _fillBrush, NULL);
		}
	}

	void Shape::paint(float _x, float _y, Transform _paintingTransform, ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush, float _outlineWidth, ID2D1StrokeStyle* outlineStrokeStyle)
	{
		if (currentGeometry != nullptr)
		{
			setPaintingTransform(translation(_x, _y) + _paintingTransform);
			if (_outlineBrush != nullptr)
				pMainRenderTarget->DrawGeometry(currentGeometry, _outlineBrush, _outlineWidth, outlineStrokeStyle);
			if (_fillBrush != nullptr)
				pMainRenderTarget->FillGeometry(currentGeometry, _fillBrush, NULL);
		}
	}

	void Shape::paint(Transform _paintingTransform, ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush, float _outlineWidth, ID2D1StrokeStyle* outlineStrokeStyle)
	{
		if (currentGeometry != nullptr)
		{
			setPaintingTransform(_paintingTransform);
			if (_outlineBrush != nullptr)
				pMainRenderTarget->DrawGeometry(currentGeometry, _outlineBrush, _outlineWidth, outlineStrokeStyle);
			if (_fillBrush != nullptr)
				pMainRenderTarget->FillGeometry(currentGeometry, _fillBrush, NULL);
		}
	}

	Bitmap* Shape::paintOnBitmap(Bitmap& _bitmap, float _x, float _y, ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush, float _outlineWidth, ID2D1StrokeStyle* outlineStrokeStyle)
	{
		_bitmap.refreshD2DBitmap();
		if (!_bitmap.d2dBitmap_ || !currentGeometry)
			return nullptr;

		ID2D1BitmapRenderTarget* pBitmapRenderTarget = nullptr;
		HRESULT hr = pMainRenderTarget->CreateCompatibleRenderTarget(
			D2D1::SizeF((FLOAT)_bitmap.getWidth(), (FLOAT)_bitmap.getHeight()),
			&pBitmapRenderTarget
		);

		if (SUCCEEDED(hr) && pBitmapRenderTarget)
		{
			pBitmapRenderTarget->BeginDraw();
			pBitmapRenderTarget->DrawBitmap(_bitmap.d2dBitmap_, D2D1::RectF(0, 0, (FLOAT)_bitmap.getWidth(), (FLOAT)_bitmap.getHeight()));
			pBitmapRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(_x, _y));

			if (_outlineBrush)
				pBitmapRenderTarget->DrawGeometry(currentGeometry, _outlineBrush, _outlineWidth, outlineStrokeStyle);

			if (_fillBrush)
				pBitmapRenderTarget->FillGeometry(currentGeometry, _fillBrush);

			hr = pBitmapRenderTarget->EndDraw();

			if (hr == D2DERR_RECREATE_TARGET)
			{
				ERRORWINDOW_GLOBAL("Render target needs to be recreated");
				pBitmapRenderTarget->Release();
				return nullptr;
			}

			ID2D1Bitmap* pBitmapResult = nullptr;
			Bitmap* resultBitmap = nullptr;
			hr = pBitmapRenderTarget->GetBitmap(&pBitmapResult);
			if (SUCCEEDED(hr) && pBitmapResult)
			{
				resultBitmap = new Bitmap(pBitmapResult);
				pBitmapResult->Release();
			}
			pBitmapRenderTarget->Release();
			return resultBitmap;
		}
		else
		{
			ERRORWINDOW_GLOBAL("Failed to create compatible render target for bitmap painting");
			return nullptr;
		}
	}

	void Shape::paint(float _x, float _y)
	{
		paint(_x, _y, fillBrush_, outlineBrush_, outlineWidth_, outlineStrokeStyle_);
	}

	void Shape::paint(float _x, float _y, Transform _paintingTransform)
	{
		paint(_x, _y, _paintingTransform, fillBrush_, outlineBrush_, outlineWidth_, outlineStrokeStyle_);
	}

	void Shape::paint(Transform _paintingTransform)
	{
		paint(_paintingTransform, fillBrush_, outlineBrush_, outlineWidth_, outlineStrokeStyle_);
	}

	bool Shape::isCrashed(Shape& _x)
	{
		D2D1_GEOMETRY_RELATION result;
		currentGeometry->CompareWithGeometry(_x.currentGeometry, nullptr, &result);
		if (result == D2D1_GEOMETRY_RELATION_DISJOINT)
			return false;
		else return true;
	}

	Shape& Shape::operator=(const Shape& _x)
	{
		transform = _x.transform;
		release_safe(originalGeometry);
		release_safe(currentGeometry);
		if (_x.originalGeometry)
		{
			originalGeometry = _x.originalGeometry;
			originalGeometry->AddRef();
			if (_x.currentGeometry)
			{
				currentGeometry = _x.currentGeometry;
				currentGeometry->AddRef();
			}
			else
			{
				pD2DFactory->
					CreateTransformedGeometry(originalGeometry, transform.matrix, &currentGeometry);
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
		release_safe(originalGeometry);
		release_safe(currentGeometry);
		originalGeometry = _x.originalGeometry;
		currentGeometry = _x.currentGeometry;
		_x.originalGeometry = nullptr;
		_x.currentGeometry = nullptr;
		return (*this);
	}

	Shape Shape::operator-(const Shape& _x)
	{
		ID2D1GeometrySink* resGeometrySink = nullptr;
		ID2D1PathGeometry* resGeometry;
		HRESULT hr;
		pD2DFactory->CreatePathGeometry(&resGeometry);
		hr = resGeometry->Open(&resGeometrySink);
		if (SUCCEEDED(hr))
		{
			hr = currentGeometry->CombineWithGeometry(
				_x.currentGeometry,
				D2D1_COMBINE_MODE_EXCLUDE,
				transform.invertTransform().matrix * _x.transform.matrix,
				NULL,
				resGeometrySink
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = resGeometrySink->Close();
		}
		release_safe(resGeometrySink);

		return Shape(resGeometry);
	}

	Shape Shape::operator&(const Shape& _x)
	{
		ID2D1GeometrySink* resGeometrySink = nullptr;
		ID2D1PathGeometry* resGeometry;
		HRESULT hr;
		pD2DFactory->CreatePathGeometry(&resGeometry);
		hr = resGeometry->Open(&resGeometrySink);
		if (SUCCEEDED(hr))
		{
			hr = currentGeometry->CombineWithGeometry(
				_x.currentGeometry,
				D2D1_COMBINE_MODE_INTERSECT,
				transform.invertTransform().matrix * _x.transform.matrix,
				NULL,
				resGeometrySink
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = resGeometrySink->Close();
		}
		release_safe(resGeometrySink);

		return Shape(resGeometry);
	}

	Shape Shape::operator|(const Shape& _x)
	{
		ID2D1GeometrySink* resGeometrySink = nullptr;
		ID2D1PathGeometry* resGeometry;
		HRESULT hr;
		pD2DFactory->CreatePathGeometry(&resGeometry);
		hr = resGeometry->Open(&resGeometrySink);
		if (SUCCEEDED(hr))
		{
			hr = currentGeometry->CombineWithGeometry(
				_x.currentGeometry,
				D2D1_COMBINE_MODE_UNION,
				transform.invertTransform().matrix * _x.transform.matrix,
				NULL,
				resGeometrySink
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = resGeometrySink->Close();
		}
		release_safe(resGeometrySink);

		return Shape(resGeometry);
	}

	Shape Shape::operator^(const Shape& _x)
	{
		ID2D1GeometrySink* resGeometrySink = nullptr;
		ID2D1PathGeometry* resGeometry;
		HRESULT hr;
		pD2DFactory->CreatePathGeometry(&resGeometry);
		hr = resGeometry->Open(&resGeometrySink);
		if (SUCCEEDED(hr))
		{
			hr = currentGeometry->CombineWithGeometry(
				_x.currentGeometry,
				D2D1_COMBINE_MODE_XOR,
				transform.invertTransform().matrix * _x.transform.matrix,
				NULL,
				resGeometrySink
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = resGeometrySink->Close();
		}
		release_safe(resGeometrySink);

		return Shape(resGeometry);
	}

	SquareShape::SquareShape(float _length, float _startX, float _startY, Transform _transform)
		:length(_length), startX(_startX), startY(_startY)
	{
		ID2D1RectangleGeometry* newGeometry;
		HRESULT hr;
		hr = pD2DFactory->CreateRectangleGeometry(
			D2D1::RectF(
				_startX, _startY,
				_startX + _length - 1.0f, _startY + _length - 1.0f
			),
			&newGeometry
		);
		if (SUCCEEDED(hr))
			setOriginalGeometry(newGeometry);
		setTransform(_transform);
		release_safe(newGeometry);
	}

	RectangleShape::RectangleShape(float _width, float _height, float _startX, float _startY, Transform _transform)
		:width(_width), height(_height), startX(_startX), startY(_startY)
	{
		ID2D1RectangleGeometry* newGeometry;
		HRESULT hr;
		hr = pD2DFactory->CreateRectangleGeometry(
			D2D1::RectF(
				_startX, _startY,
				_startX + _width - 1.0f, _startY + _height - 1.0f
			),
			&newGeometry
		);
		if (SUCCEEDED(hr))
			setOriginalGeometry(newGeometry);
		setTransform(_transform);
		release_safe(newGeometry);
	}

	CircleShape::CircleShape(float _radius, float _startX, float _startY, Transform _transform)
		:radius(_radius), startX(_startX), startY(_startY)
	{
		ID2D1EllipseGeometry* newGeometry;
		HRESULT hr;
		hr = pD2DFactory->CreateEllipseGeometry(
			D2D1::Ellipse(
				D2D1::Point2F(
					_radius + _startX, _radius + _startY
				),
				_radius, _radius
			),
			&newGeometry
		);
		if (SUCCEEDED(hr))
			setOriginalGeometry(newGeometry);
		setTransform(_transform);
		release_safe(newGeometry);
	}

	EllipseShape::EllipseShape(float _radiusX, float _radiusY, float _startX, float _startY, Transform _transform)
		:radiusX(_radiusX), radiusY(_radiusY), startX(_startX), startY(_startY)
	{
		ID2D1EllipseGeometry* newGeometry;
		HRESULT hr;
		hr = pD2DFactory->CreateEllipseGeometry(
			D2D1::Ellipse(
				D2D1::Point2F(
					_radiusX + _startX, _radiusY + _startY
				),
				_radiusX, _radiusY
			),
			&newGeometry
		);
		if (SUCCEEDED(hr))
			setOriginalGeometry(newGeometry);
		setTransform(_transform);
		release_safe(newGeometry);
	}

	ID2D1Brush* createSolidColorBrush(const Color _color)
	{
		ID2D1SolidColorBrush* newBrush;
		pMainRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(_color.r(), _color.g(), _color.b(), _color.alpha),
			&newBrush
		);
		return newBrush;
	}
}