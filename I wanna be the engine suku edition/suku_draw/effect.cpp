#include "pch.h"
#include "effect.h"
#include "draw_core.h"
#include "bitmap.h"
#include <d2d1effects.h>

namespace suku
{
	Effect Effect::scaleEffect(float _scaleX, float _scaleY, EffectScaleMode _scaleMode)
	{
		Effect newEffect;
		auto& pNewEffect = newEffect.pEffect_;
		graphics::pD2DContext->CreateEffect(CLSID_D2D1Scale, &pNewEffect);
		pNewEffect->SetValue(D2D1_SCALE_PROP_SCALE, D2D1_POINT_2F{ _scaleX, _scaleY });
		pNewEffect->SetValue(D2D1_SCALE_PROP_INTERPOLATION_MODE, static_cast<D2D1_SCALE_INTERPOLATION_MODE>(_scaleMode));
		return newEffect;
	}

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
		graphics::pD2DContext->DrawImage(pEffect_.Get());
	}
}