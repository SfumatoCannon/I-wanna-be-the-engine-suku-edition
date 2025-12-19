#include "paint_layer.h"
#include "bitmap.h"
#include "draw_core.h"
#include "shape.h"
#include "transform.h"

namespace suku
{
	void PaintLayer::newLayer(UINT _width, UINT _height)
	{
		if (!pMainRenderTarget)
		{
			ERRORWINDOW("Main render target is null");
			return;
		}
		if (pBitmapRenderTarget_)
		{
			release_safe(pBitmapRenderTarget_);
		}
		width_ = _width;
		height_ = _height;
		ComPtr<ID2D1BitmapRenderTarget> tmpRenderTarget;
		HRESULT hr = pMainRenderTarget->CreateCompatibleRenderTarget(
			D2D1::SizeF((FLOAT)_width, (FLOAT)_height),
			tmpRenderTarget.GetAddressOf()
		);
		if (FAILED(hr))
		{
			ERRORWINDOW("Failed to create compatible render target");
			return;
		}
		pBitmapRenderTarget_ = std::move(tmpRenderTarget);
	}

	void PaintLayer::beginDraw()
	{
		pBitmapRenderTarget_->BeginDraw();
	}

	RenderBitmap PaintLayer::endDraw()
	{
		pBitmapRenderTarget_->EndDraw();
		ComPtr<ID2D1Bitmap> pBitmap;
		HRESULT hr = pBitmapRenderTarget_->GetBitmap(pBitmap.GetAddressOf());
		if (FAILED(hr))
		{
			ERRORWINDOW("Failed to get bitmap from bitmap render target");
			return RenderBitmap();
		}
		return RenderBitmap(pBitmap);
	}

	void PaintLayer::clear(Color _backgroundcolor)
	{
		pBitmapRenderTarget_->Clear(D2D1::ColorF(
			_backgroundcolor.r(),
			_backgroundcolor.g(),
			_backgroundcolor.b(),
			_backgroundcolor.alpha
		));
	}

	void PaintLayer::drawBitmap(Bitmap& _bitmap, Transform _transform, float _alpha)
	{
		pBitmapRenderTarget_->SetTransform(_transform.matrix);
		pBitmapRenderTarget_->DrawBitmap(_bitmap.getD2DBitmap().Get(),
			D2D1::RectF(0, 0, (float)_bitmap.getWidth(), (float)_bitmap.getHeight()), _alpha);
	}

	void PaintLayer::drawBitmap(RenderBitmap& _bitmap, Transform _transform, float _alpha)
	{
		pBitmapRenderTarget_->SetTransform(_transform.matrix);
		pBitmapRenderTarget_->DrawBitmap(_bitmap.getD2DBitmap().Get(),
			D2D1::RectF(0, 0, (float)_bitmap.getWidth(), (float)_bitmap.getHeight()), _alpha);
	}

	void PaintLayer::drawShape(const Shape& _shape, Transform _transform,
		const ComPtr<ID2D1Brush>& _fillBrush, const ComPtr<ID2D1Brush>& _outlineBrush, float _outlineWidth, const ComPtr<ID2D1StrokeStyle>& outlineStrokeStyle)
	{
		pBitmapRenderTarget_->SetTransform(_transform.matrix);
		if (_outlineBrush)
			pBitmapRenderTarget_->DrawGeometry(_shape.currentGeometry.Get(), _outlineBrush.Get(), _outlineWidth, outlineStrokeStyle.Get());
		if (_fillBrush)
			pBitmapRenderTarget_->FillGeometry(_shape.currentGeometry.Get(), _fillBrush.Get());
	}
}