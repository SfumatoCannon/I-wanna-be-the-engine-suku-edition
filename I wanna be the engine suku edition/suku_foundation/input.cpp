#include "input.h"

namespace suku
{
	bool isKeyDown[256] = { false };
	bool isKeyHolding[256] = { false };
	bool isKeyUp[256] = { false };
	std::queue<std::pair<UINT, WPARAM> > keyMsg;

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
				bool isKeyUp = (kb.Flags & RI_KEY_BREAK) || (kb.Message == WM_KEYUP || kb.Message == WM_SYSKEYUP);
				bool isKeyDown = !isKeyUp;
				if (isKeyDown)
					pushKeyMessage(INPUT_KEYDOWN, vKey);
				else // isKeyUp
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
				if (!isKeyHolding[_wParam])
					isKeyDown[_wParam] = true;
				isKeyHolding[_wParam] = true;
				isKeyUp[_wParam] = false;
				break;
			case INPUT_KEYUP:
				isKeyHolding[_wParam] = false;
				isKeyUp[_wParam] = true;
				break;
			default:
				break;
			}
		}
	}

	void resetKey()
	{
		memset(isKeyDown, false, sizeof(isKeyDown));
		memset(isKeyUp, false, sizeof(isKeyUp));
	}
}