#pragma once

namespace suku
{
	class Mouse
	{
	public:
		static int getPixelX();
		static int getPixelY();
		static float getX(); // Logical coordinate
		static float getY(); // Logical coordinate
		static void refreshPosition(LPARAM _msgLParam);
	private:
		inline static int pixelX_ = 0;
		inline static int pixelY_ = 0;
	};
}