#include "sprite.h"
#include "collision_box.h"

namespace suku
{
	bool SpriteElement::isCrashed(Transform _transform, const SpriteElement& _other, Transform _otherTransform)const
	{
		if (!hitArea_ || !(_other.hitArea_))
			return false;
		return hitArea_->isCrashed(_transform, *(_other.hitArea_), _otherTransform);
	}

	bool SpriteElement::isCrashed(Transform _transform, const SpriteElement* _other, Transform _otherTransform)const
	{
		if (!hitArea_ || !(_other->hitArea_))
			return false;
		return hitArea_->isCrashed(_transform, *(_other->hitArea_), _otherTransform);
	}

	ShapeSpriteElement::ShapeSpriteElement(const Shape& _shape, const ComPtr<ID2D1Brush>& _fillBrush,
		const ComPtr<ID2D1Brush>& _outlineBrush, float _outlineWidth, const ComPtr<ID2D1StrokeStyle>& _outlineStrokeStyle)
	{
		shape = _shape;
		fillBrush = _fillBrush;
		outlineBrush = _outlineBrush;

		outlineWidth = _outlineWidth;

		outlineStrokeStyle = _outlineStrokeStyle;
	}

	ShapeSpriteElement::ShapeSpriteElement(const Shape& _shape, const Color& _fillColor)
	{
		shape = _shape;

		ComPtr<ID2D1SolidColorBrush> newBrush;
		newBrush = graphics::createSolidColorBrush(_fillColor);
		fillBrush = newBrush;
		outlineBrush = newBrush;

		outlineWidth = 1.0f;
		outlineStrokeStyle = nullptr;
	}

	ShapeSpriteElement::ShapeSpriteElement(const Shape& _shape, const Color& _fillColor,
		const Color& _outlineColor, float _outlineWidth, const ComPtr<ID2D1StrokeStyle>& _outlineStrokeStyle)
	{
		shape = _shape;

		fillBrush = graphics::createSolidColorBrush(_fillColor);
		outlineBrush = graphics::createSolidColorBrush(_outlineColor);

		outlineWidth = _outlineWidth;
		outlineStrokeStyle = _outlineStrokeStyle;
	}

	void ShapeSpriteElement::setShapeTransform(Transform _transform)
	{
		shape.setTransform(_transform);
	}

	void ShapeSpriteElement::paint(float _x, float _y, float _xScale, float _yScale, float _angle)
	{
		Transform paintingTransform = translation(_x, _y) + scale(centerX, centerY, _xScale, _yScale)
			+ rotation(centerX, centerY, _angle);
		shape.paint(paintingTransform, fillBrush, outlineBrush, outlineWidth, outlineStrokeStyle);
	}

	void ShapeSpriteElement::paint(float _x, float _y, Transform _paintingTransform)
	{
		shape.paint(_x, _y, _paintingTransform, fillBrush, outlineBrush, outlineWidth, outlineStrokeStyle);
	}

	void ShapeSpriteElement::paint(Transform _paintingTransform)
	{
		shape.paint(_paintingTransform, fillBrush, outlineBrush, outlineWidth, outlineStrokeStyle);
	}

	void ShapeSpriteElement::setFillColor(const Color& _color)
	{
		fillBrush = graphics::createSolidColorBrush(_color);
	}

	void ShapeSpriteElement::setOutlineColor(const Color& _color)
	{
		outlineBrush = graphics::createSolidColorBrush(_color);
	}

	void ShapeSpriteElement::setOutlineWidth(int _width)
	{
		outlineWidth = (float)_width;
	}

	void ShapeSpriteElement::paint(float _x, float _y, float _xScale, float _yScale, float _alpha, float _angle)
	{
		shape.paint(translation(_x, _y) + scale(centerX, centerY, _xScale, _yScale) + rotation(centerX, centerY, _angle),
			fillBrush,
			outlineBrush,
			outlineWidth,
			outlineStrokeStyle
		);
	}

	void ShapeSpriteElement::paint(float _x, float _y, Transform _transform, float _alpha)
	{
		shape.paint(translation(_x, _y) + _transform,
			fillBrush,
			outlineBrush,
			outlineWidth,
			outlineStrokeStyle
		);
	}

	void ShapeSpriteElement::paint(Transform _transform, float _alpha)
	{
		shape.paint(_transform,
			fillBrush,
			outlineBrush,
			outlineWidth,
			outlineStrokeStyle
		);
	}

	void BitmapSpriteElement::catchBitmap(String _path, UINT _startX, UINT _startY)
	{
		pBitmap_ = std::make_unique<Bitmap>(_path, _startX, _startY, width, height);
	}

	void BitmapSpriteElement::catchBitmapAndSize(String _path)
	{
		pBitmap_ = std::make_unique<Bitmap>(_path);
		width = pBitmap_->getWidth();
		height = pBitmap_->getHeight();
	}

	void BitmapSpriteElement::paint(float _x, float _y, float _xScale, float _yScale, float _alpha, float _angle)
	{
		pBitmap_->paint(translation(_x, _y) + scale(centerX, centerY, _xScale, _yScale) + rotation(centerX, centerY, _angle),
			_alpha);
	}

	void BitmapSpriteElement::paint(float _x, float _y, Transform _transform, float _alpha)
	{
		pBitmap_->paint(translation(_x, _y) + _transform,
			_alpha);
	}

	void BitmapSpriteElement::paint(Transform _transform, float _alpha)
	{
		pBitmap_->paint(_transform, _alpha);
	}

	BitmapSpriteElement::BitmapSpriteElement()
	{
		hitArea_ = nullptr;
		pBitmap_ = nullptr;
		width = 0;
		height = 0;
		centerX = 0;
		centerY = 0;
	}

	BitmapSpriteElement::BitmapSpriteElement(const Shape& _collisionBox, const Bitmap& _bitmap, float _centerX, float _centerY)
	{
		width = _bitmap.getWidth();
		height = _bitmap.getHeight();
		centerX = _centerX;
		centerY = _centerY;
		pBitmap_ = std::make_unique<Bitmap>(_bitmap);
		hitArea_ = std::make_unique<ShapeCollisionBox>(_collisionBox);
	}

	BitmapSpriteElement::BitmapSpriteElement(String _path, const Shape& _collisionBox, float _centerX, float _centerY)
	{
		centerX = _centerX;
		centerY = _centerY;
		pBitmap_ = nullptr;
		catchBitmapAndSize(_path);
		hitArea_ = std::make_unique<ShapeCollisionBox>(_collisionBox);
	}

	BitmapSpriteElement::BitmapSpriteElement(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height,
		const Shape& _collisionBox, float _centerX, float _centerY)
	{
		width = _width;
		height = _height;
		centerX = _centerX;
		centerY = _centerY;
		pBitmap_ = std::make_unique<Bitmap>(_path, _startX, _startY, _width, _height);
		hitArea_ = std::make_unique<ShapeCollisionBox>(_collisionBox);
	}

	BitmapSpriteElement::BitmapSpriteElement(UINT _width, UINT _height, const Shape& _collisionBox, float _centerX, float _centerY)
	{
		width = _width;
		height = _height;
		centerX = _centerX;
		centerY = _centerY;
		pBitmap_ = nullptr;
		hitArea_ = std::make_unique<ShapeCollisionBox>(_collisionBox);
	}

	BitmapSpriteElement::BitmapSpriteElement(UINT _width, UINT _height, const BitmapCollisionBox& _collisionBox,
		float _centerX, float _centerY, String _path)
	{
		hitArea_ = std::make_unique<BitmapCollisionBox>(_collisionBox);
		width = _width;
		height = _height;
		centerX = _centerX;
		centerY = _centerY;
		pBitmap_ = std::make_unique<Bitmap>(_path);
	}

	BitmapSpriteElement::BitmapSpriteElement(String _path, float _centerX, float _centerY, float _alphaThreshold)
	{
		pBitmap_ = nullptr;
		centerX = _centerX;
		centerY = _centerY;

		pBitmap_ = std::make_unique<Bitmap>(_path);

		if (pBitmap_)
		{
			width = pBitmap_->getWidth();
			height = pBitmap_->getHeight();
			hitArea_ = std::make_unique<BitmapCollisionBox>(pBitmap_.get(), _alphaThreshold);
		}
		else
		{
			width = height = 0;
			hitArea_ = nullptr;
		}
	}

	BitmapSpriteElement::BitmapSpriteElement(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height,
		float _centerX, float _centerY, float _alphaThreshold)
	{
		pBitmap_ = nullptr;
		width = _width;
		height = _height;
		centerX = _centerX;
		centerY = _centerY;

		pBitmap_ = std::make_unique<Bitmap>(_path, _startX, _startY, _width, _height);

		if (pBitmap_)
			hitArea_ = std::make_unique<BitmapCollisionBox>(pBitmap_.get(), _alphaThreshold);
		else
			hitArea_ = nullptr;
	}

	Sprite::Sprite()
		: flipTime_(0), width_(0), height_(0), centerX_(0), centerY_(0) {
	}

	Sprite::Sprite(String _path, const Shape& _collisionBox, float _centerX, float _centerY)
		: flipTime_(0), centerX_(_centerX), centerY_(_centerY), rowCount_(1), colCount_(1)
	{
		Bitmap originalBitmap(_path);
		auto [originalWidth, originalHeight] = originalBitmap.getSize();

		width_ = originalWidth;
		height_ = originalHeight;

		push(BitmapSpriteElement(_path, 0, 0, width_, height_, _collisionBox, _centerX, _centerY));
	}

	Sprite::Sprite(String _path, float _centerX, float _centerY, float _alphaThreshold)
		: flipTime_(0), centerX_(_centerX), centerY_(_centerY), rowCount_(1), colCount_(1)
	{
		Bitmap originalBitmap(_path);
		auto [originalWidth, originalHeight] = originalBitmap.getSize();

		width_ = originalWidth;
		height_ = originalHeight;

		push(BitmapSpriteElement(_path, 0, 0, width_, height_, _centerX, _centerY, _alphaThreshold));
	}

	Sprite::Sprite(String _path, UINT _amount, int _flipTime, const Shape& _collisionBox, float _centerX, float _centerY)
		: flipTime_(_flipTime), centerX_(_centerX), centerY_(_centerY), rowCount_(1), colCount_(_amount)
	{
		Bitmap originalBitmap(_path);
		auto [originalWidth, originalHeight] = originalBitmap.getSize();
		UINT width = originalWidth / _amount;

		width_ = width;
		height_ = originalHeight;

		for (UINT i = 0; i < _amount; i++)
		{
			push(BitmapSpriteElement(_path, i * width_, 0, width_, height_,
				_collisionBox, _centerX, _centerY));
		}
	}

	Sprite::Sprite(String _path, UINT _amount, int _flipTime, float _centerX, float _centerY, float _alphaThreshold)
		: flipTime_(_flipTime), centerX_(_centerX), centerY_(_centerY), rowCount_(1), colCount_(_amount)
	{
		Bitmap originalBitmap(_path);
		auto [originalWidth, originalHeight] = originalBitmap.getSize();
		UINT width = originalWidth / _amount;

		width_ = width;
		height_ = originalHeight;

		for (UINT i = 0; i < _amount; i++)
		{
			push(BitmapSpriteElement(_path, i * width_, 0, width_, height_,
				_centerX, _centerY, _alphaThreshold));
		}
	}

	Sprite::Sprite(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height, UINT _amount, int _flipTime,
		const Shape& _collisionBox, float _centerX, float _centerY)
		: flipTime_(_flipTime), width_(_width), height_(_height), centerX_(_centerX), centerY_(_centerY), rowCount_(1), colCount_(_amount)
	{
		for (UINT i = 0; i < _amount; i++)
		{
			push(BitmapSpriteElement(_path, _startX + i * _width, _startY, _width, _height,
				_collisionBox, _centerX, _centerY));
		}
	}

	Sprite::Sprite(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height, UINT _amount, int _flipTime,
		float _centerX, float _centerY, float _alphaThreshold)
		: flipTime_(_flipTime), width_(_width), height_(_height), centerX_(_centerX), centerY_(_centerY), rowCount_(1), colCount_(_amount)
	{
		for (UINT i = 0; i < _amount; i++)
		{
			push(BitmapSpriteElement(_path, _startX + i * _width, _startY, _width, _height,
				_centerX, _centerY, _alphaThreshold));
		}
	}

	Sprite::Sprite(String _path, UINT _amountRow, UINT _amountCol, int _flipTime, 
		const Shape& _collisionBox, float _centerX, float _centerY)
		: flipTime_(_flipTime), centerX_(_centerX), centerY_(_centerY), rowCount_(_amountRow), colCount_(_amountCol)
	{
		Bitmap originalBitmap(_path);
		auto [originalWidth, originalHeight] = originalBitmap.getSize();
		UINT width = originalWidth / _amountCol;
		UINT height = originalHeight / _amountRow;

		width_ = width;
		height_ = height;

		for (UINT j = 0; j < _amountRow; j++)
		{
			for (UINT i = 0; i < _amountCol; i++)
			{
				push(BitmapSpriteElement(_path, i * width_, j * height_, width_, height_,
					_collisionBox, _centerX, _centerY));
			}
		}
	}

	Sprite::Sprite(String _path, UINT _amountRow, UINT _amountCol, int _flipTime, 
		float _centerX, float _centerY, float _alphaThreshold)
		: flipTime_(_flipTime), centerX_(_centerX), centerY_(_centerY), rowCount_(_amountRow), colCount_(_amountCol)
	{
		Bitmap originalBitmap(_path);
		auto [originalWidth, originalHeight] = originalBitmap.getSize();
		UINT width = originalWidth / _amountCol;
		UINT height = originalHeight / _amountRow;

		width_ = width;
		height_ = height;

		for (UINT j = 0; j < _amountRow; j++)
		{
			for (UINT i = 0; i < _amountCol; i++)
			{
				push(BitmapSpriteElement(_path, i * width_, j * height_, width_, height_,
					_centerX, _centerY, _alphaThreshold));
			}
		}
	}

	Sprite::Sprite(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height, UINT _amountRow, UINT _amountCol, 
		int _flipTime, const Shape& _collisionBox, float _centerX, float _centerY)
		: flipTime_(_flipTime), width_(_width), height_(_height), centerX_(_centerX), centerY_(_centerY), rowCount_(_amountRow), colCount_(_amountCol)
	{
		for (UINT j = 0; j < _amountRow; j++)
		{
			for (UINT i = 0; i < _amountCol; i++)
			{
				push(BitmapSpriteElement(_path, _startX + i * _width, _startY + j * _height, _width, _height,
					_collisionBox, _centerX, _centerY));
			}
		}
	}

	Sprite::Sprite(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height, UINT _amountRow, UINT _amountCol, int _flipTime, float _centerX, float _centerY, float _alphaThreshold)
		: flipTime_(_flipTime), width_(_width), height_(_height), centerX_(_centerX), centerY_(_centerY), rowCount_(_amountRow), colCount_(_amountCol)
	{
		for (UINT j = 0; j < _amountRow; j++)
		{
			for (UINT i = 0; i < _amountCol; i++)
			{
				push(BitmapSpriteElement(_path, _startX + i * _width, _startY + j * _height, _width, _height,
					_centerX, _centerY, _alphaThreshold));
			}
		}
	}

	void Sprite::setSpeed(int _speed)
	{
		flipTime_ = _speed;
	}

	void Sprite::setStartingIndex(UINT _index)
	{
		lastSetIndex_ = _index % bodyList.size();
	}

	SpriteElement* Sprite::getState(UINT _index)
	{
		return bodyList[_index % bodyList.size()].get();
	}

	SpriteElement* Sprite::getFrameState(long long _frameTick)const
	{
		if (bodyList.empty())
			return nullptr;
		else if (flipTime_ == 0)
			return bodyList[lastSetIndex_].get();
		else if (flipTime_ > 0)
			return bodyList[(lastSetIndex_ + _frameTick / flipTime_) % bodyList.size()].get();
		else // flipTime_ < 0
		{
			size_t size = bodyList.size();
			return bodyList[((lastSetIndex_ + _frameTick / flipTime_) % size + size) % size].get();
		}
	}

	SpriteElement* Sprite::getFrameState(long double _frameTick) const
	{
		return getFrameState(static_cast<long long>(_frameTick));
	}

	SpriteElement* Sprite::getFrameState(UINT _startIndex, long long _frameTick) const
	{
		if (bodyList.empty())
			return nullptr;
		else if (flipTime_ == 0)
			return bodyList[_startIndex].get();
		else if (flipTime_ > 0)
			return bodyList[(_startIndex + _frameTick / flipTime_) % bodyList.size()].get();
		else // flipTime_ < 0
		{
			size_t size = bodyList.size();
			return bodyList[((_startIndex + _frameTick / flipTime_) % size + size) % size].get();
		}
	}

	SpriteElement* Sprite::getFrameState(UINT _startIndex, long double _frameTick) const
	{
		return getFrameState(_startIndex, static_cast<long long>(_frameTick));
	}

	UINT Sprite::getFrameStateIndex(long long _frameTick) const
	{
		if (bodyList.empty())
			return 0;
		else if (flipTime_ == 0)
			return lastSetIndex_;
		else if (flipTime_ > 0)
			return static_cast<UINT>((lastSetIndex_ + _frameTick / flipTime_) % bodyList.size());
		else // flipTime_ < 0
		{
			size_t size = bodyList.size();
			return static_cast<UINT>(((lastSetIndex_ + _frameTick / flipTime_) % size + size) % size);
		}
	}

	UINT Sprite::getFrameStateIndex(long double _frameTick) const
	{
		return getFrameStateIndex(static_cast<long long>(_frameTick));
	}

	UINT Sprite::getFrameStateIndex(UINT _startIndex, long long _frameTick) const
	{
		if (bodyList.empty())
			return 0;
		else if (flipTime_ == 0)
			return _startIndex;
		else if (flipTime_ > 0)
			return static_cast<UINT>((_startIndex + _frameTick / flipTime_) % bodyList.size());
		else // flipTime_ < 0
		{
			size_t size = bodyList.size();
			return static_cast<UINT>(((_startIndex + _frameTick / flipTime_) % size + size) % size);
		}
	}

	UINT Sprite::getFrameStateIndex(UINT _startIndex, long double _frameTick) const
	{
		return getFrameStateIndex(_startIndex, static_cast<long long>(_frameTick));
	}
}