#pragma once
#include "framework.h"
namespace suku
{
	constexpr const wchar_t* SaveDir = L"Data\\Save";
	constexpr int WindowWidth = 800;
	constexpr int WindowHeight = 608;
	constexpr int WindowX = 100;
	constexpr int WindowY = 50;

	enum Direction
	{
		Left,
		Right,
		Up,
		Down
	};
}