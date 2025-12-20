#pragma once
namespace suku
{
	constexpr const wchar_t* SaveDir = L"Data\\Save";
	constexpr UINT WindowWidth = 800;
	constexpr UINT WindowHeight = 608;
	constexpr UINT WindowX = 100;
	constexpr UINT WindowY = 50;

	enum Direction
	{
		Left,
		Right,
		Up,
		Down
	};
}