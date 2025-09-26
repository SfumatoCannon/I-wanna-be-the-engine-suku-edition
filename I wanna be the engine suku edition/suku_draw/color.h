#pragma once
#include "../framework.h"
#include "../suku_foundation.h"

namespace suku
{
	class Color
	{
	public:
		float alpha;	//range: 0.0f ~ 1.0f

		constexpr Color() : r_(0.0f), g_(0.0f), b_(0.0f), alpha(1.0f) {}
		constexpr Color(float _r, float _g, float _b, float _alpha = 1.0f)
			: r_(_r), g_(_g), b_(_b), alpha(_alpha) {
		}

		inline float r()const { return r_; }
		inline float g()const { return g_; }
		inline float b()const { return b_; }
		float h()const;
		float s()const;
		float v()const;
		inline void setR(float _r) { r_ = _r; }
		inline void setG(float _g) { g_ = _g; }
		inline void setB(float _b) { b_ = _b; }
		inline void setRGB(float _r, float _g, float _b) { r_ = _r, g_ = _g, b_ = _b; }
		void setH(float _h);
		void setS(float _s);
		void setV(float _v);
		void setHSV(float _h, float _s, float _v);

		static constexpr Color BLACK(float _alpha = 1.0f) { return Color(0.0f, 0.0f, 0.0f, _alpha); }
		static constexpr Color WHITE(float _alpha = 1.0f) { return Color(255.0f, 255.0f, 255.0f, _alpha); }
		static constexpr Color RED(float _alpha = 1.0f) { return Color(255.0f, 0.0f, 0.0f, _alpha); }
		static constexpr Color GREEN(float _alpha = 1.0f) { return Color(0.0f, 255.0f, 0.0f, _alpha); }
		static constexpr Color BLUE(float _alpha = 1.0f) { return Color(0.0f, 0.0f, 255.0f, _alpha); }
	private:
		float r_, g_, b_;	//range: 0.0f ~ 255.0f
	};
}