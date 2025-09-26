#include "paint_layer.h"
#include "includes.h"

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
		HRESULT hr = pMainRenderTarget->CreateCompatibleRenderTarget(
			D2D1::SizeF((FLOAT)_width, (FLOAT)_height),
			&pBitmapRenderTarget_
		);
		if (FAILED(hr))
			ERRORWINDOW("Failed to create compatible render target");
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

	void PaintLayer::beginDraw()
	{
		pBitmapRenderTarget_->BeginDraw();
	}

	Bitmap* PaintLayer::endDraw()
	{
		pBitmapRenderTarget_->EndDraw();
		ID2D1Bitmap* pBitmap = nullptr;
		HRESULT hr = pBitmapRenderTarget_->GetBitmap(&pBitmap);
		if (FAILED(hr))
		{
			ERRORWINDOW("Failed to get bitmap from bitmap render target");
			return nullptr;
		}
		return new Bitmap(pBitmap);
	}

	void PaintLayer::paintBitmap(const Bitmap& _bitmap, Transform _transform, float _alpha)
	{
		pBitmapRenderTarget_->SetTransform(_transform.matrix);
		pBitmapRenderTarget_->DrawBitmap(_bitmap.d2dBitmap_, D2D1::RectF(0, 0, (float)_bitmap.getWidth(), (float)_bitmap.getHeight()), _alpha);
	}

	void PaintLayer::paintShape(const Shape& _shape, Transform _transform, float _alpha)
	{
		// WIP
	}
}