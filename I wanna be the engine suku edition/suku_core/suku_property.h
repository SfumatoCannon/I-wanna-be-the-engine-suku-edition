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
		Property(const T& _value) : value_(_value), frameState_(_value), lastFrameState_(_value) {}
		Property(T&& _value) : value_(std::move(_value)), frameState_(value_), lastFrameState_(value_) {}

		operator T() const { return value_; }

		void operator=(T _value);
		void operator=(std::pair<T, const Transition&> _valueWithTransition);
		void operator+=(T _value);
		void operator+=(std::pair<T, const Transition&> _valueWithTransition);
		void operator-=(T _value);
		void operator-=(std::pair<T, const Transition&> _valueWithTransition);
		void operator*=(T _value);
		void operator*=(std::pair<T, const Transition&> _valueWithTransition);
		void operator/=(T _value);
		void operator/=(std::pair<T, const Transition&> _valueWithTransition);
		T operator++(int);
		T operator--(int);
		auto operator<=>(const T& _value) const { return getValue() <=> _value; }

		void setValueForce(T _value);
		T getValue() const;
		T getExpectedValue() const;
		T getFrameState() const { return frameState_; }
		T getLastFrameState() const { return lastFrameState_; }
		void updateFrameState();
		void addTick(double _ticks = 1.0);
	private:
		Transition currentTransition_ = Transition(0.0, TransitionCurve::linear);
		bool isTranslating_ = false;
		T transitionValueBegin_;
		T transitionValueEnd_;
		double transitionDuration_ = 0.0;
		double transitionElapsedTime_ = 0.0; //?
		long double transitionStartTime_ = 0;
		T value_;
		T frameState_;
		T lastFrameState_;
		bool forceUpdateTag_ = false;
		T forceUpdateValue_;
	};
}

#include "suku_property.inl"