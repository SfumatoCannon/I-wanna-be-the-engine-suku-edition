#include "pch.h"
#include "win32_message_handler.h"
#include "game_window.h"
#include "../suku_draw/includes.h"
#include "../suku_foundation/includes.h"
#include "../suku_sounds.h"
#include "../suku_config/includes.h"
#include "game_loop.h"

#define WM_CREATEFINISHED (WM_USER + 1)

RAWINPUTDEVICE rid;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	using namespace suku;
	using namespace suku::input;
	std::pair<UINT, USHORT> inputResult;
	static bool wasWindowMaximized = false;

	switch (message)
	{
	case WM_CREATE:
		rid.usUsagePage = 0x01;
		rid.usUsage = 0x06;
		rid.dwFlags = 0;
		rid.hwndTarget = hWnd;
		// 输入法设置为英文
		static HKL hkl = LoadKeyboardLayout(L"00000409", KLF_ACTIVATE);
		ActivateKeyboardLayout(hkl, KLF_SETFORPROCESS);
		// reduce input delay
		RegisterRawInputDevices(&rid, 1, sizeof(rid));

		PostMessage(hWnd, WM_CREATEFINISHED, 0, 0);
		break;
	case WM_CREATEFINISHED:
		timeBeginPeriod(1);
		//suku_save_init();
		graphics::WICFactoryGlobal::getWICFactory();
		graphics::D2DFactoryGlobal::getD2DFactory();
		graphics::suku_drawing_postinit(hWnd);
		soundInit();
		init();
		if (ConfigElementPool::isVSyncOn.value())
			startSenderVsync();
		else
			startSender();
		break;
	case WM_INPUT:
		inputResult = suku::input::onWindowInput(lParam);
		if (inputResult.first == INPUT_KEYDOWN && inputResult.second == VK_F3)
		{
			suku::GameWindow::resetDisplaySizeToDefault();
		}
		if (inputResult.first == INPUT_KEYDOWN && inputResult.second == VK_F4)
		{
			static bool fullscreenTag = false;
			fullscreenTag = !fullscreenTag;
			suku::GameWindow::setFullscreen(fullscreenTag);
		}
		break;
	case WM_ERASEBKGND:
		return true;
		break;
	case WM_DESTROY:
		timeEndPeriod(1);
		graphics::suku_drawing_uninit();
		soundUninit();
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
			suku::GameWindow::onMinimize();
		else if (wParam == SIZE_MAXIMIZED)
		{
			wasWindowMaximized = true;
			suku::GameWindow::onMaximize(true);
			suku::GameWindow::refreshSizeInfo();
		}
		else
		{
			if (wasWindowMaximized)
			{
				wasWindowMaximized = false;
				suku::GameWindow::onMaximize(false);
			}
			suku::GameWindow::refreshSizeInfo();
		}
		break;
	case WM_MOVE:
		suku::GameWindow::refreshPositionInfo(
			(int)(lParam & 0xFFFF), 
			(int)((lParam >> 16) & 0xFFFF)
		);
		break;
	case WM_ACTIVATE:
		if (wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE)
			suku::GameWindow::onFocusChanged(true);
		else
			suku::GameWindow::onFocusChanged(false);
		break;
	case WM_MOUSEMOVE:
		suku::input::Mouse::refreshPosition(lParam);
		break;
	case WM_LBUTTONDOWN:
		suku::input::Mouse::pushMouseMessage(MOUSE_INPUT_DOWN, MOUSE_VK_LEFT);
		SetCapture(hWnd);
		break;
	case WM_LBUTTONUP:
		suku::input::Mouse::pushMouseMessage(MOUSE_INPUT_UP, MOUSE_VK_LEFT);
		ReleaseCapture();
		break;
	case WM_LBUTTONDBLCLK:
		suku::input::Mouse::pushMouseMessage(MOUSE_INPUT_DOUBLECLICK, MOUSE_VK_LEFT);
		break;
	case WM_RBUTTONDOWN:
		suku::input::Mouse::pushMouseMessage(MOUSE_INPUT_DOWN, MOUSE_VK_RIGHT);
		break;
	case WM_RBUTTONUP:
		suku::input::Mouse::pushMouseMessage(MOUSE_INPUT_UP, MOUSE_VK_RIGHT);
		break;
	case WM_MBUTTONDOWN:
		suku::input::Mouse::pushMouseMessage(MOUSE_INPUT_DOWN, MOUSE_VK_MIDDLE);
		break;
	case WM_MBUTTONUP:
		suku::input::Mouse::pushMouseMessage(MOUSE_INPUT_UP, MOUSE_VK_MIDDLE);
		break;
	case WM_MOUSEWHEEL:
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			suku::input::Mouse::pushMouseMessage(MOUSE_INPUT_SCROLLUP, 0);
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
			suku::input::Mouse::pushMouseMessage(MOUSE_INPUT_SCROLLDOWN, 0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}