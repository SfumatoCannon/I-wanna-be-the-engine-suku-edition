#pragma once
#include <windows.h>
#include "../suku_foundation/suku_string.h"
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
		static bool isFullscreen() { return isFullscreen_; }
		static bool isFocused() { return isFocused_; }
		static bool isMinimized();

		static void setLogicalSize(UINT _width, UINT _height);
		static void setFullscreen(bool _isFullscreen);
		static void refreshSizeInfo();
		static void refreshPositionInfo(int _posX, int _posY);

		static void onMinimize();
		static void onFocusChanged(bool _isFocused);

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
		inline static bool isFullscreen_ = false;
		inline static bool isFocused_ = false;
		inline static WINDOWPLACEMENT previousWindowPlacement_ = { sizeof(WINDOWPLACEMENT) };
	};
}