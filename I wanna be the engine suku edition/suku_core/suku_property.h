#pragma once
#include "transition.h"

namespace suku
{
	class Object;

	template<typename T>
	concept suku_property_type = std::is_arithmetic<T>::value;

	template<suku_property_type T>
	class Property
	{
	public:
		Property() = default;
		Property(const T& _value) : value_(_value) {}
		Property(T&& _value) : value_(std::move(_value)) {}
		void bindClock(const unsigned long long& _clock);

		T& operator()() { return value_; }
		const T& operator()() const { return value_; }
		void operator=(const T& _value);
		void operator=(std::pair<const T&, const Transition&> _valueWithTransition);
		void operator+=(const T& _value);
		void operator+=(std::pair<const T&, const Transition&> _valueWithTransition);
		void operator-=(const T& _value);
		void operator-=(std::pair<const T&, const Transition&> _valueWithTransition);
		void operator*=(const T& _value);
		void operator*=(std::pair<const T&, const Transition&> _valueWithTransition);
		void operator/=(const T& _value);
		void operator/=(std::pair<const T&, const Transition&> _valueWithTransition);
		T operator+(const T& _value) const { return getValue() + _value; }
		T operator-(const T& _value) const { return getValue() - _value; }
		T operator*(const T& _value) const { return getValue() * _value; }
		T operator/(const T& _value) const { return getValue() / _value; }
		auto operator<=>(const T& _value) const { return getValue() <=> _value; }

		T getValue() const;
		T getExpectedValue() const;
		void addTick(double _ticks = 1.0);
	private:
		const long double* parentClock_ = nullptr;
		Transition currentTransition_ = Transition(0.0, TransitionCurve::linear);
		bool isTranslating_ = false;
		T translateValueBegin_;
		T translateValueEnd_;
		double translateDuration_ = 0.0;
		double translateElapsedTime_ = 0.0; //?
		long double translateStartTime_ = 0;
		T value_;
	};
}

#include "suku_property.inl"