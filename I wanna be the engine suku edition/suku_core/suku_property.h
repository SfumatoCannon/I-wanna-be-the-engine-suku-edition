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
		T& operator()() { return value_; }
		const T& operator()() const { return value_; }
		void operator=(const T& _value) { value_ = _value; }
		void operator=(T&& _value) { value_ = std::move(_value); }
		T operator+(const T& _value) const { return value_ + _value; }
		void operator+=(const T& _value) { value_ += _value; }
		T operator-(const T& _value) const { return value_ - _value; }
		void operator-=(const T& _value) { value_ -= _value; }
		T operator*(const T& _value) const { return value_ * _value; }
		void operator*=(const T& _value) { value_ *= _value; }
		T operator/(const T& _value) const { return value_ / _value; }
		void operator/=(const T& _value) { value_ /= _value; }
		auto operator<=>(const T& _value) const { return value_ <=> _value; }

		void addTick(double _ticks = 1.0);
	private:
		const TransitionCurve& defaultTransitionCurve_ = linear;
		T value_;
		double tickCount_ = 0.0f;
	};
}

#include "suku_property.inl"