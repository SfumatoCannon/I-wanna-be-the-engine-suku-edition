#pragma once

namespace suku
{
	void endGame();
	namespace game_loop
	{
		constexpr double updateFPS = 50.0;
		void debugMessageDisplay(bool _isDisplay);
		void start();
		void startWithVsync(int _frameRate = 1);
		double getRenderFPS();
		void setRenderFPS(double _fps);
	}
}