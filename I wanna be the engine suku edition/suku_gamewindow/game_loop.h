#pragma once
#include <suku_config/config_element.h>

namespace suku
{
	double getUpdateFPS();
	double getRenderFPS();
	void endGame();

	class GameLoopSender
	{
	public:
		GameLoopSender(const GameLoopSender&) = delete;
		GameLoopSender& operator=(const GameLoopSender&) = delete;

		static GameLoopSender& getInstance() { static GameLoopSender instance; return instance; }
		void start();

	private:
		GameLoopSender() = default;
	};

	class GameLoopVsyncSender
	{
	public:
		GameLoopVsyncSender(const GameLoopVsyncSender&) = delete;
		GameLoopVsyncSender& operator=(const GameLoopVsyncSender&) = delete;

		static GameLoopVsyncSender& getInstance() { static GameLoopVsyncSender instance; return instance; }
		void start(int _frameRate);
		int getFrameRate()const { return frameRate_; }
	private:
		int frameRate_ = 1;
		GameLoopVsyncSender() = default;
	};

	class GameLoopConfig
	{
	public:
		// Render
		inline static ConfigElement<int> renderFPS = { "Render", "fps", 50, {50, 60, 90, 100, 120, 144, 160, 200, 240} };
		inline static ConfigElement<bool> isVSyncOn = { "Render", "vsync", true };
		inline static ConfigElement<int> vsyncFrameRate = { "Render", "vsyncFrameRate", 1, 1, 4 };
	};
}