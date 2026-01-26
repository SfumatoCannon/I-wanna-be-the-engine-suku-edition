#pragma once
#include <windows.h>
#include "string.h"
#include "../global_value.h"
#include "../suku_draw/transform.h"

namespace suku
{
	class GameWindow
	{
	public:
		GameWindow() = delete;
		inline static HWND hWnd;
		static String GetTitle() { return title_; }
		static std::pair<UINT, UINT> getSize();
		static UINT getWidth() { return getSize().first; }
		static UINT getHeight() { return getSize().second; }
		static std::pair<UINT, UINT> getLogicalSize() { return { logicalWidth_, logicalHeight_ }; }
		static UINT getLogicalWidth() { return logicalWidth_; }
		static UINT getLogicalHeight() { return logicalHeight_; }
		static void setLogicalSize(UINT _width, UINT _height);
		static void RefreshSizeInfo();

		static Transform getPixelMappingTransform();

	private:
		inline static String title_ = L"I wanna be the engine suku edition";
		inline static UINT width_ = 0;
		inline static UINT height_ = 0;
		inline static UINT logicalWidth_ = WindowWidth;
		inline static UINT logicalHeight_ = WindowHeight;
		inline static Transform pixelMappingTransform_;
		inline static bool sizeUpdateTag_ = true;
		inline static bool pixelMappingTransformUpdateTag_ = true;
	};
}