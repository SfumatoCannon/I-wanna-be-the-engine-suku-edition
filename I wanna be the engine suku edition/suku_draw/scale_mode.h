#pragma once
#include <d2d1.h>
#include <d2d1effects.h>
#include "../suku_foundation/message.h"


namespace suku
{
	template<typename T>
	concept ScaleModeTargetType =
		std::same_as<D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE, T> ||
		std::same_as<D2D1_BITMAP_INTERPOLATION_MODE, T>;

	enum class ScaleMode
	{
		NearestNeighbor,
		Linear,
		Cubic,
		MultiSampleLinear,
		Anisotropic,
		HighQualityCubic
	};

	class ScaleModeTranslator
	{
	public:
		template <ScaleModeTargetType T>
		static T toNative(ScaleMode _scaleMode);
	};
}

#include "scale_mode.inl"