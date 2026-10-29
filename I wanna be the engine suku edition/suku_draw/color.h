#pragma once
#include <suku_foundation/maths.h>

namespace suku
{
	class Color
	{
	public:
		float alpha;	//range: 0.0f ~ 1.0f

		constexpr Color() 
			: r_(0.0f), g_(0.0f), b_(0.0f), alpha(1.0f) {}
		constexpr Color(real _r, real _g, real _b, real _alpha = 1.0f)
			: r_(_r), g_(_g), b_(_b), alpha(_alpha) {}
		constexpr Color(const Color& _other) 
			: r_(_other.r()), g_(_other.g()), b_(_other.b()), alpha(_other.alpha) {}
		constexpr Color(const Color& _other, float _alpha)
			: r_(_other.r()), g_(_other.g()), b_(_other.b()), alpha(_alpha) {}

		inline float r()const { return r_; }
		inline float g()const { return g_; }
		inline float b()const { return b_; }
		float h()const;
		float s()const;
		float v()const;
		inline void setR(real _r) { r_ = _r; }
		inline void setG(real _g) { g_ = _g; }
		inline void setB(real _b) { b_ = _b; }
		inline void setRGB(real _r, real _g, real _b) { r_ = _r, g_ = _g, b_ = _b; }
		void setH(real _h);
		void setS(real _s);
		void setV(real _v);
		void setHSV(real _h, real _s, real _v);

		static const Color Black;
		static const Color White;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
	private:
		float r_, g_, b_;	//range: 0.0f ~ 255.0f
	};

	inline const Color Color::Black(0.0f, 0.0f, 0.0f);
	inline const Color Color::White(255.0f, 255.0f, 255.0f);
	inline const Color Color::Red(255.0f, 0.0f, 0.0f);
	inline const Color Color::Green(0.0f, 255.0f, 0.0f);
	inline const Color Color::Blue(0.0f, 0.0f, 255.0f);
}