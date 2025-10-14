#pragma once
#include "../framework.h"
#include "../suku_foundation/includes.h"
#include "../suku_draw/includes.h"

namespace suku
{
	// Macro for initializing a sprite
#define SPR_INIT(sprPointer, func) static Sprite* temp_##sprPointer = sprPointer = new Sprite func;

	class SpriteZero;
	class BitmapSpriteZero;
	class ShapeSpriteZero;
	class Sprite;

	class CollisionBox;
	class BitmapCollisionBox;
	class ShapeCollisionBox;
	class Transform;
	class Shape;
	class Bitmap;

	class SpriteZero
	{
	public:
		UINT height, width;
		float centerX, centerY;
		CollisionBox* hitArea;
		SpriteZero() : height(0), width(0), centerX(0), centerY(0), hitArea(nullptr) {}

		virtual void paint(float _x, float _y,
			float _xscale = 1.0, float _yscale = 1.0, float _alpha = 1.0, float _angle = 0.0) = 0;
		virtual void paint(float _x, float _y,
			Transform _transform, float _alpha = 1.0) = 0;
		virtual void paint(Transform _transform, float _alpha = 1.0) = 0;
		bool isCrashed(Transform _transform, const SpriteZero& _other, Transform _otherTransform)const;
		bool isCrashed(Transform _transform, const SpriteZero* _other, Transform _otherTransform)const;
	};

	class BitmapSpriteZero :public SpriteZero
	{
	public:
		BitmapSpriteZero();
		BitmapSpriteZero(const Shape& _collisionBox, const Bitmap& _bitmap, float _centerX = 0.0f, float _centerY = 0.0f);
		BitmapSpriteZero(UINT _width, UINT _height, const Shape& _collisionBox, float _centerX = 0.0f, float _centerY = 0.0f);
		BitmapSpriteZero(UINT _width, UINT _height, const BitmapCollisionBox& _collisionBox,
			float _centerX = 0, float _centerY = 0, String _path = String());

		BitmapSpriteZero(String _path, const Shape& _collisionBox, float _centerX = 0.0f, float _centerY = 0.0f);
		BitmapSpriteZero(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height,
			const Shape& _collisionBox, float _centerX = 0.0f, float _centerY = 0.0f);


		BitmapSpriteZero(String _path, float _centerX = 0, float _centerY = 0, float _alphaThreshold = 0.0f);
		BitmapSpriteZero(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height,
			float _centerX = 0.0f, float _centerY = 0.0f, float _alphaThreshold = 0.0f);


		void catchBitmap(String _path, UINT _startX = 0, UINT _startY = 0);
		void catchBitmapAndSize(String _path);

		virtual void paint(float _x, float _y,
			float _xScale = 1.0, float _yScale = 1.0, float _alpha = 1.0, float _angle = 0.0) override;
		virtual void paint(float _x, float _y,
			Transform _transform, float _alpha = 1.0) override;
		virtual void paint(Transform _transform, float _alpha = 1.0) override;

	private:
		Bitmap* pBitmap_;
	};

	class ShapeSpriteZero :public SpriteZero
	{
	public:
		Shape shape;
		ID2D1Brush* fillBrush;
		ID2D1Brush* outlineBrush;
		ID2D1StrokeStyle* outlineStrokeStyle;
		float outlineWidth;
		ShapeSpriteZero(const Shape& _shape, ID2D1Brush* _fillBrush = nullptr,
			ID2D1Brush* _outlineBrush = nullptr, float _outlineWidth = 1.0f, ID2D1StrokeStyle* _outlineStrokeStyle = nullptr);
		ShapeSpriteZero(const Shape& _shape, const Color& _fillColor);
		ShapeSpriteZero(const Shape& _shape, const Color& _fillColor,
			const Color& _outlineColor, float _outlineWidth = 1.0f, ID2D1StrokeStyle* _outlineStrokeStyle = nullptr);
		~ShapeSpriteZero();
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
		std::vector<SpriteZero*> bodyList;
		Sprite();
		template<typename SprZ> Sprite(const SprZ& _spriteZ);
		template<typename SprZ, typename... SprZNext> Sprite(int _flipTime, const SprZ& _spriteZ, const SprZNext&... _spriteZNext);

		//Load Sprite Directly from the long sprite bitmap; width and height will be auto calculated.
		Sprite(String _path, UINT _amount, int _flipTime,
			const Shape& _collisionBox, float _centerX = 0.0f, float _centerY = 0.0f);
		Sprite(String _path, UINT _amount, int _flipTime,
			float _centerX = 0.0f, float _centerY = 0.0f, float _alphaThreshold = 0.0f);
		//Load Sprite Directly from the long sprite bitmap, with given starting position and size.
		Sprite(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height, UINT _amount, int _flipTime,
			const Shape& _collisionBox, float _centerX = 0.0f, float _centerY = 0.0f);
		Sprite(String _path, UINT _startX, UINT _startY, UINT _width, UINT _height, UINT _amount, int _flipTime,
			float _centerX = 0.0f, float _centerY = 0.0f, float _alphaThreshold = 0.0f);

		template<typename SprZ> void init(const SprZ& _spriteZ);
		template<typename SprZ, typename... SprZNext> void init(int _flipTime, const SprZ& _spriteZ, const SprZNext&... _spriteZNext);
		void operator= (Sprite& _sprite)const = delete;

		void setSpeed(int _speed);
		template<typename SprZ> void push(const SprZ& _spriteZ);
		template<typename SprZ, typename... SprZNext> void push(const SprZ& _spriteZ, const SprZNext&... _spriteZNext);
		SpriteZero* getState(int _wp);
	private:
		int flipTime_;
	};
}

#include "sprite.inl"