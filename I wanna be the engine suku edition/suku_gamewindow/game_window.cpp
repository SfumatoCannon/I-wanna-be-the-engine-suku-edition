#include "pch.h"
#include "game_window.h"
#include "../suku_foundation/message.h"
#include "../suku_draw/draw_core.h"
#include "../suku_config/includes.h"

namespace suku
{
	std::pair<int, int> GameWindow::getPosition()
	{
		if (positionUpdateTag_)
		{
			RECT rc;
			GetClientRect(hWnd, &rc);
			x_ = rc.left;
			y_ = rc.top;
			return { x_, y_ };
		}
		else
		{
			return { x_, y_ };
		}
	}

	std::pair<UINT, UINT> GameWindow::getSize()
	{
		if (sizeUpdateTag_)
		{
			RECT rc;
			GetClientRect(hWnd, &rc);

			width_ = rc.right - rc.left;
			height_ = rc.bottom - rc.top;

			if (width_ == 0 || height_ == 0)
			{
				WARNINGWINDOW_GLOBAL("Window size is zero in GameWindow::getSize().\n Window may not ready.");
				return { 0, 0 };
			}
			else
			{
				sizeUpdateTag_ = false;
				return { width_, height_ };
			}
		}
		else
		{
			return { width_, height_ };
		}
	}

	bool GameWindow::isMaximized()
	{
		return IsZoomed(hWnd) != 0;
	}

	bool GameWindow::isMinimized()
	{
		return IsIconic(hWnd) != 0;
	}

	void GameWindow::setLogicalSize(UINT _width, UINT _height)
	{
		logicalWidth_ = _width;
		logicalHeight_ = _height;
		pixelMappingTransformUpdateTag_ = true;
	}

	void GameWindow::resetDisplaySizeToDefault()
	{
		RECT rc = { 0, 0,
			static_cast<LONG>(suku::GameWindow::defaultDisplayWidth),
			static_cast<LONG>(suku::GameWindow::defaultDisplayHeight) };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
		int windowWidth = rc.right - rc.left;
		int windowHeight = rc.bottom - rc.top;
		int screenW = GetSystemMetrics(SM_CXSCREEN);
		int screenH = GetSystemMetrics(SM_CYSCREEN);
		int windowX = (screenW - windowWidth) / 2;
		int windowY = (screenH - windowHeight) / 2;

		SendMessage(hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
		MoveWindow(hWnd, windowX, windowY,
			windowWidth, windowHeight,
			TRUE);
	}

	void GameWindow::setFullscreen(bool _isFullscreen)
	{
		ConfigElementPool::isFullScreen = _isFullscreen;
		if (isFullscreen_ == _isFullscreen)
			return;
		DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
		if (_isFullscreen == true)
		{
			MONITORINFO mi = { sizeof(mi) };
			if (GetWindowPlacement(hWnd, &previousWindowPlacement_)
				&& GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY), &mi))
			{
				SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(hWnd, HWND_TOP,
					mi.rcMonitor.left, mi.rcMonitor.top,
					mi.rcMonitor.right - mi.rcMonitor.left,
					mi.rcMonitor.bottom - mi.rcMonitor.top,
					SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
			}
			isFullscreen_ = true;
		}
		else
		{
			SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
			SetWindowPlacement(hWnd, &previousWindowPlacement_);
			SetWindowPos(hWnd, NULL, 0, 0, 0, 0,
				SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
				SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
			isFullscreen_ = false;
		}
	}

	void GameWindow::refreshSizeInfo()
	{
		sizeUpdateTag_ = true;
	}

	void GameWindow::refreshPositionInfo()
	{
		positionUpdateTag_ = true;
	}

	void GameWindow::refreshPositionInfo(int _posX, int _posY)
	{
		if (!isMaximized())
		{
			suku::ConfigElementPool::windowPosX = _posX;
			suku::ConfigElementPool::windowPosY = _posY;
		}
		x_ = _posX;
		y_ = _posY;
		positionUpdateTag_ = false;
	}

	void GameWindow::onMinimize()
	{
	}

	void GameWindow::onMaximize(bool _isMaximized)
	{
		if (_isMaximized)
			suku::ConfigElementPool::isMaximized = true;
		else
			suku::ConfigElementPool::isMaximized = false;
	}

	void GameWindow::onFocusChanged(bool _isFocused)
	{
		if (_isFocused == true)
		{
			isFocused_ = true;
		}
		else
		{
			isFocused_ = false;
		}
	}

	Transform GameWindow::getPixelMappingTransform()
	{
		if (sizeUpdateTag_)
		{
			// refresh to make {width_, height_} is right
			getSize();

			if (!isMaximized())
			{
				ConfigElementPool::windowWidth = width_;
				ConfigElementPool::windowHeight = height_;
			}

			pixelMappingTransformUpdateTag_ = true;
		}
		if (pixelMappingTransformUpdateTag_)
		{
			float scaleX = static_cast<float>(width_) / static_cast<float>(logicalWidth_);
			float scaleY = static_cast<float>(height_) / static_cast<float>(logicalHeight_);
			float centerX = static_cast<float>(width_) / 2;
			float centerY = static_cast<float>(height_) / 2;
			float logicalCenterX = static_cast<float>(logicalWidth_) / 2;
			float logicalCenterY = static_cast<float>(logicalHeight_) / 2;
			float actualScale = std::min(scaleX, scaleY);
			pixelMappingTransform_ = translation(centerX - logicalCenterX, centerY - logicalCenterY)
				+ scale(logicalCenterX, logicalCenterY, actualScale, actualScale);
			pixelMappingTransformUpdateTag_ = false;
		}
		return pixelMappingTransform_;
	}
}
