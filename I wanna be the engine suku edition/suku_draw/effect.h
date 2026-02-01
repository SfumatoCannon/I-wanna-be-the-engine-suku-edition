#pragma once
#include <d2d1_1.h>
#include "scale_mode.h"
#include "bitmap.h"
#include <wrl/client.h>

namespace suku
{
	using Microsoft::WRL::ComPtr;

	class Effect
	{
	public:
		static Effect scaleEffect(float _scaleX, float _scaleY, EffectScaleMode _scaleMode = EffectScaleMode::HighQualityCubic);
		
		void setInput(Bitmap& _bitmap, int _index = 0);
		void setInput(RenderBitmap& _renderBitmap, int _index = 0);
		void setInput(const Effect& _effect, int _index = 0);
		void drawEffect()const;
	private:
		ComPtr<ID2D1Effect> pEffect_ = nullptr;
	};
}