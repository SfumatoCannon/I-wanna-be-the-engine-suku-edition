#pragma once
#include "type_tree.h"

#define MOUSE_VK_LEFT 0x01
#define MOUSE_VK_RIGHT 0x02
#define MOUSE_VK_MIDDLE 0x03

#define MOUSE_INPUT_DOWN 0x01
#define MOUSE_INPUT_UP 0x02
#define MOUSE_INPUT_DOUBLECLICK 0x03
#define MOUSE_INPUT_SCROLLUP 0x04
#define MOUSE_INPUT_SCROLLDOWN 0x05
//Pending
//#define MOUSE_INPUT_SCROLLLEFT 0x06
//#define MOUSE_INPUT_SCROLLRIGHT 0x07

namespace suku::input
{
	class Mouse
	{
	public:
		static int getPixelX();
		static int getPixelY();
		static float getX(); // Logical coordinate
		static float getY(); // Logical coordinate
		static Vector getPosition();
		static void refreshPosition(LPARAM _msgLParam);
		static void resetButtonState();
		static void frameStateUpdate();
		static void pushMouseMessage(UINT _message, USHORT _buttonVCode);

		static bool isLeftButtonDown();
		static bool isLeftButtonUp();
		static bool isLeftButtonHolding();
		static bool isLeftButtonClick();
		static bool isRightButtonDown();
		static bool isRightButtonUp();
		static bool isRightButtonHolding();
		static bool isRightButtonClick();
		static bool isMiddleButtonDown();
		static bool isMiddleButtonUp();
		static bool isMiddleButtonHolding();
		static bool isMiddleButtonClick();
		static bool isScrollUp();
		static bool isScrollDown();
		static bool isDoubleClick();

	private:
		inline static int pixelX_ = 0;
		inline static int pixelY_ = 0;
	};
}