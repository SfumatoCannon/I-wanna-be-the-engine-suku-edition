#pragma once

namespace suku::input
{
	class Mouse
	{
	public:
		static int getPixelX();
		static int getPixelY();
		static float getX(); // Logical coordinate
		static float getY(); // Logical coordinate
		static void refreshPosition(LPARAM _msgLParam);
		static void frameStateUpdate();
		static void onLeftButtonDown();
		static void onLeftButtonUp();
		static void onRightButtonDown();
		static void onRightButtonUp();
		static void onMiddleButtonDown();
		static void onMiddleButtonUp();

	private:
		inline static int pixelX_ = 0;
		inline static int pixelY_ = 0;
		inline static bool isLeftButtonDown_ = false;
		inline static bool isLeftButtonUp_ = false;
		inline static bool isLeftButtonHolding_ = false;
		inline static int leftButtonDownDuration_ = 0;

	};
}