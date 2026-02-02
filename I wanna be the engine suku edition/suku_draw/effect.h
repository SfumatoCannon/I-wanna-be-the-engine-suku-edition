#pragma once
#include <d2d1_1.h>
#include "scale_mode.h"
#include "bitmap.h"
#include "transform.h"
#include <wrl/client.h>

namespace suku
{
	using Microsoft::WRL::ComPtr;

	class Effect;
	class EffectTransform;

	class Effect
	{
	public:
		
		void setInput(Bitmap& _bitmap, int _index = 0);
		void setInput(RenderBitmap& _renderBitmap, int _index = 0);
		void setInput(const Effect& _effect, int _index = 0);
		void drawEffect()const;
	protected:
		ComPtr<ID2D1Effect> pEffect_ = nullptr;
	};

	class EffectTransform : public Effect
	{
	public:
		EffectTransform();
		EffectTransform(Transform _transform, 
			EffectScaleMode _scaleMode = EffectScaleMode::HighQualityCubic, float _sharpness = 0.0f);
		EffectTransform(EffectScaleMode _scaleMode, float _sharpness = 0.0f);

		Transform getTransform() { return transform_; }
		void setTransform(Transform _transform);
		EffectScaleMode getScaleMode(){ return scaleMode_; }
		void setScaleMode(EffectScaleMode _scaleMode);
		float getSharpness() { return sharpness_; }
		void setSharpness(float _sharpness);
	private:
		Transform transform_;
		EffectScaleMode scaleMode_;
		float sharpness_ = 0.0f;
	};
}