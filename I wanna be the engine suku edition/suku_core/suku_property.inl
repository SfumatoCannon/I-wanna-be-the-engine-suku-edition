#include "suku_property.h"

namespace suku
{

	template<typename T>
	inline void Property<T>::updateFrame(double _ticks)
	{
		clock_ += _ticks;
	}
}