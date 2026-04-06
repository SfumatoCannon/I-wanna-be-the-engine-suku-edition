#include "pch.h"
#include "input_mouse.h"
#include "input.h"
#include "game_window.h"
#include <windowsx.h>

namespace suku::input
{
	constexpr float mouseButtonClickTimeThreshold = 0.4f; // seconds
	static bool isMouseButtonDownArray[4] = { false };
	static bool isMouseButtonUpArray[4] = { false };
	static bool isMouseButtonHoldingArray[4] = { false };
	static bool isMouseButtonDoubleClick = false;
	static bool isMouseScrollUp = false;
	static bool isMouseScrollDown = false;
	static int mouseButtonClickTimerArray[4] = { 0 };
	static std::queue<std::pair<UINT, USHORT> > mouseButtonMsg;

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
		return getPosition().x;
	}

	float Mouse::getY()
	{
		return getPosition().y;
	}

	Vector Mouse::getPosition()
	{
		return GameWindow::getPixelMappingTransform().invertTransform().transformPoint(pixelX_, pixelY_);
	}

	void Mouse::refreshPosition(LPARAM _msgLParam)
	{
		pixelX_ = GET_X_LPARAM(_msgLParam);
		pixelY_ = GET_Y_LPARAM(_msgLParam);
	}

	void Mouse::resetButtonState()
	{
		memset(isMouseButtonDownArray, false, sizeof(isMouseButtonDownArray));
		memset(isMouseButtonUpArray, false, sizeof(isMouseButtonUpArray));
		isMouseButtonDoubleClick = false;
		for (int i = 0; i < 4; i++)
		{
			if (mouseButtonClickTimerArray[i] > 0)
				mouseButtonClickTimerArray[i]--;
		}
		isMouseScrollUp = false;
		isMouseScrollDown = false;
	}

	void Mouse::frameStateUpdate()
	{
		UINT message;
		WPARAM buttonCode;
		while (!mouseButtonMsg.empty())
		{
			message = mouseButtonMsg.front().first;
			buttonCode = mouseButtonMsg.front().second;
			mouseButtonMsg.pop();
			switch (message)
			{
			case MOUSE_INPUT_DOWN:
				if (!isMouseButtonHoldingArray[buttonCode])
					isMouseButtonDownArray[buttonCode] = true;
				isMouseButtonHoldingArray[buttonCode] = true;
				isMouseButtonUpArray[buttonCode] = false;
				mouseButtonClickTimerArray[buttonCode] = (int)(mouseButtonClickTimeThreshold / 0.02f); // 0.02f -> Window::frameRate, WIP
				break;
			case MOUSE_INPUT_UP:
				isMouseButtonHoldingArray[buttonCode] = false;
				isMouseButtonUpArray[buttonCode] = true;
				break;
			case MOUSE_INPUT_DOUBLECLICK:
				isMouseButtonDoubleClick = true;
				break;
			case MOUSE_INPUT_SCROLLUP:
				isMouseScrollUp = true;
				break;
			case MOUSE_INPUT_SCROLLDOWN:
				isMouseScrollDown = true;
				break;
			default:
				break;
			}
		}
	}

	void Mouse::pushMouseMessage(UINT _message, USHORT _buttonVCode)
	{
		mouseButtonMsg.push(std::make_pair(_message, _buttonVCode));
	}

	bool Mouse::isLeftButtonDown()
	{
		return isMouseButtonDownArray[MOUSE_VK_LEFT];
	}

	bool Mouse::isLeftButtonUp()
	{
		return isMouseButtonUpArray[MOUSE_VK_LEFT];
	}

	bool Mouse::isLeftButtonHolding()
	{
		return isMouseButtonHoldingArray[MOUSE_VK_LEFT];
	}

	bool Mouse::isLeftButtonClick()
	{
		return isMouseButtonUpArray[MOUSE_VK_LEFT] && mouseButtonClickTimerArray[MOUSE_VK_LEFT] > 0;
	}

	bool Mouse::isRightButtonDown()
	{
		return isMouseButtonDownArray[MOUSE_VK_RIGHT];
	}

	bool Mouse::isRightButtonUp()
	{
		return isMouseButtonUpArray[MOUSE_VK_RIGHT];
	}

	bool Mouse::isRightButtonHolding()
	{
		return isMouseButtonHoldingArray[MOUSE_VK_RIGHT];
	}

	bool Mouse::isRightButtonClick()
	{
		return isMouseButtonUpArray[MOUSE_VK_RIGHT] && mouseButtonClickTimerArray[MOUSE_VK_RIGHT] > 0;
	}
	
	bool Mouse::isMiddleButtonDown()
	{
		return isMouseButtonDownArray[MOUSE_VK_MIDDLE];
	}

	bool Mouse::isMiddleButtonUp()
	{
		return isMouseButtonUpArray[MOUSE_VK_MIDDLE];
	}
	
	bool Mouse::isMiddleButtonHolding()
	{
		return isMouseButtonHoldingArray[MOUSE_VK_MIDDLE];
	}

	bool Mouse::isMiddleButtonClick()
	{
		return isMouseButtonUpArray[MOUSE_VK_MIDDLE] && mouseButtonClickTimerArray[MOUSE_VK_MIDDLE] > 0;
	}

	bool Mouse::isScrollUp()
	{
		return isMouseScrollUp;
	}

	bool Mouse::isScrollDown()
	{
		return isMouseScrollDown;
	}

	bool Mouse::isDoubleClick()
	{
		return isMouseButtonDoubleClick;
	}
}