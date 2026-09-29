#include "suku_property.h"

namespace suku
{
	template<suku_property_type T>
	template<typename U>
	inline Property<T>& Property<T>::operator=(U _value) requires std::is_same_v<T, Real> && std::is_arithmetic_v<U>
	{
		value_ = static_cast<T>(_value);
		return *this;
	}

	template<suku_property_type T>
	inline Property<T>& Property<T>::operator=(T _value)
	{
		value_ = _value;
		return *this;
	}

	template<suku_property_type T>
	inline Property<T>& Property<T>::operator=(std::pair<T, const Transition&> _valueWithTransition)
	{
		auto& [value, transition] = _valueWithTransition;
		if (transition.getDuration() <= 0.0)
		{
			setValueForce(value);
			return *this;
		}
		isTranslating_ = true;
		currentTransition_ = transition;
		transitionDuration_ = transition.getDuration();
		transitionValueBegin_ = value_;
		transitionValueEnd_ = value;
		transitionElapsedTime_ = 0.0;
		return *this;
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
		if (transition.getDuration() <= 0.0)
		{
			setValueForce(getExpectedValue() + value);
			return;
		}
		isTranslating_ = true;
		currentTransition_ = transition;
		transitionDuration_ = transition.getDuration();
		transitionValueBegin_ = getExpectedValue();
		transitionValueEnd_ = transitionValueBegin_ + value;
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
		if (transition.getDuration() <= 0.0)
		{
			setValueForce(getExpectedValue() - value);
			return;
		}
		isTranslating_ = true;
		currentTransition_ = transition;
		transitionDuration_ = transition.getDuration();
		transitionValueBegin_ = getExpectedValue();
		transitionValueEnd_ = transitionValueBegin_ - value;
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
		if (transition.getDuration() <= 0.0)
		{
			setValueForce(getExpectedValue() * value);
			return;
		}
		isTranslating_ = true;
		currentTransition_ = transition;
		transitionDuration_ = transition.getDuration();
		transitionValueBegin_ = getExpectedValue();
		transitionValueEnd_ = transitionValueBegin_ * value;
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
		if (transition.getDuration() <= 0.0)
		{
			setValueForce(getExpectedValue() / value);
			return;
		}
		isTranslating_ = true;
		currentTransition_ = transition;
		transitionDuration_ = transition.getDuration();
		transitionValueBegin_ = getExpectedValue();
		transitionValueEnd_ = transitionValueBegin_ / value;
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
	inline void Property<T>::setValueForce(T _value)
	{
		isTranslating_ = false;
		value_ = _value;
		forceUpdateTag_ = true;
		forceUpdateValue_ = _value;
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
	inline T Property<T>::getInterpolatedFrameState(float _offsetRate) const
	{
		return getLastFrameState() * (1 - _offsetRate) + getFrameState() * _offsetRate;
	}

	template<suku_property_type T>
	inline void Property<T>::updateFrameState()
	{
		if (!isTranslating_ && forceUpdateTag_)
		{
			lastFrameState_ = frameState_ = forceUpdateValue_;
		}
		else
		{
			lastFrameState_ = frameState_;
			frameState_ = getValue();
		}
		forceUpdateTag_ = false;
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