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

	void Effect::drawEffect() const
	{
		graphics::pD2DContext->SetTransform(D2D1::Matrix3x2F::Identity());
		graphics::pD2DContext->DrawImage(pEffect_.Get());
	}

	EffectTransform::EffectTransform()
	{
		graphics::pD2DContext->CreateEffect(CLSID_D2D12DAffineTransform, &pEffect_);
	}

	EffectTransform::EffectTransform(Transform _transform, EffectScaleMode _scaleMode, float _sharpness)
	{
		graphics::pD2DContext->CreateEffect(CLSID_D2D12DAffineTransform, &pEffect_);
		pEffect_->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, _transform.matrix);
		pEffect_->SetValue(D2D1_2DAFFINETRANSFORM_PROP_INTERPOLATION_MODE, static_cast<D2D1_SCALE_INTERPOLATION_MODE>(_scaleMode));
		pEffect_->SetValue(D2D1_2DAFFINETRANSFORM_PROP_SHARPNESS, _sharpness);
	}

	EffectTransform::EffectTransform(EffectScaleMode _scaleMode, float _sharpness)
	{
		graphics::pD2DContext->CreateEffect(CLSID_D2D12DAffineTransform, &pEffect_);
		pEffect_->SetValue(D2D1_2DAFFINETRANSFORM_PROP_INTERPOLATION_MODE, static_cast<D2D1_SCALE_INTERPOLATION_MODE>(_scaleMode));
		pEffect_->SetValue(D2D1_2DAFFINETRANSFORM_PROP_SHARPNESS, _sharpness);
	}

	void EffectTransform::setTransform(Transform _transform)
	{
		transform_ = _transform;
		pEffect_->SetValue(D2D1_2DAFFINETRANSFORM_PROP_TRANSFORM_MATRIX, _transform.matrix);
	}

	void EffectTransform::setScaleMode(EffectScaleMode _scaleMode)
	{
		pEffect_->SetValue(D2D1_2DAFFINETRANSFORM_PROP_INTERPOLATION_MODE, static_cast<D2D1_SCALE_INTERPOLATION_MODE>(_scaleMode));
	}

	void EffectTransform::setSharpness(float _sharpness)
	{
		pEffect_->SetValue(D2D1_2DAFFINETRANSFORM_PROP_SHARPNESS, _sharpness);
	}
}