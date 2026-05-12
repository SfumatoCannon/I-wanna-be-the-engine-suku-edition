#include "suku_property.h"
#include "object.h"

namespace suku
{
	template<typename T>
	inline void Property<T>::bind(Object* _parent)
	{
		parent_ = _parent;
		parentClock_ = &_parent->clock_;
	}

	template<typename T>
	inline void Property<T>::operator=(const T& _value)
	{
		isTranslating_ = false;
		value_ = _value;
	}

	template<typename T>
	inline void Property<T>::operator=(std::pair<const T&, const Transition&> _valueWithTransition)
	{
		auto [value, transition] = _valueWithTransition;
		isTranslating_ = true;
		translateDuration_ = transition.getDuration();
		translateValueBegin_ = value_;
		translateValueEnd_ = value;
		translateStartTime_ = *parentClock_;
	}

	template<typename T>
	inline void Property<T>::operator+=(const T& _value)
	{
		isTranslating_ = false;
		value_ = getExpectedValue() + _value;
	}

	template<typename T>
	inline void Property<T>::operator+=(std::pair<const T&, const Transition&> _valueWithTransition)
	{
		auto [value, transition] = _valueWithTransition;
		isTranslating_ = true;
		translateDuration_ = transition.getDuration();
		translateValueBegin_ = getExpectedValue();
		translateValueEnd_ = translateValueBegin_ + value;
		translateStartTime_ = *parentClock_;
	}

	template<typename T>
	inline void Property<T>::operator-=(const T& _value)
	{
		isTranslating_ = false;
		value_ = getExpectedValue() - _value;
	}

	template<typename T>
	inline void Property<T>::operator-=(std::pair<const T&, const Transition&> _valueWithTransition)
	{
		auto [value, transition] = _valueWithTransition;
		isTranslating_ = true;
		translateDuration_ = transition.getDuration();
		translateValueBegin_ = getExpectedValue();
		translateValueEnd_ = translateValueBegin_ - value;
		translateStartTime_ = *parentClock_;
	}

	template<typename T>
	inline void Property<T>::operator*=(const T& _value)
	{
		isTranslating_ = false;
		value_ = getExpectedValue() * _value;
	}

	template<typename T>
	inline void Property<T>::operator*=(std::pair<const T&, const Transition&> _valueWithTransition)
	{
		auto [value, transition] = _valueWithTransition;
		isTranslating_ = true;
		translateDuration_ = transition.getDuration();
		translateValueBegin_ = getExpectedValue();
		translateValueEnd_ = translateValueBegin_ * value;
		translateStartTime_ = *parentClock_;
	}

	template<typename T>
	inline void Property<T>::operator/=(const T& _value)
	{
		isTranslating_ = false;
		value_ = getExpectedValue() / _value;
	}

	template<typename T>
	inline void Property<T>::operator/=(std::pair<const T&, const Transition&> _valueWithTransition)
	{
		auto [value, transition] = _valueWithTransition;
		isTranslating_ = true;
		translateDuration_ = transition.getDuration();
		translateValueBegin_ = getExpectedValue();
		translateValueEnd_ = translateValueBegin_ / value;
		translateStartTime_ = *parentClock_;
	}

	template<typename T>
	inline T Property<T>::getValue() const
	{
		if (!isTranslating_)
			return value_;
		else
		{
			if (parentClock_ == nullptr)
				return value_;
			translateElapsedTime_ = (double)(*parentClock_ - translateStartTime_);
			if (translateElapsedTime_ >= translateDuration_)
			{
				isTranslating_ = false;
				value_ = translateValueEnd_;
				return value_;
			}
			else
			{
				double t = currentTransition_.getValue(0.0, 1.0, translateElapsedTime_);
				return translateValueBegin_ + (translateValueEnd_ - translateValueBegin_) * t;
			}
		}
	}

	template<typename T>
	inline T Property<T>::getExpectedValue() const
	{
		if (!isTranslating_)
			return value_;
		else
			return translateValueEnd_;
	}

	template<typename T>
	inline void Property<T>::addTick(double _ticks)
	{
		if (isTranslating_)
			translateElapsedTime_ += _ticks;
	}
}