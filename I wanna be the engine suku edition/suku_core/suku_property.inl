#include "suku_property.h"

namespace suku
{

	template<typename T>
	inline void Property<T>::addTick(double _ticks)
	{
		tickCount_ += _ticks;
	}
}