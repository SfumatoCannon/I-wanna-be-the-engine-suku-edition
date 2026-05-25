#include "suku_property.h"

namespace suku
{
	template<suku_property_type T>
	inline void Property<T>::bindClock(const unsigned long long& _clock)
	{
		parentClock_ = &_clock;
	}

	template<suku_property_type T>
	inline void Property<T>::operator=(const T& _value)
	{
		isTranslating_ = false;
		value_ = _value;
	}

	template<suku_property_type T>
	inline void Property<T>::operator=(std::pair<const T&, const Transition&> _valueWithTransition)
	{
		auto [value, transition] = _valueWithTransition;
		isTranslating_ = true;
		transitionDuration_ = transition.getDuration();
		transitionValueBegin_ = value_;
		transitionValueEnd_ = value;
		transitionStartTime_ = *parentClock_;
	}

	template<suku_property_type T>
	inline void Property<T>::operator+=(const T& _value)
	{
		value_ = getExpectedValue() + _value;
		isTranslating_ = false;
	}

	template<suku_property_type T>
	inline void Property<T>::operator+=(std::pair<const T&, const Transition&> _valueWithTransition)
	{
		auto [value, transition] = _valueWithTransition;
		isTranslating_ = true;
		transitionDuration_ = transition.getDuration();
		transitionValueBegin_ = getExpectedValue();
		transitionValueEnd_ = transitionValueBegin_ + value;
		transitionStartTime_ = *parentClock_;
	}

	template<suku_property_type T>
	inline void Property<T>::operator-=(const T& _value)
	{
		value_ = getExpectedValue() - _value;
		isTranslating_ = false;
	}

	template<suku_property_type T>
	inline void Property<T>::operator-=(std::pair<const T&, const Transition&> _valueWithTransition)
	{
		auto [value, transition] = _valueWithTransition;
		isTranslating_ = true;
		transitionDuration_ = transition.getDuration();
		transitionValueBegin_ = getExpectedValue();
		transitionValueEnd_ = transitionValueBegin_ - value;
		transitionStartTime_ = *parentClock_;
	}

	template<suku_property_type T>
	inline void Property<T>::operator*=(const T& _value)
	{
		value_ = getExpectedValue() * _value;
		isTranslating_ = false;
	}

	template<suku_property_type T>
	inline void Property<T>::operator*=(std::pair<const T&, const Transition&> _valueWithTransition)
	{
		auto [value, transition] = _valueWithTransition;
		isTranslating_ = true;
		transitionDuration_ = transition.getDuration();
		transitionValueBegin_ = getExpectedValue();
		transitionValueEnd_ = transitionValueBegin_ * value;
		transitionStartTime_ = *parentClock_;
	}

	template<suku_property_type T>
	inline void Property<T>::operator/=(const T& _value)
	{
		value_ = getExpectedValue() / _value;
		isTranslating_ = false;
	}

	template<suku_property_type T>
	inline void Property<T>::operator/=(std::pair<const T&, const Transition&> _valueWithTransition)
	{
		auto [value, transition] = _valueWithTransition;
		isTranslating_ = true;
		transitionDuration_ = transition.getDuration();
		transitionValueBegin_ = getExpectedValue();
		transitionValueEnd_ = transitionValueBegin_ / value;
		transitionStartTime_ = *parentClock_;
	}

	template<suku_property_type T>
	inline T Property<T>::operator++(int)
	{
		T old = value_;
		value_ = getExpectedValue() + 1;
		isTranslating_ = false;
		return old;
	}

	template<suku_property_type T>
	inline T Property<T>::operator--(int)
	{
		T old = value_;
		value_ = getExpectedValue() - 1;
		isTranslating_ = false;
		return old;
	}

	template<suku_property_type T>
	inline T Property<T>::getValue() const
	{
		if (!isTranslating_)
			return value_;
		else
		{
			double t = currentTransition_.getValue(0.0, 1.0, transitionElapsedTime_);
			return static_cast<T>(transitionValueBegin_ + (transitionValueEnd_ - transitionValueBegin_) * t);
		}
	}

	template<suku_property_type T>
	inline T Property<T>::getExpectedValue() const
	{
		if (!isTranslating_)
			return value_;
		else
			return transitionValueEnd_;
	}

	template<suku_property_type T>
	inline void Property<T>::updateFrameState()
	{
		lastFrameState_ = frameState_;
		frameState_ = getValue();
	}

	template<suku_property_type T>
	inline void Property<T>::addTick(double _ticks)
	{
		if (isTranslating_)
			transitionElapsedTime_ += _ticks;
		if (transitionElapsedTime_ >= transitionDuration_)
		{
			isTranslating_ = false;
			value_ = transitionValueEnd_;
		}
		updateFrameState();
	}
}