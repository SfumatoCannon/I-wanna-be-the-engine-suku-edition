#pragma once
#include "transition.h"

namespace suku
{
	class Object;

	template<typename T>
	class Property
	{
	public:
		Property() = default;
		Property(const T& _value) : value_(_value) {}
		Property(T&& _value) : value_(std::move(_value)) {}
		void bind(Object* _parent);

		T& operator()() { return value_; }
		const T& operator()() const { return value_; }
		void operator=(const T& _value);
		T operator+(const T& _value) const { return getValue() + _value; }
		void operator+=(const T& _value);
		T operator-(const T& _value) const { return getValue() - _value; }
		void operator-=(const T& _value);
		T operator*(const T& _value) const { return getValue() * _value; }
		void operator*=(const T& _value);
		T operator/(const T& _value) const { return getValue() / _value; }
		void operator/=(const T& _value);
		auto operator<=>(const T& _value) const { return getValue() <=> _value; }

		T getValue() const;
		void addTick(double _ticks = 1.0);
	private:
		Object* parent_ = nullptr;
		const long double* parentClock_ = nullptr;
		const TransitionCurve& defaultTransitionCurve_ = TransitionCurve::linear;
		Transition currentTransition_ = Transition(0.0, defaultTransitionCurve_);
		bool isTranslating_ = false;
		T translateValueBegin_;
		T translateValueEnd_;
		double translateDuration_ = 0.0;
		double translateElapsedTime_ = 0.0;
		long double translateStartClock_ = 0;
		T value_;
	};
}

#include "suku_property.inl"