#pragma once
#include "../framework.h"
#include "../suku_foundation/suku_string.h"
#include "../suku_draw/shape.h"
#include "../suku_draw/bitmap.h"
#include "collision_box.h"
#include "../suku_draw/color.h"
#include "../suku_draw/transform.h"

namespace suku
{
	class SpriteElement;
	class BitmapSpriteElement;
	class ShapeSpriteElement;
	class Sprite;

	class CollisionBox;
	class BitmapCollisionBox;
	class ShapeCollisionBox;
	class Transform;
	class Shape;
	class Bitmap;
	
	template<typename T>
	concept sprite_element_type = std::is_base_of_v<SpriteElement, T>;

	class SpriteElement
	{
	public:
		UINT height = 0, width = 0;
		float centerX = 0.0f, centerY = 0.0f;

		SpriteElement() = default;
		SpriteElement(SpriteElement&& _other) = default;

		virtual void paint(float _x, float _y,
			float _xscale = 1.0, float _yscale = 1.0, float _alpha = 1.0, float _angle = 0.0) = 0;
		virtual void paint(float _x, float _y,
			Transform _transform, float _alpha = 1.0) = 0;
		virtual void paint(Transform _transform, float _alpha = 1.0) = 0;
		bool isCrashed(Transform _transform, const SpriteElement& _other, Transform _otherTransform)const;
		bool isCrashed(Transform _transform, const SpriteElement* _other, Transform _otherTransform)const;
	protected:
		std::unique_ptr<CollisionBox> hitArea_;
	};

	class BitmapSpriteElement :public SpriteElement
	{
	public:
		BitmapSpriteElement();
		BitmapSpriteElement(BitmapSpriteElement&& _other) = default;

		BitmapSpriteElement(const Shape& _collisionBox, const Bitmap& _bitmap, float _centerX = 0.0f, float _centerY = 0.0f);
		BitmapSpriteElement(UINT _width, UINT _height, const Shape& _collisionBox, float _centerX = 0.0f, float _centerY = 0.0f);
		BitmapSpriteElement(UINT _width, UINT _height, const BitmapCollisionBox& _collisionBox,
			float _centerX = 0, float _centerY = 0, String _path = String());

		BitmapSpriteElement(String _path, const Shape& _collisionBox, float _centerX = 0.0f, float _centerY = 0.0f);
		BitmapSpriteElement(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height,
			const Shape& _collisionBox, float _centerX = 0.0f, float _centerY = 0.0f);


		BitmapSpriteElement(String _path, float _centerX = 0, float _centerY = 0, float _alphaThreshold = 0.0f);
		BitmapSpriteElement(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height,
			float _centerX = 0.0f, float _centerY = 0.0f, float _alphaThreshold = 0.0f);


		void catchBitmap(String _path, UINT _startX = 0, UINT _startY = 0);
		void catchBitmapAndSize(String _path);

		virtual void paint(float _x, float _y,
			float _xScale = 1.0, float _yScale = 1.0, float _alpha = 1.0, float _angle = 0.0) override;
		virtual void paint(float _x, float _y,
			Transform _transform, float _alpha = 1.0) override;
		virtual void paint(Transform _transform, float _alpha = 1.0) override;
	private:
		std::unique_ptr<Bitmap> pBitmap_;
	};

	class ShapeSpriteElement :public SpriteElement
	{
	public:
		Shape shape;
		Microsoft::WRL::ComPtr<ID2D1Brush> fillBrush;
		Microsoft::WRL::ComPtr<ID2D1Brush> outlineBrush;
		Microsoft::WRL::ComPtr<ID2D1StrokeStyle> outlineStrokeStyle;
		float outlineWidth;

		ShapeSpriteElement(ShapeSpriteElement&& _other) = default;

		ShapeSpriteElement(const Shape& _shape, const Microsoft::WRL::ComPtr<ID2D1Brush>& _fillBrush = nullptr,
			const Microsoft::WRL::ComPtr<ID2D1Brush>& _outlineBrush = nullptr, float _outlineWidth = 1.0f, const Microsoft::WRL::ComPtr<ID2D1StrokeStyle>& _outlineStrokeStyle = nullptr);
		ShapeSpriteElement(const Shape& _shape, const Color& _fillColor);
		ShapeSpriteElement(const Shape& _shape, const Color& _fillColor,
			const Color& _outlineColor, float _outlineWidth = 1.0f, const Microsoft::WRL::ComPtr<ID2D1StrokeStyle>& _outlineStrokeStyle = nullptr);

		void setShapeTransform(Transform _transform);
		void paint(float _x, float _y,
			float _xScale = 1.0f, float _yScale = 1.0f, float _angle = 0.0f);
		void paint(float _x, float _y, Transform _paintingTransform);
		void paint(Transform _paintingTransform);
		void setFillColor(const Color& _color);
		void setOutlineColor(const Color& _color);
		void setOutlineWidth(int _width);

		virtual void paint(float _x, float _y,
			float _xScale = 1.0f, float _yScale = 1.0f, float _alpha = 1.0f, float _angle = 0.0f) override;
		virtual void paint(float _x, float _y,
			Transform _transform, float _alpha = 1.0) override;
		virtual void paint(Transform _transform, float _alpha = 1.0) override;
	};

	class Sprite
	{
	public:
		std::vector<std::unique_ptr<SpriteElement>> bodyList;
		Sprite();
		Sprite(Sprite&& _other) = default;
		Sprite& operator=(Sprite&& _other) = default;

		template<sprite_element_type T> Sprite(T&& _spriteZ);
		template<sprite_element_type T, sprite_element_type... TNext> Sprite(int _flipTime, T&& _spriteZ, TNext&&... _spriteZNext);

		//Load Sprite which has only one frame.
		Sprite(String _path, const Shape& _collisionBox, float _centerX = 0.0f, float _centerY = 0.0f);
		Sprite(String _path, float _centerX = 0.0f, float _centerY = 0.0f, float _alphaThreshold = 0.0f);
		//Load Sprite directly from the long sprite bitmap; width and height will be auto calculated.
		Sprite(String _path, UINT _amount, int _flipTime,
			const Shape& _collisionBox, float _centerX = 0.0f, float _centerY = 0.0f);
		Sprite(String _path, UINT _amount, int _flipTime,
			float _centerX = 0.0f, float _centerY = 0.0f, float _alphaThreshold = 0.0f);
		//Load Sprite directly from the long sprite bitmap, with given starting position and size.
		Sprite(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height, UINT _amount, int _flipTime,
			const Shape& _collisionBox, float _centerX = 0.0f, float _centerY = 0.0f);
		Sprite(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height, UINT _amount, int _flipTime,
			float _centerX = 0.0f, float _centerY = 0.0f, float _alphaThreshold = 0.0f);
		//Load Sprite directly from the tileset; width and height will be auto calculated.
		Sprite(String _path, UINT _amountRow, UINT _amountCol, int _flipTime,
			const Shape& _collisionBox, float _centerX = 0.0f, float _centerY = 0.0f);
		Sprite(String _path, UINT _amountRow, UINT _amountCol, int _flipTime,
			float _centerX = 0.0f, float _centerY = 0.0f, float _alphaThreshold = 0.0f);
		//Load Sprite directly from the tileset, with given starting position and size.
		Sprite(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height, UINT _amountRow, UINT _amountCol, int _flipTime,
			const Shape& _collisionBox, float _centerX = 0.0f, float _centerY = 0.0f);
		Sprite(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height, UINT _amountRow, UINT _amountCol, int _flipTime,
			float _centerX = 0.0f, float _centerY = 0.0f, float _alphaThreshold = 0.0f);

		template<sprite_element_type T> void init(T&& _spriteZ);
		template<sprite_element_type T, sprite_element_type... TNext> void init(int _flipTime, T&& _spriteZ, TNext&&... _spriteZNext);
		void operator= (Sprite& _sprite)const = delete;

		void setSpeed(int _speed);
		template<sprite_element_type T> void push(T&& _spriteZ);
		template<sprite_element_type T, sprite_element_type... TNext> void push(T&& _spriteZ, TNext&&... _spriteZNext);

		void setStartingIndex(UINT _index);

		UINT getWidth()const { return width_; }
		UINT getHeight()const { return height_; }
		std::pair<UINT, UINT> getSize()const { return { width_, height_ }; }
		UINT getRowCount()const { return rowCount_; }
		UINT getColCount()const { return colCount_; }
		std::pair<UINT, UINT> getRowColCount()const { return { rowCount_, colCount_ }; }
		UINT getCount()const { return bodyList.size(); }
		int getFlipTime()const { return flipTime_; }
		float getCenterX()const { return centerX_; }
		float getCenterY()const { return centerY_; }
		std::pair<float, float> getCenter()const { return { centerX_, centerY_ }; }

		SpriteElement* operator[](UINT _index) { return getState(_index); }
		SpriteElement* getState(UINT _index);
		SpriteElement* getFrameState(long long _frameTick)const;
		SpriteElement* getFrameState(long double _frameTick)const;
		SpriteElement* getFrameState(UINT _startIndex, long long _frameTick)const;
		SpriteElement* getFrameState(UINT _startIndex, long double _frameTick)const;
		UINT getFrameStateIndex(long long _frameTick)const;
		UINT getFrameStateIndex(long double _frameTick)const;
		UINT getFrameStateIndex(UINT _startIndex, long long _frameTick)const;
		UINT getFrameStateIndex(UINT _startIndex, long double _frameTick)const;
	private:
		UINT rowCount_, colCount_;
		UINT width_, height_;
		float centerX_, centerY_;
		int flipTime_;
		UINT lastSetIndex_ = 0;
	};
}

#include "sprite.inl"