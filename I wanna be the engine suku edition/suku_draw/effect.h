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
		void paint()const;
		//void paint(float _x, float _y)const;
		//void paint(float _x, float _y, Transform _transform)const;
		//void paint(Transform _transform)const;
	protected:
		ComPtr<ID2D1Effect> pEffect_ = nullptr;
	};

	class EffectTransform : public Effect
	{
	public:
		EffectTransform();
		EffectTransform(Transform _transform, 
			ScaleMode _scaleMode = ScaleMode::HighQualityCubic, float _sharpness = 0.0f);
		EffectTransform(ScaleMode _scaleMode, float _sharpness = 0.0f);

		Transform getTransform() { return transform_; }
		void setTransform(Transform _transform);
		ScaleMode getScaleMode(){ return scaleMode_; }
		void setScaleMode(ScaleMode _scaleMode);
		float getSharpness() { return sharpness_; }
		void setSharpness(float _sharpness);
	private:
		Transform transform_ = Transform();
		ScaleMode scaleMode_ = ScaleMode::Linear;
		float sharpness_ = 0.0f;
	};
}