#pragma once
#include "transition.h"

namespace suku
{
	template<typename T>
	class Property
	{
	public:
		Property() = default;
		Property(const T& _value) : value_(_value) {}
		Property(T&& _value) : value_(std::move(_value)) {}
		T& operator()() { return getValue(); }
		const T& operator()() const { return getValue(); }
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
		const TransitionCurve& defaultTransitionCurve_ = TransitionCurve::linear;
		Transition currentTransition_ = Transition(0.0, defaultTransitionCurve_);
		bool isTranslating_ = false;
		T translateValueBegin_;
		T translateValueEnd_;
		double translateDuration_ = 0.0;
		double translateElapsedTime_ = 0.0;
		T value_;
	};
}

#include "suku_property.inl"