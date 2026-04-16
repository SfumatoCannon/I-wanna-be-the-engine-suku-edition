#include "pch.h"
#include "paint_layer.h"
#include <utility>
#include "draw_core.h"
#include "shape.h"
#include "suku_foundation/suku_string.h"
#include "suku_foundation/message.h"
#include "bitmap.h"

namespace suku
{
	using namespace suku::graphics;

	std::stack<ComPtr<ID2D1Bitmap1>> PaintLayer::currentLayerStateStack_;

	void PaintLayer::newLayer(UINT _width, UINT _height)
	{
		pLayerBitmap_ = createLayerBitmap(_width, _height);
	}

	void PaintLayer::beginDraw()
	{
		currentLayerStateStack_.push(pLayerBitmap_);
		pD2DContext->SetTarget(pLayerBitmap_.Get());
	}

	RenderBitmap PaintLayer::endDraw()
	{
		if (currentLayerStateStack_.empty())
		{
			WARNINGWINDOW("Couldn't found layer data in drawing stack \"PaintLayer::currentLayerStateStack_\".\n\
				Unexpected error may happen.");
			pD2DContext->SetTarget(pScreenTargetBitmap.Get());
		}
		else if (currentLayerStateStack_.top() != pLayerBitmap_)
		{
			WARNINGWINDOW("The top layer data in drawing stack \"PaintLayer::currentLayerStateStack_\" is not matching the current layer.\n\
				Unexpected error may happen.");
		}
		else
		{
			currentLayerStateStack_.pop();
		}

		if (currentLayerStateStack_.empty())
			pD2DContext->SetTarget(pScreenTargetBitmap.Get());
		else
			pD2DContext->SetTarget(currentLayerStateStack_.top().Get());
		return RenderBitmap(pLayerBitmap_);
	}

	void PaintLayer::clear()
	{
		beginDraw();
		pD2DContext->Clear(D2D1::ColorF(D2D1::ColorF::White));
		endDraw();
	}

	void PaintLayer::clear(Color _backgroundcolor)
	{
		beginDraw();
		pD2DContext->Clear(D2D1::ColorF(_backgroundcolor.r() / 255.0f,
			_backgroundcolor.g() / 255.0f,
			_backgroundcolor.b() / 255.0f,
			_backgroundcolor.alpha));
		endDraw();
	}

	void PaintLayer::drawBitmap(Bitmap& _bitmap, float _x, float _y, float _alpha)
	{
		beginDraw();
		_bitmap.paint(_x, _y, _alpha);
			//pD2DContext->SetTransform(translation(_x, _y).matrix);
			//pD2DContext->DrawBitmap(_bitmap.getD2DBitmap().Get(),
			//	D2D1::RectF(0, 0, (float)_bitmap.getWidth(), (float)_bitmap.getHeight()), _alpha);
		endDraw();
	}

	void PaintLayer::drawBitmap(Bitmap& _bitmap, float _x, float _y, Transform _transform, float _alpha)
	{
		beginDraw();
		_bitmap.paint(_x, _y, _transform, _alpha);
			//pD2DContext->SetTransform((translation(_x, _y) + _transform).matrix);
			//pD2DContext->DrawBitmap(_bitmap.getD2DBitmap().Get(),
			//	D2D1::RectF(0, 0, (float)_bitmap.getWidth(), (float)_bitmap.getHeight()), _alpha);
		endDraw();
	}

	void PaintLayer::drawBitmap(Bitmap& _bitmap, Transform _transform, float _alpha)
	{
		beginDraw();
		_bitmap.paint(_transform, _alpha);
			//pD2DContext->SetTransform(_transform.matrix);
			//pD2DContext->DrawBitmap(_bitmap.getD2DBitmap().Get(),
			//	D2D1::RectF(0, 0, (float)_bitmap.getWidth(), (float)_bitmap.getHeight()), _alpha);
		endDraw();
	}

	void PaintLayer::drawBitmap(RenderBitmap& _bitmap, float _x, float _y, float _alpha)
	{
		beginDraw();
		_bitmap.paint(_x, _y, _alpha);
		endDraw();
	}

	void PaintLayer::drawBitmap(RenderBitmap& _bitmap, float _x, float _y, Transform _transform, float _alpha)
	{
		beginDraw();
		_bitmap.paint(_x, _y, _transform, _alpha);
		endDraw();
	}

	void PaintLayer::drawBitmap(RenderBitmap& _bitmap, Transform _transform, float _alpha)
	{
		beginDraw();
		_bitmap.paint(_transform, _alpha);
		endDraw();
	}

	void PaintLayer::drawShape(const Shape& _shape, Transform _transform,
		const ComPtr<ID2D1Brush>& _fillBrush, const ComPtr<ID2D1Brush>& _outlineBrush, float _outlineWidth, const ComPtr<ID2D1StrokeStyle>& outlineStrokeStyle)
	{
		beginDraw();
		pD2DContext->SetTransform(_transform.matrix);
		if (_outlineBrush)
			pD2DContext->DrawGeometry(_shape.currentGeometry.Get(), _outlineBrush.Get(), _outlineWidth, outlineStrokeStyle.Get());
		if (_fillBrush)
			pD2DContext->FillGeometry(_shape.currentGeometry.Get(), _fillBrush.Get());
		endDraw();
	}
}