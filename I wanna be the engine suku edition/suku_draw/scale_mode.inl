#pragma once

#include "../suku_foundation/string.h"

namespace suku
{
	template<ScaleModeTargetType T>
	inline T ScaleModeTranslator::toNative(ScaleMode _scaleMode)
	{
		if constexpr (std::is_same_v<T, D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE>)
		{
			switch (_scaleMode)
			{
			case ScaleMode::NearestNeighbor:
				return D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
			case ScaleMode::Linear:
				return D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE_LINEAR;
			case ScaleMode::Cubic:
				return D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE_CUBIC;
			case ScaleMode::MultiSampleLinear:
				return D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE_MULTI_SAMPLE_LINEAR;
			case ScaleMode::Anisotropic:
				return D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE_ANISOTROPIC;
			case ScaleMode::HighQualityCubic:
				return D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC;
			default:
				return D2D1_2DAFFINETRANSFORM_INTERPOLATION_MODE_LINEAR;
			}
		}
		else if constexpr (std::is_same_v<T, D2D1_BITMAP_INTERPOLATION_MODE>)
		{
			switch (_scaleMode)
			{
			case ScaleMode::NearestNeighbor:
				return D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
			case ScaleMode::Linear:
				return D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;
			case ScaleMode::Cubic:
			case ScaleMode::MultiSampleLinear:
			case ScaleMode::Anisotropic:
			case ScaleMode::HighQualityCubic:
				WARNINGWINDOW_GLOBAL("The selected ScaleMode is not supported in BitmapScaleMode. Using Linear instead.");
				return D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;
			default:
				return D2D1_BITMAP_INTERPOLATION_MODE_LINEAR;
			}
		}
	}
}