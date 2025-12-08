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
				auto pD2DFactory = D2DFactoryGlobal::getD2DFactory();
				pD2DFactory->
					CreateTransformedGeometry(originalGeometry.Get(), transform.matrix, currentGeometry.GetAddressOf());
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
			originalGeometry.Attach(_geometry);
			auto pD2DFactory = D2DFactoryGlobal::getD2DFactory();
			pD2DFactory->CreateTransformedGeometry(originalGeometry.Get(), transform.matrix, currentGeometry.GetAddressOf());
		}
		else
			originalGeometry = currentGeometry = nullptr;
	}

	void Shape::setTransform(Transform _transform)
	{
		transform = _transform;
		release_safe(currentGeometry);
		auto pD2DFactory = D2DFactoryGlobal::getD2DFactory();
		pD2DFactory->CreateTransformedGeometry(originalGeometry.Get(), transform.matrix, currentGeometry.GetAddressOf());
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
				pMainRenderTarget->DrawGeometry(currentGeometry.Get(), _outlineBrush, _outlineWidth, outlineStrokeStyle);
			if (_fillBrush != nullptr)
				pMainRenderTarget->FillGeometry(currentGeometry.Get(), _fillBrush, NULL);
		}
	}

	void Shape::paint(float _x, float _y, Transform _paintingTransform, ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush, float _outlineWidth, ID2D1StrokeStyle* outlineStrokeStyle)
	{
		if (currentGeometry != nullptr)
		{
			setPaintingTransform(translation(_x, _y) + _paintingTransform);
			if (_outlineBrush != nullptr)
				pMainRenderTarget->DrawGeometry(currentGeometry.Get(), _outlineBrush, _outlineWidth, outlineStrokeStyle);
			if (_fillBrush != nullptr)
				pMainRenderTarget->FillGeometry(currentGeometry.Get(), _fillBrush, NULL);
		}
	}

	void Shape::paint(Transform _paintingTransform, ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush, float _outlineWidth, ID2D1StrokeStyle* outlineStrokeStyle)
	{
		if (currentGeometry != nullptr)
		{
			setPaintingTransform(_paintingTransform);
			if (_outlineBrush != nullptr)
				pMainRenderTarget->DrawGeometry(currentGeometry.Get(), _outlineBrush, _outlineWidth, outlineStrokeStyle);
			if (_fillBrush != nullptr)
				pMainRenderTarget->FillGeometry(currentGeometry.Get(), _fillBrush, NULL);
		}
	}

	Bitmap* Shape::paintOnBitmap(Bitmap& _bitmap, float _x, float _y, ID2D1Brush* _fillBrush, ID2D1Brush* _outlineBrush, float _outlineWidth, ID2D1StrokeStyle* outlineStrokeStyle)
	{
		_bitmap.refreshD2DBitmap();
		if (!_bitmap.d2dBitmap_ || !currentGeometry)
			return nullptr;

		ComPtr<ID2D1BitmapRenderTarget> pBitmapRenderTarget = nullptr;
		HRESULT hr = pMainRenderTarget->CreateCompatibleRenderTarget(
			D2D1::SizeF((FLOAT)_bitmap.getWidth(), (FLOAT)_bitmap.getHeight()),
			pBitmapRenderTarget.GetAddressOf()
		);

		if (SUCCEEDED(hr) && pBitmapRenderTarget)
		{
			pBitmapRenderTarget->BeginDraw();
			D2D1_RECT_F destRect = D2D1::RectF(0, 0, (FLOAT)_bitmap.getWidth(), (FLOAT)_bitmap.getHeight());
			// draw the bitmap to the compatible render target using simple overload
			pBitmapRenderTarget->DrawBitmap(
				_bitmap.d2dBitmap_.Get(),
				nullptr,
				1.0f,
				D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
				nullptr
			);

			pBitmapRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(_x, _y));

			if (_outlineBrush)
				pBitmapRenderTarget->DrawGeometry(currentGeometry.Get(), _outlineBrush, _outlineWidth, outlineStrokeStyle);

			if (_fillBrush)
				pBitmapRenderTarget->FillGeometry(currentGeometry.Get(), _fillBrush);

			hr = pBitmapRenderTarget->EndDraw();

			if (hr == D2DERR_RECREATE_TARGET)
			{
				ERRORWINDOW_GLOBAL("Render target needs to be recreated");
				pBitmapRenderTarget.Reset();
				return nullptr;
			}

			ComPtr<ID2D1Bitmap> pBitmapResult = nullptr;
			Bitmap* resultBitmap = nullptr;
			hr = pBitmapRenderTarget->GetBitmap(pBitmapResult.GetAddressOf());
			if (SUCCEEDED(hr) && pBitmapResult)
			{
				resultBitmap = new Bitmap(pBitmapResult.Get());
			}
			pBitmapRenderTarget.Reset();
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
		paint(_x, _y, fillBrush_.Get(), outlineBrush_.Get(), outlineWidth_, outlineStrokeStyle_.Get());
	}

	void Shape::paint(float _x, float _y, Transform _paintingTransform)
	{
		paint(_x, _y, _paintingTransform, fillBrush_.Get(), outlineBrush_.Get(), outlineWidth_, outlineStrokeStyle_.Get());
	}

	void Shape::paint(Transform _paintingTransform)
	{
		paint(_paintingTransform, fillBrush_.Get(), outlineBrush_.Get(), outlineWidth_, outlineStrokeStyle_.Get());
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
				auto pD2DFactory = D2DFactoryGlobal::getD2DFactory();
				pD2DFactory->
					CreateTransformedGeometry(originalGeometry.Get(), transform.matrix, currentGeometry.GetAddressOf());
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
			hr = currentGeometry->CombineWithGeometry(
				_x.currentGeometry.Get(),
				D2D1_COMBINE_MODE_EXCLUDE,
				transform.invertTransform().matrix * _x.transform.matrix,
				NULL,
				resGeometrySink.Get()
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = resGeometrySink->Close();
		}
		release_safe(resGeometrySink);

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
			hr = currentGeometry->CombineWithGeometry(
				_x.currentGeometry.Get(),
				D2D1_COMBINE_MODE_INTERSECT,
				transform.invertTransform().matrix * _x.transform.matrix,
				NULL,
				resGeometrySink.Get()
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = resGeometrySink->Close();
		}
		release_safe(resGeometrySink);

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
			hr = currentGeometry->CombineWithGeometry(
				_x.currentGeometry.Get(),
				D2D1_COMBINE_MODE_UNION,
				transform.invertTransform().matrix * _x.transform.matrix,
				NULL,
				resGeometrySink.Get()
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = resGeometrySink->Close();
		}
		release_safe(resGeometrySink);

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
			hr = currentGeometry->CombineWithGeometry(
				_x.currentGeometry.Get(),
				D2D1_COMBINE_MODE_XOR,
				transform.invertTransform().matrix * _x.transform.matrix,
				NULL,
				resGeometrySink.Get()
			);
		}
		if (SUCCEEDED(hr))
		{
			hr = resGeometrySink->Close();
		}
		release_safe(resGeometrySink);

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
				_startX + _length - 1.0f, _startY + _length - 1.0f
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
				_startX + _width - 1.0f, _startY + _height - 1.0f
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

	ID2D1Brush* createSolidColorBrush(const Color _color)
	{
		ID2D1SolidColorBrush* newBrush = nullptr;
		pMainRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(_color.r(), _color.g(), _color.b(), _color.alpha),
			&newBrush
		);
		return newBrush;
	}
}