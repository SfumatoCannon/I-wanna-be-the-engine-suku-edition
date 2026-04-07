#include "input.h"

namespace suku::input
{
	static bool isKeyDownArray[256] = { false };
	static bool isKeyHoldingArray[256] = { false };
	static bool isKeyUpArray[256] = { false };
	static std::queue<std::pair<UINT, USHORT> > keyMsg;

	bool isKeyDown(UINT8 _keyVCode)
	{
		return isKeyDownArray[_keyVCode];
	}

	bool isKeyUp(UINT8 _keyVCode)
	{
		return isKeyUpArray[_keyVCode];
	}

	bool isKeyHolding(UINT8 _keyVCode)
	{
		return isKeyHoldingArray[_keyVCode];
	}

	std::pair<UINT,USHORT> onWindowInput(LPARAM _lParam)
	{
		UINT dwSize = 0;
		GetRawInputData((HRAWINPUT)_lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
		std::vector<BYTE> lpb(dwSize);
		if (GetRawInputData((HRAWINPUT)_lParam, RID_INPUT, lpb.data(), &dwSize, sizeof(RAWINPUTHEADER)) == dwSize)
		{
			RAWINPUT* raw = (RAWINPUT*)lpb.data();

			if (raw->header.dwType == RIM_TYPEKEYBOARD)
			{
				const RAWKEYBOARD& kb = raw->data.keyboard;
				USHORT vKey = kb.VKey;
				bool isKeyUpArray = (kb.Flags & RI_KEY_BREAK) || (kb.Message == WM_KEYUP || kb.Message == WM_SYSKEYUP);
				bool isKeyDown = !isKeyUpArray;
				if (isKeyDown)
				{
					pushKeyMessage(INPUT_KEYDOWN, vKey);
					return { INPUT_KEYDOWN, vKey };
				}
				else // isKeyUpArray
				{
					pushKeyMessage(INPUT_KEYUP, vKey);
					return { INPUT_KEYUP, vKey };
				}
			}
		}
		return { 0, 0 };
	}

	void pushKeyMessage(UINT _message, USHORT _keyVCode)
	{
		keyMsg.push(std::make_pair(_message, _keyVCode));
	}

	void frameStateUpdate()
	{
		UINT message;
		USHORT keyCode;
		while (!keyMsg.empty())
		{
			message = keyMsg.front().first;
			keyCode = keyMsg.front().second;
			keyMsg.pop();
			switch (message)
			{
			case INPUT_KEYDOWN:
				if (!isKeyHoldingArray[keyCode])
					isKeyDownArray[keyCode] = true;
				isKeyHoldingArray[keyCode] = true;
				isKeyUpArray[keyCode] = false;
				break;
			case INPUT_KEYUP:
				isKeyHoldingArray[keyCode] = false;
				isKeyUpArray[keyCode] = true;
				break;
			default:
				break;
			}
		}
	}

	void resetKeyState()
	{
		memset(isKeyDownArray, false, sizeof(isKeyDownArray));
		memset(isKeyUpArray, false, sizeof(isKeyUpArray));
	}
}