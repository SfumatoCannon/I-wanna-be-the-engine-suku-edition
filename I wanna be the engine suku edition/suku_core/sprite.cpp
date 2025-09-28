#include "sprite.h"
#include "collision_box.h"

namespace suku
{
	bool SpriteZero::isCrashed(Transform _transform, const SpriteZero& _other, Transform _otherTransform)const
	{
		if (!hitArea || !(_other.hitArea))
			return false;
		return hitArea->isCrashed(_transform, *(_other.hitArea), _otherTransform);
	}

	bool SpriteZero::isCrashed(Transform _transform, const SpriteZero* _other, Transform _otherTransform)const
	{
		if (!hitArea || !(_other->hitArea))
			return false;
		return hitArea->isCrashed(_transform, *(_other->hitArea), _otherTransform);
	}

	ShapeSpriteZero::ShapeSpriteZero(const Shape& _shape, ID2D1Brush* _fillBrush,
		ID2D1Brush* _outlineBrush, float _outlineWidth, ID2D1StrokeStyle* _outlineStrokeStyle)
	{
		shape = _shape;
		addRef_safe(_fillBrush);
		fillBrush = _fillBrush;
		addRef_safe(_outlineBrush);
		outlineBrush = _outlineBrush;

		outlineWidth = _outlineWidth;

		addRef_safe(_outlineStrokeStyle);
		outlineStrokeStyle = _outlineStrokeStyle;
	}

	ShapeSpriteZero::ShapeSpriteZero(const Shape& _shape, const Color& _fillColor)
	{
		shape = _shape;

		ID2D1SolidColorBrush* newBrush;
		HRESULT hr;
		hr = pMainRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(_fillColor.r() / 255.0f, _fillColor.g() / 255.0f, _fillColor.b() / 255.0f, _fillColor.alpha),
			&newBrush
		);
		if (SUCCEEDED(hr))
		{
			newBrush->AddRef();
			fillBrush = newBrush;
			newBrush->AddRef();
			outlineBrush = newBrush;
		}
		else
		{
			fillBrush = nullptr;
			outlineBrush = nullptr;
		}

		outlineWidth = 1.0f;
		outlineStrokeStyle = nullptr;
	}

	ShapeSpriteZero::ShapeSpriteZero(const Shape& _shape, const Color& _fillColor,
		const Color& _outlineColor, float _outlineWidth, ID2D1StrokeStyle* _outlineStrokeStyle)
	{
		shape = _shape;

		ID2D1SolidColorBrush* newBrush;
		HRESULT hr;
		hr = pMainRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(_fillColor.r() / 255.0f, _fillColor.g() / 255.0f, _fillColor.b() / 255.0f, _fillColor.alpha),
			&newBrush
		);
		if (SUCCEEDED(hr))
		{
			newBrush->AddRef();
			fillBrush = newBrush;
		}
		else
			fillBrush = nullptr;

		hr = pMainRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(_outlineColor.r() / 255.0f, _outlineColor.g() / 255.0f, _outlineColor.b() / 255.0f, _outlineColor.alpha),
			&newBrush
		);
		if (SUCCEEDED(hr))
		{
			newBrush->AddRef();
			outlineBrush = newBrush;
		}
		else
			outlineBrush = nullptr;

		outlineWidth = _outlineWidth;
		addRef_safe(_outlineStrokeStyle);
		outlineStrokeStyle = _outlineStrokeStyle;
	}

	ShapeSpriteZero::~ShapeSpriteZero()
	{
		release_safe(fillBrush);
		release_safe(outlineBrush);
		release_safe(outlineStrokeStyle);
	}

	void ShapeSpriteZero::setShapeTransform(Transform _transform)
	{
		shape.setTransform(_transform);
	}

	void ShapeSpriteZero::paint(float _x, float _y, float _xScale, float _yScale, float _angle)
	{
		Transform paintingTransform = translation(_x, _y) + scale(centerX, centerY, _xScale, _yScale)
			+ rotation(centerX, centerY, _angle);
		shape.paint(paintingTransform, fillBrush, outlineBrush, outlineWidth, outlineStrokeStyle);
	}

	void ShapeSpriteZero::paint(float _x, float _y, Transform _paintingTransform)
	{
		shape.paint(_x, _y, _paintingTransform, fillBrush, outlineBrush, outlineWidth, outlineStrokeStyle);
	}

	void ShapeSpriteZero::paint(Transform _paintingTransform)
	{
		shape.paint(_paintingTransform, fillBrush, outlineBrush, outlineWidth, outlineStrokeStyle);
	}

	void ShapeSpriteZero::setFillColor(const Color& _color)
	{
		if (fillBrush != nullptr)
			release_safe(fillBrush);
		ID2D1SolidColorBrush* newBrush;
		HRESULT hr;
		hr = pMainRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(_color.r() / 255.0f, _color.g() / 255.0f, _color.b() / 255.0f, _color.alpha),
			&newBrush
		);
		if (SUCCEEDED(hr))
		{
			newBrush->AddRef();
			fillBrush = newBrush;
		}
	}

	void ShapeSpriteZero::setOutlineColor(const Color& _color)
	{
		if (outlineBrush != nullptr)
			release_safe(outlineBrush);
		ID2D1SolidColorBrush* newBrush;
		HRESULT hr;
		hr = pMainRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(_color.r() / 255.0f, _color.g() / 255.0f, _color.b() / 255.0f, _color.alpha),
			&newBrush
		);
		if (SUCCEEDED(hr))
		{
			newBrush->AddRef();
			outlineBrush = newBrush;
		}
	}

	void ShapeSpriteZero::setOutlineWidth(int _width)
	{
		outlineWidth = (float)_width;
	}

	void ShapeSpriteZero::paint(float _x, float _y, float _xScale, float _yScale, float _alpha, float _angle)
	{
		shape.paint(translation(_x, _y) + scale(centerX, centerY, _xScale, _yScale) + rotation(centerX, centerY, _angle),
			fillBrush,
			outlineBrush,
			outlineWidth,
			outlineStrokeStyle
		);
	}

	void ShapeSpriteZero::paint(float _x, float _y, Transform _transform, float _alpha)
	{
		shape.paint(translation(_x, _y) + _transform,
			fillBrush,
			outlineBrush,
			outlineWidth,
			outlineStrokeStyle
		);
	}

	void ShapeSpriteZero::paint(Transform _transform, float _alpha)
	{
		shape.paint(_transform,
			fillBrush,
			outlineBrush,
			outlineWidth,
			outlineStrokeStyle
		);
	}

	void BitmapSpriteZero::catchBitmap(String _path, UINT _startX, UINT _startY)
	{
		if (pBitmap_ != nullptr)
		{
			delete pBitmap_;
			pBitmap_ = nullptr;
		}
		pBitmap_ = new Bitmap(_path, _startX, _startY, width, height);
	}

	void BitmapSpriteZero::catchBitmapAndSize(String _path)
	{
		if (pBitmap_ != nullptr)
		{
			delete pBitmap_;
			pBitmap_ = nullptr;
		}
		pBitmap_ = new Bitmap(_path);
		width = pBitmap_->getWidth();
		height = pBitmap_->getHeight();
	}

	void BitmapSpriteZero::paint(float _x, float _y, float _xScale, float _yScale, float _alpha, float _angle)
	{
		pBitmap_->paint(translation(_x, _y) + scale(centerX, centerY, _xScale, _yScale) + rotation(centerX, centerY, _angle),
			_alpha);
		//drawBitmap(d2dBitmap_, (float)width, (float)height, _alpha,
		//	translation(_x, _y) + scale(centerX, centerY, _xScale, _yScale) + rotation(centerX, centerY, _angle));
	}

	void BitmapSpriteZero::paint(float _x, float _y, Transform _transform, float _alpha)
	{
		pBitmap_->paint(translation(_x, _y) + _transform,
			_alpha);
		//drawBitmap(d2dBitmap_, (float)width, (float)height, _alpha,
		//	translation(_x, _y) + _transform);
	}

	void BitmapSpriteZero::paint(Transform _transform, float _alpha)
	{
		pBitmap_->paint(_transform, _alpha);
		//drawBitmap(d2dBitmap_, (float)width, (float)height, _alpha, _transform);
	}

	BitmapSpriteZero::BitmapSpriteZero()
	{
		hitArea = nullptr;
		pBitmap_ = nullptr;
		width = 0;
		height = 0;
		centerX = 0;
		centerY = 0;
	}

	BitmapSpriteZero::BitmapSpriteZero(const Shape& _collisionBox, const Bitmap& _bitmap, float _centerX, float _centerY)
	{
		width = _bitmap.getWidth();
		height = _bitmap.getHeight();
		centerX = _centerX;
		centerY = _centerY;
		pBitmap_ = new Bitmap(_bitmap);
		hitArea = new ShapeCollisionBox(_collisionBox);
	}

	BitmapSpriteZero::BitmapSpriteZero(String _path, const Shape& _collisionBox, float _centerX, float _centerY)
	{
		centerX = _centerX;
		centerY = _centerY;
		pBitmap_ = nullptr;
		catchBitmapAndSize(_path);
		hitArea = new ShapeCollisionBox(_collisionBox);
	}

	BitmapSpriteZero::BitmapSpriteZero(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height,
		const Shape& _collisionBox, float _centerX, float _centerY)
	{
		width = _width;
		height = _height;
		centerX = _centerX;
		centerY = _centerY;
		pBitmap_ = new Bitmap(_path, _startX, _startY, _width, _height);
		hitArea = new ShapeCollisionBox(_collisionBox);
	}

	BitmapSpriteZero::BitmapSpriteZero(UINT _width, UINT _height, const Shape& _collisionBox, float _centerX, float _centerY)
	{
		width = _width;
		height = _height;
		centerX = _centerX;
		centerY = _centerY;
		pBitmap_ = nullptr;
		hitArea = new ShapeCollisionBox(_collisionBox);
	}

	BitmapSpriteZero::BitmapSpriteZero(UINT _width, UINT _height, const BitmapCollisionBox& _collisionBox,
		float _centerX, float _centerY, String _path)
	{
		hitArea = new BitmapCollisionBox(_collisionBox);
		width = _width;
		height = _height;
		centerX = _centerX;
		centerY = _centerY;
		pBitmap_ = new Bitmap(_path);
	}

	BitmapSpriteZero::BitmapSpriteZero(String _path, float _centerX, float _centerY, float _alphaThreshold)
	{
		pBitmap_ = nullptr;
		centerX = _centerX;
		centerY = _centerY;

		pBitmap_ = new Bitmap(_path);

		if (pBitmap_)
		{
			width = pBitmap_->getWidth();
			height = pBitmap_->getHeight();
			hitArea = new BitmapCollisionBox(pBitmap_, _alphaThreshold);
		}
		else
		{
			width = height = 0;
			hitArea = nullptr;
		}
	}

	BitmapSpriteZero::BitmapSpriteZero(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height,
		float _centerX, float _centerY, float _alphaThreshold)
	{
		pBitmap_ = nullptr;
		width = _width;
		height = _height;
		centerX = _centerX;
		centerY = _centerY;

		pBitmap_ = new Bitmap(_path, _startX, _startY, _width, _height);

		if (pBitmap_)
			hitArea = new BitmapCollisionBox(pBitmap_, _alphaThreshold);
		else
			hitArea = nullptr;
	}

	Sprite::Sprite()
	{
		flipTime_ = 1;
	}

	Sprite::Sprite(String _path, UINT _amount, int _flipTime, const Shape& _collisionBox, float _centerX, float _centerY)
	{
		flipTime_ = _flipTime;
		Bitmap originalBitmap(_path);
		auto [originalWidth, originalHeight] = originalBitmap.getSize();
		UINT width = originalWidth / _amount;
		for (UINT i = 0; i < _amount; i++)
		{
			BitmapSpriteZero* sprZ = new BitmapSpriteZero(_path, i * width, 0, width, originalHeight,
				_collisionBox, _centerX, _centerY);
			bodyList.push_back((SpriteZero*)sprZ);
		}
	}

	Sprite::Sprite(String _path, UINT _amount, int _flipTime, float _centerX, float _centerY, float _alphaThreshold)
	{
		flipTime_ = _flipTime;
		Bitmap originalBitmap(_path);
		auto [originalWidth, originalHeight] = originalBitmap.getSize();
		UINT width = originalWidth / _amount;
		for (UINT i = 0; i < _amount; i++)
		{
			BitmapSpriteZero* sprZ = new BitmapSpriteZero(_path, i * width, 0, width, originalHeight,
				_centerX, _centerY, _alphaThreshold);
			bodyList.push_back((SpriteZero*)sprZ);
		}
	}

	Sprite::Sprite(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height, UINT _amount, int _flipTime,
		const Shape& _collisionBox, float _centerX, float _centerY)
	{
		flipTime_ = _flipTime;
		for (UINT i = 0; i < _amount; i++)
		{
			BitmapSpriteZero* sprZ = new BitmapSpriteZero(_path, _startX + i * _width, _startY, _width, _height,
				_collisionBox, _centerX, _centerY);
			bodyList.push_back((SpriteZero*)sprZ);
		}
	}

	Sprite::Sprite(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height, UINT _amount, int _flipTime,
		float _centerX, float _centerY, float _alphaThreshold)
	{
		flipTime_ = _flipTime;
		for (UINT i = 0; i < _amount; i++)
		{
			BitmapSpriteZero* sprZ = new BitmapSpriteZero(_path, _startX + i * _width, _startY, _width, _height,
				_centerX, _centerY, _alphaThreshold);
			bodyList.push_back((SpriteZero*)sprZ);
		}
	}

	void Sprite::setSpeed(int _speed)
	{
		flipTime_ = _speed;
	}

	SpriteZero* Sprite::getState(int _wp)
	{
		if (bodyList.empty())
			return nullptr;
		else return bodyList[_wp / flipTime_ % bodyList.size()];
	}
}