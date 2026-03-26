#include "pch.h"
#include "text.h"
#include <dwrite.h>

namespace suku
{

	namespace graphics
	{
		TextFactoryGlobal::TextFactoryGlobal()
		{
			DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory),
				reinterpret_cast<IUnknown**>(pDWriteFactory_.GetAddressOf())
			);
		}
	}
}