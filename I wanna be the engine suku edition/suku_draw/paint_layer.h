#pragma once
#include <d2d1_1.h>
#include <Windows.h>
#include "color.h"
#include <stack>
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
		PaintLayer() = default;
		PaintLayer(UINT _width, UINT _height) { newLayer(_width, _height); }
		void newLayer(UINT _width, UINT _height);

		UINT getWidth() { return width_; }
		UINT getHeight() { return height_; }
		std::pair<UINT, UINT> getSize() { return { width_, height_ }; }

		void beginDraw();
		RenderBitmap endDraw();
		void clear();
		void clear(Color _backgroundcolor);
		void drawBitmap(Bitmap& _bitmap, Transform _transform, float _alpha = 1.0f);
		void drawBitmap(RenderBitmap& _bitmap, Transform _transform, float _alpha = 1.0f);
		void drawShape(const Shape& _shape, Transform _transform, 
			const ComPtr<ID2D1Brush>& _fillBrush, const ComPtr<ID2D1Brush>& _outlineBrush, float _outlineWidth = 1.0,
			const ComPtr<ID2D1StrokeStyle>& outlineStrokeStyle = nullptr);
	private:
		static std::stack<ComPtr<ID2D1Bitmap1>> CurrentLayerStateStack_;
		ComPtr<ID2D1Bitmap1> pLayerBitmap_;
		UINT width_;
		UINT height_;
	};
}