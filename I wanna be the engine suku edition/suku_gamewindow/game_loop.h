#pragma once

namespace suku
{
	void endGame();
	namespace game_loop
	{
		constexpr double updateFPS = 50.0;
		void start();
		void startWithVsync();
	}
}