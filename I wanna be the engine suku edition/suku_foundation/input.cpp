#include "input.h"

namespace suku
{
	bool isKeyDown[256] = { false };
	bool isKeyHolding[256] = { false };
	bool isKeyUp[256] = { false };
	std::queue<std::pair<UINT, WPARAM> > keyMsg;

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