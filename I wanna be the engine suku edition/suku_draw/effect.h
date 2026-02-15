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

	class EffectCut;

	class EffectContrast;
	class EffectBrightness;
	class EffectSaturation;

	class EffectTransform;

	class EffectBlur;
	class EffectBlurDirectional;
	class EffectOpacityMask;

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

	class EffectCut : public Effect
	{
	public:
		EffectCut(bool _isAtlasMode = false);
		EffectCut(float _x, float _y, float _width, float _height, bool _isAtlasMode = false);
		void setCutArea(float _x, float _y, float _width, float _height);
		void setCutX(float _x);
		void setCutY(float _y);
		void setCutWidth(float _width);
		void setCutHeight(float _height);
		void setCutPosition(float _x, float _y);
		void setCutSize(float _width, float _height);

		bool isAtlasMode()const { return isAtlasMode_; }
		float getCutX()const { return x_; }
		float getCutY()const { return y_; }
		float getCutWidth()const { return width_; }
		float getCutHeight()const { return height_; }
	private:
		bool isAtlasMode_ = false;
		float x_ = 0;
		float y_ = 0;
		float width_ = FLT_MAX;
		float height_ = FLT_MAX;
	};

	class EffectContrast : public Effect
	{
	public:
		EffectContrast();
		EffectContrast(float _contrast);
		// contrast argument range: (-inf, +inf)
		// 1.0f -> no change, 0.0f -> completely gray
		void setContrast(float _contrast);
		float getContrast()const { return contrast_; }
	private:
		float contrast_ = 1.0f;
	};

	class EffectBrightness : public Effect
	{
	public:
		EffectBrightness();
		EffectBrightness(float _brightness);
		// brightness argument range: (-1.0f, +1.0f)
		// 0.0f -> no change, positive -> brighter, negative -> darker
		void setBrightness(float _brightness);
		float getBrightness()const { return brightness_; }
	private:
		float brightness_ = 0.0f;
	};

	class EffectSaturation : public Effect
	{
	public:
		EffectSaturation();
		EffectSaturation(float _saturation);
		// saturation argument range: (0.0f, +inf)
		// 0.5f -> no change, 0.0f -> completely gray
		// <= 2.0f recommended
		void setSaturation(float _saturation);
		float getSaturation()const { return saturation_; }
	private:
		float saturation_ = 0.5f;
	};

	class EffectTransform : public Effect
	{
	public:
		EffectTransform();
		EffectTransform(Transform _transform,
			ScaleMode _scaleMode = ScaleMode::HighQualityCubic, bool _isBorderSoftMode = false, float _sharpness = 0.0f);
		EffectTransform(ScaleMode _scaleMode, bool _isBorderSoftMode = false, float _sharpness = 0.0f);

		Transform getTransform()const { return transform_; }
		void setTransform(Transform _transform);
		ScaleMode getScaleMode()const { return scaleMode_; }
		void setScaleMode(ScaleMode _scaleMode);
		bool isBorderSoftMode()const { return isBorderSoftMode_; }
		void setBorderSoftMode(bool _isBorderSoftMode);
		// sharpness argument range: (0.0f, 1.0f)
		// can be available only when scaleMode_ == scaleMode::HighQualityCubic
		float getSharpness() { return sharpness_; }
		void setSharpness(float _sharpness);
	private:
		Transform transform_ = Transform();
		ScaleMode scaleMode_ = ScaleMode::Linear;
		float sharpness_ = 0.0f;
		bool isBorderSoftMode_ = false;
	};

	class EffectBlur : public Effect
	{
	public:
		EffectBlur(bool _isBorderSoftMode = false);
		EffectBlur(float _blurRadius, bool _isBorderSoftMode = false);

		float getBlurRadius()const { return blurRadius_; }
		void setBlurRadius(float _blurRadius);
		bool isBorderSoftMode()const { return isBorderSoftMode_; }
		void setBorderSoftMode(bool _isBorderSoftMode);
	private:
		float blurRadius_ = 9.0f;
		bool isBorderSoftMode_ = false;
	};

	class EffectBlurDirectional : public Effect
	{
	public:
		EffectBlurDirectional(float _blurAngle, bool _isBorderSoftMode = false);
		EffectBlurDirectional(float _blurAngle, float _blurRadius, bool _isBorderSoftMode = false);

		float getBlurAngle()const { return blurAngle_; }
		void setBlurAngle(float _blurAngle);
		float getBlurRadius()const { return blurRadius_; }
		void setBlurRadius(float _blurRadius);
		bool isBorderSoftMode()const { return isBorderSoftMode_; }
		void setBorderSoftMode(bool _isBorderSoftMode);
	private:
		float blurAngle_ = 0.0f;
		float blurRadius_ = 9.0f;
		bool isBorderSoftMode_ = false;
	};

	class EffectOpacityMask : public Effect
	{
	public:
		EffectOpacityMask();
		EffectOpacityMask(Bitmap& _maskBitmap);
		EffectOpacityMask(RenderBitmap& _maskBitmap);

		void setMaskBitmap(Bitmap& _maskBitmap);
		void setMaskBitmap(RenderBitmap& _maskBitmap);
	};
}