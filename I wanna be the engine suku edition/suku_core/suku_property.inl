#include "suku_property.h"

namespace suku
{
	template<suku_property_type T>
	inline void Property<T>::bindClock(const long double& _clock)
	{
		parentClock_ = &_clock;
	}

	template<suku_property_type T>
	inline void Property<T>::operator=(T _value)
	{
		value_ = _value;
	}

	template<suku_property_type T>
	inline void Property<T>::operator=(std::pair<T, const Transition&> _valueWithTransition)
	{
		auto& [value, transition] = _valueWithTransition;
		isTranslating_ = true;
		currentTransition_ = transition;
		transitionDuration_ = transition.getDuration();
		transitionValueBegin_ = value_;
		transitionValueEnd_ = value;
		transitionStartTime_ = *parentClock_;
		transitionElapsedTime_ = 0.0;
	}

	template<suku_property_type T>
	inline void Property<T>::operator+=(T _value)
	{
		value_ = getExpectedValue() + _value;
	}

	template<suku_property_type T>
	inline void Property<T>::operator+=(std::pair<T, const Transition&> _valueWithTransition)
	{
		auto [value, transition] = _valueWithTransition;
		isTranslating_ = true;
		currentTransition_ = transition;
		transitionDuration_ = transition.getDuration();
		transitionValueBegin_ = getExpectedValue();
		transitionValueEnd_ = transitionValueBegin_ + value;
		transitionStartTime_ = *parentClock_;		
		transitionElapsedTime_ = 0.0;
	}

	template<suku_property_type T>
	inline void Property<T>::operator-=(T _value)
	{
		value_ = getExpectedValue() - _value;
	}

	template<suku_property_type T>
	inline void Property<T>::operator-=(std::pair<T, const Transition&> _valueWithTransition)
	{
		auto [value, transition] = _valueWithTransition;
		isTranslating_ = true;
		currentTransition_ = transition;
		transitionDuration_ = transition.getDuration();
		transitionValueBegin_ = getExpectedValue();
		transitionValueEnd_ = transitionValueBegin_ - value;
		transitionStartTime_ = *parentClock_;
		transitionElapsedTime_ = 0.0;
	}

	template<suku_property_type T>
	inline void Property<T>::operator*=(T _value)
	{
		value_ = getExpectedValue() * _value;
	}

	template<suku_property_type T>
	inline void Property<T>::operator*=(std::pair<T, const Transition&> _valueWithTransition)
	{
		auto [value, transition] = _valueWithTransition;
		isTranslating_ = true;
		currentTransition_ = transition;
		transitionDuration_ = transition.getDuration();
		transitionValueBegin_ = getExpectedValue();
		transitionValueEnd_ = transitionValueBegin_ * value;
		transitionStartTime_ = *parentClock_;
		transitionElapsedTime_ = 0.0;
	}

	template<suku_property_type T>
	inline void Property<T>::operator/=(T _value)
	{
		value_ = getExpectedValue() / _value;
	}

	template<suku_property_type T>
	inline void Property<T>::operator/=(std::pair<T, const Transition&> _valueWithTransition)
	{
		auto [value, transition] = _valueWithTransition;
		isTranslating_ = true;
		currentTransition_ = transition;
		transitionDuration_ = transition.getDuration();
		transitionValueBegin_ = getExpectedValue();
		transitionValueEnd_ = transitionValueBegin_ / value;
		transitionStartTime_ = *parentClock_;
		transitionElapsedTime_ = 0.0;
	}

	template<suku_property_type T>
	inline T Property<T>::operator++(int)
	{
		T old = value_;
		value_ = getExpectedValue() + 1;
		return old;
	}

	template<suku_property_type T>
	inline T Property<T>::operator--(int)
	{
		T old = value_;
		value_ = getExpectedValue() - 1;
		return old;
	}

	template<suku_property_type T>
	inline T Property<T>::getValue() const
	{
		if (!isTranslating_)
			return value_;
		else
		{
			return static_cast<T>(currentTransition_.getValue(transitionValueBegin_, transitionValueEnd_, transitionElapsedTime_));
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
		{
			transitionElapsedTime_ += _ticks;
			if (transitionElapsedTime_ >= transitionDuration_)
			{
				isTranslating_ = false;
				value_ = transitionValueEnd_;
			}
		}
		updateFrameState();
	}
}