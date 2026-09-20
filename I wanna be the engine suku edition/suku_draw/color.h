#pragma once

namespace suku
{
	class Color
	{
	public:
		float alpha;	//range: 0.0f ~ 1.0f

		constexpr Color() 
			: r_(0.0f), g_(0.0f), b_(0.0f), alpha(1.0f) {}
		constexpr Color(float _r, float _g, float _b, float _alpha = 1.0f)
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
		inline void setR(float _r) { r_ = _r; }
		inline void setG(float _g) { g_ = _g; }
		inline void setB(float _b) { b_ = _b; }
		inline void setRGB(float _r, float _g, float _b) { r_ = _r, g_ = _g, b_ = _b; }
		void setH(float _h);
		void setS(float _s);
		void setV(float _v);
		void setHSV(float _h, float _s, float _v);

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