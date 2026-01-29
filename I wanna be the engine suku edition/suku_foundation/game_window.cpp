#include "pch.h"
#include "game_window.h"
#include "message.h"
#include "../suku_draw/draw_core.h"

namespace suku
{
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

    void GameWindow::setLogicalSize(UINT _width, UINT _height)
    {
        logicalWidth_ = _width;
		logicalHeight_ = _height;
        pixelMappingTransformUpdateTag_ = true;
    }

    void GameWindow::setFullscreen(bool _isFullscreen)
    {
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

    void GameWindow::RefreshSizeInfo()
    {
		sizeUpdateTag_ = true;
    }

    Transform GameWindow::getPixelMappingTransform()
    {
        if (sizeUpdateTag_)
        {
			// refresh to make {width_, height_} is right
            getSize();
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
