#include "color.h"

namespace suku
{
	float Color::h()const
	{
		float R = r_ / 255.0f;
		float G = g_ / 255.0f;
		float B = b_ / 255.0f;
		float maxValue = max(R, max(G, B));
		float minValue = min(R, min(G, B));
		float delta = maxValue - minValue;
		if (delta == 0)
			return 0.0f;
		else if (maxValue == R)
			return 60.0f * (G - B) / delta;
		else if (maxValue == G)
			return 60.0f * ((B - R) / delta + 2);
		else   // maxValue == B
			return 60.0f * ((R - G) / delta + 4);
	}

	float Color::s()const
	{
		float R = r_ / 255.0f;
		float G = g_ / 255.0f;
		float B = b_ / 255.0f;
		float maxValue = max(R, max(G, B));
		float minValue = min(R, min(G, B));
		float delta = maxValue - minValue;
		if (maxValue == 0.0f)
			return 0.0f;
		else	// maxValue != 0.0f
			return delta / maxValue;
	}

	float Color::v()const
	{
		float R = r_ / 255.0f;
		float G = g_ / 255.0f;
		float B = b_ / 255.0f;
		float maxValue = max(R, max(G, B));
		return maxValue;
	}

	void Color::setH(float _h)
	{
		setHSV(_h, s(), v());
	}

	void Color::setS(float _s)
	{
		setHSV(h(), _s, v());
	}

	void Color::setV(float _v)
	{
		setHSV(h(), s(), _v);
	}

	void Color::setHSV(float _h, float _s, float _v)
	{
		float C = _v * _s;
		float X = C * (1.0f - (float)fabs((int)(_h / 60) % 2 - 1));
		float m = _v - C;

		float R = 0, G = 0, B = 0;
		if (_h < 60)
			R = C, G = X;
		else if (_h < 120)
			R = X, G = C;
		else if (_h < 180)
			G = C, B = X;
		else if (_h < 240)
			G = X, B = C;
		else if (_h < 300)
			R = X, B = C;
		else // _h < 360
			R = C, B = X;

		r_ = (R + m) * 255.0f;
		g_ = (G + m) * 255.0f;
		b_ = (B + m) * 255.0f;
	}
}