#include "pch.h"
#include "mouse.h"
#include "game_window.h"
#include <windowsx.h>

namespace suku
{
	int Mouse::getPixelX()
	{
		return pixelX_;
	}

	int Mouse::getPixelY()
	{
		return pixelY_;
	}

	float Mouse::getX()
	{
		return (float)pixelX_ * GameWindow::getLogicalWidth() / (float)GameWindow::getWidth();
	}

	float Mouse::getY()
	{
		return (float)pixelY_ * GameWindow::getLogicalHeight() / (float)GameWindow::getHeight();
	}

	void Mouse::refreshPosition(LPARAM _msgLParam)
	{
		pixelX_ = GET_X_LPARAM(_msgLParam);
		pixelY_ = GET_Y_LPARAM(_msgLParam);
	}
}