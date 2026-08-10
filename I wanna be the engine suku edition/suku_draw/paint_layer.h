#pragma once
#include <d2d1_1.h>
#include <Windows.h>
#include "color.h"
#include <stack>
#include <wrl/client.h>
#include <suku_draw/transform.h>
#include <vector>

namespace suku
{
    using Microsoft::WRL::ComPtr;
	class Bitmap;
	class RenderBitmap;
	class Shape;

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

		void pushBasicTransform(Transform _transform);
		void popBasicTransform();
		std::vector<Transform> getBasicTransformStack();
		Transform getBasicTransform();

		void drawBitmap(Bitmap& _bitmap, float _x, float _y, float _alpha = 1.0f);
		void drawBitmap(Bitmap& _bitmap, float _x, float _y, Transform _transform, float _alpha = 1.0f);
		void drawBitmap(Bitmap& _bitmap, Transform _transform, float _alpha = 1.0f);
		void drawBitmap(Bitmap& _bitmap, float _x, float _y, float _partX, float _partY, float _partWidth, float _partHeight, float _alpha = 1.0f);
		void drawBitmap(Bitmap& _bitmap, float _x, float _y, float _partX, float _partY, float _partWidth, float _partHeight, Transform _transform, float _alpha = 1.0f);
		void drawBitmap(Bitmap& _bitmap, float _partX, float _partY, float _partWidth, float _partHeight, Transform _transform, float _alpha = 1.0f);
		void drawBitmap(RenderBitmap& _bitmap, float _x, float _y, float _alpha = 1.0f);
		void drawBitmap(RenderBitmap& _bitmap, float _x, float _y, Transform _transform, float _alpha = 1.0f);
		void drawBitmap(RenderBitmap& _bitmap, Transform _transform, float _alpha = 1.0f);
		void drawShape(const Shape& _shape, Transform _transform, 
			const ComPtr<ID2D1Brush>& _fillBrush, const ComPtr<ID2D1Brush>& _outlineBrush, float _outlineWidth = 1.0,
			const ComPtr<ID2D1StrokeStyle>& outlineStrokeStyle = nullptr);

		static PaintLayer* getCurrentPaintLayer();
	private:
		static std::stack<PaintLayer*> currentPaintLayerPtrStack_;
		static std::stack<ComPtr<ID2D1Bitmap1>> currentLayerStateStack_;
		std::vector<Transform> basicTransformStack_;
		ComPtr<ID2D1Bitmap1> pLayerBitmap_;
		UINT width_ = 0;
		UINT height_ = 0;
	};
}