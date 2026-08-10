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

	std::stack<PaintLayer*> PaintLayer::currentPaintLayerPtrStack_;
	std::stack<ComPtr<ID2D1Bitmap1>> PaintLayer::currentLayerStateStack_;

	void PaintLayer::newLayer(UINT _width, UINT _height)
	{
		pLayerBitmap_ = createLayerBitmap(_width, _height);
	}

	void PaintLayer::beginDraw()
	{
		currentLayerStateStack_.push(pLayerBitmap_);
		currentPaintLayerPtrStack_.push(this);
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
			currentPaintLayerPtrStack_.pop();
		}

		if (currentLayerStateStack_.empty())
			pD2DContext->SetTarget(pScreenTargetBitmap.Get());
		else
			pD2DContext->SetTarget(currentLayerStateStack_.top().Get());
		return RenderBitmap(pLayerBitmap_);
	}

	void PaintLayer::clear()
	{
		if (currentPaintLayerPtrStack_.top() != this)
		{
			beginDraw();
			pD2DContext->Clear(D2D1::ColorF(D2D1::ColorF::White));
			endDraw();
		}
		else
		{
			pD2DContext->Clear(D2D1::ColorF(D2D1::ColorF::White));
		}
	}

	void PaintLayer::clear(Color _backgroundcolor)
	{
		if (currentPaintLayerPtrStack_.top() != this)
		{
			beginDraw();
			pD2DContext->Clear(D2D1::ColorF(_backgroundcolor.r() / 255.0f,
				_backgroundcolor.g() / 255.0f,
				_backgroundcolor.b() / 255.0f,
				_backgroundcolor.alpha));
			endDraw();
		}
		else
		{
			pD2DContext->Clear(D2D1::ColorF(D2D1::ColorF::White));
		}
	}

	void PaintLayer::setBasicTransform(Transform _transform)
	{
		basicTransform_ = _transform;
	}

	Transform PaintLayer::getBasicTransform()
	{
		return basicTransform_;
	}

	void PaintLayer::drawBitmap(Bitmap& _bitmap, float _x, float _y, float _alpha)
	{
		if (currentPaintLayerPtrStack_.top() != this)
		{
			beginDraw();
			_bitmap.paint(_x, _y, _alpha);
			endDraw();
		}
		else
		{
			_bitmap.paint(_x, _y, _alpha);
		}
	}

	void PaintLayer::drawBitmap(Bitmap& _bitmap, float _x, float _y, Transform _transform, float _alpha)
	{
		if (currentPaintLayerPtrStack_.top() != this)
		{
			beginDraw();
			_bitmap.paint(_x, _y, _transform, _alpha);
			endDraw();
		}
		else
		{
			_bitmap.paint(_x, _y, _transform, _alpha);
		}
	}

	void PaintLayer::drawBitmap(Bitmap& _bitmap, Transform _transform, float _alpha)
	{
		if (currentPaintLayerPtrStack_.top() != this)
		{
			beginDraw();
			_bitmap.paint(_transform, _alpha);
			endDraw();
		}
		else
		{
			_bitmap.paint(_transform, _alpha);
		}
	}

	void PaintLayer::drawBitmap(Bitmap& _bitmap, float _x, float _y, float _partX, float _partY, float _partWidth, float _partHeight, float _alpha)
	{
		if (currentPaintLayerPtrStack_.top() != this)
		{
			beginDraw();
			_bitmap.paint(_x, _y, _partX, _partY, _partWidth, _partHeight, _alpha);
			endDraw();
		}
		else
		{
			_bitmap.paint(_x, _y, _partX, _partY, _partWidth, _partHeight, _alpha);
		}
	}

	void PaintLayer::drawBitmap(Bitmap& _bitmap, float _x, float _y, float _partX, float _partY, float _partWidth, float _partHeight, Transform _transform, float _alpha)
	{
		if (currentPaintLayerPtrStack_.top() != this)
		{
			beginDraw();
			_bitmap.paint(_x, _y, _partX, _partY, _partWidth, _partHeight, _transform, _alpha);
			endDraw();
		}
		else
		{
			_bitmap.paint(_x, _y, _partX, _partY, _partWidth, _partHeight, _transform, _alpha);
		}
	}

	void PaintLayer::drawBitmap(Bitmap& _bitmap, float _partX, float _partY, float _partWidth, float _partHeight, Transform _transform, float _alpha)
	{
		if (currentPaintLayerPtrStack_.top() != this)
		{
			beginDraw();
			_bitmap.paint(_partX, _partY, _partWidth, _partHeight, _transform, _alpha);
			endDraw();
		}
		else
		{
			_bitmap.paint(_partX, _partY, _partWidth, _partHeight, _transform, _alpha);
		}
	}

	void PaintLayer::drawBitmap(RenderBitmap& _bitmap, float _x, float _y, float _alpha)
	{
		if (currentPaintLayerPtrStack_.top() != this)
		{
			beginDraw();
			_bitmap.paint(_x, _y, _alpha);
			endDraw();
		}
		else
		{
			_bitmap.paint(_x, _y, _alpha);
		}
	}

	void PaintLayer::drawBitmap(RenderBitmap& _bitmap, float _x, float _y, Transform _transform, float _alpha)
	{
		if (currentPaintLayerPtrStack_.top() != this)
		{
			beginDraw();
			_bitmap.paint(_x, _y, _transform, _alpha);
			endDraw();
		}
		else
		{
			_bitmap.paint(_x, _y, _transform, _alpha);
		}
	}

	void PaintLayer::drawBitmap(RenderBitmap& _bitmap, Transform _transform, float _alpha)
	{
		if (currentPaintLayerPtrStack_.top() != this)
		{
			beginDraw();
			_bitmap.paint(_transform, _alpha);
			endDraw();
		}
		else
		{
			_bitmap.paint(_transform, _alpha);
		}
	}

	void PaintLayer::drawShape(const Shape& _shape, Transform _transform,
		const ComPtr<ID2D1Brush>& _fillBrush, const ComPtr<ID2D1Brush>& _outlineBrush, float _outlineWidth, const ComPtr<ID2D1StrokeStyle>& outlineStrokeStyle)
	{
		if (currentPaintLayerPtrStack_.top() != this)
		{
			beginDraw();
			pD2DContext->SetTransform((this->basicTransform_ + _transform).matrix);
			if (_outlineBrush)
				pD2DContext->DrawGeometry(_shape.currentGeometry.Get(), _outlineBrush.Get(), _outlineWidth, outlineStrokeStyle.Get());
			if (_fillBrush)
				pD2DContext->FillGeometry(_shape.currentGeometry.Get(), _fillBrush.Get());
			endDraw();
		}
		else
		{
			pD2DContext->SetTransform((this->basicTransform_ + _transform).matrix);
			if (_outlineBrush)
				pD2DContext->DrawGeometry(_shape.currentGeometry.Get(), _outlineBrush.Get(), _outlineWidth, outlineStrokeStyle.Get());
			if (_fillBrush)
				pD2DContext->FillGeometry(_shape.currentGeometry.Get(), _fillBrush.Get());
		}
	}

	PaintLayer* PaintLayer::getCurrentPaintLayer()
	{
		if (currentPaintLayerPtrStack_.empty())
			return nullptr;
		return currentPaintLayerPtrStack_.top();
	}
}