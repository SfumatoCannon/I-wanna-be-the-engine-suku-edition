#include "input.h"

namespace suku::input
{
	bool isKeyDownArray[256] = { false };
	bool isKeyHoldingArray[256] = { false };
	bool isKeyUpArray[256] = { false };
	std::queue<std::pair<UINT, WPARAM> > keyMsg;

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

	void onWindowInput(LPARAM _lParam)
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
					pushKeyMessage(INPUT_KEYDOWN, vKey);
				else // isKeyUpArray
					pushKeyMessage(INPUT_KEYUP, vKey);
			}
		}
	}

	void pushKeyMessage(UINT _message, WPARAM _wParam)
	{
		keyMsg.push(std::make_pair(_message, _wParam));
	}

	void keyCheck()
	{
		UINT _message;
		WPARAM _wParam;
		while (!keyMsg.empty())
		{
			_message = keyMsg.front().first;
			_wParam = keyMsg.front().second;
			keyMsg.pop();
			switch (_message)
			{
			case INPUT_KEYDOWN:
				if (!isKeyHoldingArray[_wParam])
					isKeyDownArray[_wParam] = true;
				isKeyHoldingArray[_wParam] = true;
				isKeyUpArray[_wParam] = false;
				break;
			case INPUT_KEYUP:
				isKeyHoldingArray[_wParam] = false;
				isKeyUpArray[_wParam] = true;
				break;
			default:
				break;
			}
		}
	}

	void resetKey()
	{
		memset(isKeyDownArray, false, sizeof(isKeyDownArray));
		memset(isKeyUpArray, false, sizeof(isKeyUpArray));
	}
}