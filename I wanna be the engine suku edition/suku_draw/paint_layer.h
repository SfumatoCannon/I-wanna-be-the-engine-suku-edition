#pragma once
#include "../framework.h"
#include "color.h"

namespace suku
{
	class Bitmap;
	class Shape;
	class Transform;

	class PaintLayer
	{
	private:
		ID2D1BitmapRenderTarget* pBitmapRenderTarget_;
	public:
		void newLayer(UINT _width, UINT _height);
		void clear(Color _backgroundcolor = Color::WHITE());
		void beginDraw();
		Bitmap* endDraw();
		void paintBitmap(const Bitmap& _bitmap, Transform _transform, float _alpha = 1.0f);
		void paintShape(const Shape& _shape, Transform _transform, float _alpha = 1.0f);
	};
}