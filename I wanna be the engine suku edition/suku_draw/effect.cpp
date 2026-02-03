#include "pch.h"
#include "effect.h"
#include "draw_core.h"
#include "bitmap.h"
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
}