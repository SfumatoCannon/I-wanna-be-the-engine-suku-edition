#pragma once
#include <queue>
#include <windows.h>


namespace suku::input
{
	bool isKeyDown(UINT8 _keyVCode);
	bool isKeyUp(UINT8 _keyVCode);
	bool isKeyHolding(UINT8 _keyVCode);
	std::pair<UINT, USHORT> onWindowInput(LPARAM _lParam);
	void pushKeyMessage(UINT _message, USHORT _keyVCode);
	void frameStateUpdate();
	void resetKeyState();

	constexpr USHORT VK_0 = 0x30;
	constexpr USHORT VK_1 = 0x31;
	constexpr USHORT VK_2 = 0x32;
	constexpr USHORT VK_3 = 0x33;
	constexpr USHORT VK_4 = 0x34;
	constexpr USHORT VK_5 = 0x35;
	constexpr USHORT VK_6 = 0x36;
	constexpr USHORT VK_7 = 0x37;
	constexpr USHORT VK_8 = 0x38;
	constexpr USHORT VK_9 = 0x39;
	constexpr USHORT VK_A = 0x41;
	constexpr USHORT VK_B = 0x42;
	constexpr USHORT VK_C = 0x43;
	constexpr USHORT VK_D = 0x44;
	constexpr USHORT VK_E = 0x45;
	constexpr USHORT VK_F = 0x46;
	constexpr USHORT VK_G = 0x47;
	constexpr USHORT VK_H = 0x48;
	constexpr USHORT VK_I = 0x49;
	constexpr USHORT VK_J = 0x4A;
	constexpr USHORT VK_K = 0x4B;
	constexpr USHORT VK_L = 0x4C;
	constexpr USHORT VK_M = 0x4D;
	constexpr USHORT VK_N = 0x4E;
	constexpr USHORT VK_O = 0x4F;
	constexpr USHORT VK_P = 0x50;
	constexpr USHORT VK_Q = 0x51;
	constexpr USHORT VK_R = 0x52;
	constexpr USHORT VK_S = 0x53;
	constexpr USHORT VK_T = 0x54;
	constexpr USHORT VK_U = 0x55;
	constexpr USHORT VK_V = 0x56;
	constexpr USHORT VK_W = 0x57;
	constexpr USHORT VK_X = 0x58;
	constexpr USHORT VK_Y = 0x59;
	constexpr USHORT VK_Z = 0x5A;

	constexpr UINT INPUT_KEYDOWN = 0x01;
	constexpr UINT INPUT_KEYUP = 0x02;
}