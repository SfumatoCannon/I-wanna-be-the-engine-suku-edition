#pragma once
#include <windows.h>

namespace suku
{
	class GameWindow
	{
	public:
		GameWindow() = delete;
		static HWND hWnd;
		static String title;
		static int width;
		static int height;
	};
}