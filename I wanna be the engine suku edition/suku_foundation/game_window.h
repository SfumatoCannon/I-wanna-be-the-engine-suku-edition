#pragma once
#include <windows.h>
#include "string.h"
#include "../global_value.h"

namespace suku
{
	class GameWindow
	{
	public:
		GameWindow() = delete;
		inline static HWND hWnd;
		String GetTitle() { return title_; }
		
	private:
		inline static String title_ = L"I wanna be the engine suku edition";
		inline static UINT width_ = WindowWidth;
		inline static UINT height_ = WindowHeight;
		inline static UINT viewWidth_ = WindowWidth;
		inline static UINT viewHeight_ = WindowHeight;
	};
}