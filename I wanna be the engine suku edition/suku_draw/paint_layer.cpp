#include "pch.h"
#include "paint_layer.h"
#include <utility>
#include "draw_core.h"
#include "shape.h"
#include "suku_foundation/string.h"
#include "suku_foundation/message.h"
#include "bitmap.h"

namespace suku
{
	using namespace suku::graphics;

	void PaintLayer::newLayer(UINT _width, UINT _height)
	{
		pLayerBitmap_ = createLayerBitmap(_width, _height);
	}

	void PaintLayer::beginDraw()
	{
		pD2DContext->SetTarget(pLayerBitmap_.Get());
	}

	RenderBitmap PaintLayer::endDraw()
	{
		pD2DContext->SetTarget(pScreenTargetBitmap.Get());
		return RenderBitmap(pLayerBitmap_);
	}

	void PaintLayer::clear()
	{
		pD2DContext->Clear(D2D1::ColorF(D2D1::ColorF::White));
	}

	void PaintLayer::clear(Color _backgroundcolor)
	{
		pD2DContext->Clear(D2D1::ColorF(_backgroundcolor.r() / 255.0f,
			_backgroundcolor.g() / 255.0f,
			_backgroundcolor.b() / 255.0f,
			_backgroundcolor.alpha));
	}

	void PaintLayer::drawBitmap(Bitmap& _bitmap, Transform _transform, float _alpha)
	{
		pD2DContext->SetTransform(_transform.matrix);
		pD2DContext->DrawBitmap(_bitmap.getD2DBitmap().Get(),
			D2D1::RectF(0, 0, (float)_bitmap.getWidth(), (float)_bitmap.getHeight()), _alpha);
	}

	void PaintLayer::drawBitmap(RenderBitmap& _bitmap, Transform _transform, float _alpha)
	{
		pD2DContext->SetTransform(_transform.matrix);
		pD2DContext->DrawBitmap(_bitmap.getD2DBitmap().Get(),
			D2D1::RectF(0, 0, (float)_bitmap.getWidth(), (float)_bitmap.getHeight()), _alpha);
	}

	void PaintLayer::drawShape(const Shape& _shape, Transform _transform,
		const ComPtr<ID2D1Brush>& _fillBrush, const ComPtr<ID2D1Brush>& _outlineBrush, float _outlineWidth, const ComPtr<ID2D1StrokeStyle>& outlineStrokeStyle)
	{
		pD2DContext->SetTransform(_transform.matrix);
		if (_outlineBrush)
			pD2DContext->DrawGeometry(_shape.currentGeometry.Get(), _outlineBrush.Get(), _outlineWidth, outlineStrokeStyle.Get());
		if (_fillBrush)
			pD2DContext->FillGeometry(_shape.currentGeometry.Get(), _fillBrush.Get());
	}
}