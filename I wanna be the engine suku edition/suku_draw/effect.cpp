#include "pch.h"
#include "effect.h"
#include "draw_core.h"
#include "bitmap.h"
#include <d2d1_1.h>
#include <d2d1effects.h>


namespace suku
{
	void Effect::setInput(Bitmap& _bitmap, int _index)
	{
		pEffect_->SetInput(_index, _bitmap.getD2DBitmap().Get());
	}

	void Effect::setInput(RenderBitmap& _renderBitmap, int _index)
	{
		pEffect_->SetInput(_index, _renderBitmap.getD2DBitmap().Get());
	}

	void Effect::setInput(const Effect& _effect, int _index)
	{
		pEffect_->SetInputEffect(_index, _effect.pEffect_.Get());
	}

	void Effect::paint() const
	{
		graphics::setPaintingTransform(Transform::Identity());
		graphics::drawEffect(pEffect_);
	}

	EffectTransform::EffectTransform()
	{
		graphics::pD2DContext->CreateEffect(CLSID_D2D12DAffineTransform, &pEffect_);
	}

	EffectTransform::EffectTransform(Transform _transform, ScaleMode _scaleMode, float _sharpness)
	{
		graphics::pD2DContext->CreateEffect(CLSID_D2D12DAffineTransform, &pEffect_);
		pEffect_->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, _transform.matrix);
		pEffect_->SetValue(D2D1_2DAFFINETRANSFORM_PROP_INTERPOLATION_MODE, 
			ScaleModeTranslator::toNative<D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE>(_scaleMode));
		pEffect_->SetValue(D2D1_2DAFFINETRANSFORM_PROP_SHARPNESS, _sharpness);
	}

	EffectTransform::EffectTransform(ScaleMode _scaleMode, float _sharpness)
	{
		graphics::pD2DContext->CreateEffect(CLSID_D2D12DAffineTransform, &pEffect_);
		pEffect_->SetValue(D2D1_2DAFFINETRANSFORM_PROP_INTERPOLATION_MODE, 
			ScaleModeTranslator::toNative<D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE>(_scaleMode));
		pEffect_->SetValue(D2D1_2DAFFINETRANSFORM_PROP_SHARPNESS, _sharpness);
	}

	void EffectTransform::setTransform(Transform _transform)
	{
		transform_ = _transform;
		pEffect_->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, _transform.matrix);
	}

	void EffectTransform::setScaleMode(ScaleMode _scaleMode)
	{
		pEffect_->SetValue(D2D1_2DAFFINETRANSFORM_PROP_INTERPOLATION_MODE, 
			ScaleModeTranslator::toNative<D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE>(_scaleMode));
	}

	void EffectTransform::setSharpness(float _sharpness)
	{
		pEffect_->SetValue(D2D1_2DAFFINETRANSFORM_PROP_SHARPNESS, _sharpness);
	}

	EffectOpacityMask::EffectOpacityMask()
	{
		graphics::pD2DContext->CreateEffect(CLSID_D2D1Composite, &pEffect_);
		pEffect_->SetValue(D2D1_COMPOSITE_PROP_MODE, D2D1_COMPOSITE_MODE_DESTINATION_IN);
	}

	EffectOpacityMask::EffectOpacityMask(Bitmap& _maskBitmap)
	{
		graphics::pD2DContext->CreateEffect(CLSID_D2D1Composite, &pEffect_);
		pEffect_->SetValue(D2D1_COMPOSITE_PROP_MODE, D2D1_COMPOSITE_MODE_DESTINATION_IN);
		setMaskBitmap(_maskBitmap);
	}

	EffectOpacityMask::EffectOpacityMask(RenderBitmap& _maskBitmap)
	{
		graphics::pD2DContext->CreateEffect(CLSID_D2D1Composite, &pEffect_);
		pEffect_->SetValue(D2D1_COMPOSITE_PROP_MODE, D2D1_COMPOSITE_MODE_DESTINATION_IN);
		setMaskBitmap(_maskBitmap);
	}

	void EffectOpacityMask::setMaskBitmap(Bitmap& _maskBitmap)
	{
		pEffect_->SetInput(1, _maskBitmap.getD2DBitmap().Get());
	}

	void EffectOpacityMask::setMaskBitmap(RenderBitmap& _maskBitmap)
	{
		pEffect_->SetInput(1, _maskBitmap.getD2DBitmap().Get());
	}

	EffectCut::EffectCut(bool _isAtlasMode) : isAtlasMode_(_isAtlasMode)
	{
		if (_isAtlasMode == false)
		{
			graphics::pD2DContext->CreateEffect(CLSID_D2D1Crop, &pEffect_);
		}
		else
		{
			graphics::pD2DContext->CreateEffect(CLSID_D2D1Atlas, &pEffect_);
		}
	}

	EffectCut::EffectCut(float _x, float _y, float _width, float _height, bool _isAtlasMode)
		: x_(_x), y_(_y), width_(_width), height_(_height), isAtlasMode_(_isAtlasMode)
	{
		if (_isAtlasMode == false)
		{
			graphics::pD2DContext->CreateEffect(CLSID_D2D1Crop, &pEffect_);
			pEffect_->SetValue(D2D1_CROP_PROP_RECT, D2D1_RECT_F{ _x, _y, _x + _width, _y + _height });
		}
		else
		{
			graphics::pD2DContext->CreateEffect(CLSID_D2D1Atlas, &pEffect_);
			pEffect_->SetValue(D2D1_ATLAS_PROP_INPUT_RECT, D2D1_VECTOR_4F{ _x, _y, _x + _width, _y + _height });
		}
	}

	void EffectCut::setCutArea(float _x, float _y, float _width, float _height)
	{
		x_ = _x;
		y_ = _y;
		width_ = _width;
		height_ = _height;
		if (isAtlasMode_ == false)
		{
			pEffect_->SetValue(D2D1_CROP_PROP_RECT, D2D1_RECT_F{ _x, _y, _x + _width, _y + _height });
		}
		else
		{
			pEffect_->SetValue(D2D1_ATLAS_PROP_INPUT_RECT, D2D1_VECTOR_4F{ _x, _y, _x + _width, _y + _height });
		}
	}

	void EffectCut::setCutX(float _x)
	{
		x_ = _x;
		if (isAtlasMode_ == false)
		{
			pEffect_->SetValue(D2D1_CROP_PROP_RECT, D2D1_RECT_F{ x_, y_, x_ + width_, y_ + height_ });
		}
		else
		{
			pEffect_->SetValue(D2D1_ATLAS_PROP_INPUT_RECT, D2D1_VECTOR_4F{ x_, y_, x_ + width_, y_ + height_ });
		}
	}

	void EffectCut::setCutY(float _y)
	{
		y_ = _y;
		if (isAtlasMode_ == false)
		{
			pEffect_->SetValue(D2D1_CROP_PROP_RECT, D2D1_RECT_F{ x_, y_, x_ + width_, y_ + height_ });
		}
		else
		{
			pEffect_->SetValue(D2D1_ATLAS_PROP_INPUT_RECT, D2D1_VECTOR_4F{ x_, y_, x_ + width_, y_ + height_ });
		}
	}

	void EffectCut::setCutWidth(float _width)
	{
		width_ = _width;
		if (isAtlasMode_ == false)
		{
			pEffect_->SetValue(D2D1_CROP_PROP_RECT, D2D1_RECT_F{ x_, y_, x_ + width_, y_ + height_ });
		}
		else
		{
			pEffect_->SetValue(D2D1_ATLAS_PROP_INPUT_RECT, D2D1_VECTOR_4F{ x_, y_, x_ + width_, y_ + height_ });
		}
	}

	void EffectCut::setCutHeight(float _height)
	{
		height_ = _height;
		if (isAtlasMode_ == false)
		{
			pEffect_->SetValue(D2D1_CROP_PROP_RECT, D2D1_RECT_F{ x_, y_, x_ + width_, y_ + height_ });
		}
		else
		{
			pEffect_->SetValue(D2D1_ATLAS_PROP_INPUT_RECT, D2D1_VECTOR_4F{ x_, y_, x_ + width_, y_ + height_ });
		}
	}

	void EffectCut::setCutPosition(float _x, float _y)
	{
		x_ = _x;
		y_ = _y;
		if (isAtlasMode_ == false)
		{
			pEffect_->SetValue(D2D1_CROP_PROP_RECT, D2D1_RECT_F{ x_, y_, x_ + width_, y_ + height_ });
		}
		else
		{
			pEffect_->SetValue(D2D1_ATLAS_PROP_INPUT_RECT, D2D1_VECTOR_4F{ x_, y_, x_ + width_, y_ + height_ });
		}
	}

	void EffectCut::setCutSize(float _width, float _height)
	{
		width_ = _width;
		height_ = _height;
		if (isAtlasMode_ == false)
		{
			pEffect_->SetValue(D2D1_CROP_PROP_RECT, D2D1_RECT_F{ x_, y_, x_ + width_, y_ + height_ });
		}
		else
		{
			pEffect_->SetValue(D2D1_ATLAS_PROP_INPUT_RECT, D2D1_VECTOR_4F{ x_, y_, x_ + width_, y_ + height_ });
		}
	}

	EffectContrast::EffectContrast()
	{
		graphics::pD2DContext->CreateEffect(CLSID_D2D1Contrast, &pEffect_);
	}

	EffectContrast::EffectContrast(float _contrast) : contrast_(_contrast)
	{
		graphics::pD2DContext->CreateEffect(CLSID_D2D1Contrast, &pEffect_);
		pEffect_->SetValue(D2D1_CONTRAST_PROP_CONTRAST, _contrast);
	}

	void EffectContrast::setContrast(float _contrast)
	{
		contrast_ = _contrast;
		pEffect_->SetValue(D2D1_CONTRAST_PROP_CONTRAST, _contrast);
	}
}