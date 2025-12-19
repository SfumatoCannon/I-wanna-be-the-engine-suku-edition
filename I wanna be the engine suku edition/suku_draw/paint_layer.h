#pragma once
#include "../framework.h"
#include "color.h"
#include <wrl/client.h>

namespace suku
{
    using Microsoft::WRL::ComPtr;
	class Bitmap;
	class RenderBitmap;
	class Shape;
	class Transform;

	class PaintLayer
	{
	public:
		void newLayer(UINT _width, UINT _height);
		void beginDraw();
		RenderBitmap endDraw();
		void clear(Color _backgroundcolor = Color::WHITE());
		void drawBitmap(Bitmap& _bitmap, Transform _transform, float _alpha = 1.0f);
		void drawShape(const Shape& _shape, Transform _transform, 
			const ComPtr<ID2D1Brush>& _fillBrush, const ComPtr<ID2D1Brush>& _outlineBrush, float _outlineWidth = 1.0,
			const ComPtr<ID2D1StrokeStyle>& outlineStrokeStyle = nullptr);
	private:
		ComPtr<ID2D1BitmapRenderTarget> pBitmapRenderTarget_;
		UINT width_;
		UINT height_;
	};
}