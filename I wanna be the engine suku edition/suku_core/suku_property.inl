#include "suku_property.h"

namespace suku
{
	template<typename T>
	inline T Property<T>::getValue() const
	{
		if (!isTranslating_)
			return value_;
		else
		{
		}
	}

	template<typename T>
	inline void Property<T>::addTick(double _ticks)
	{
		if (isTranslating_)
			translateElapsedTime_ += _ticks;
	}
}