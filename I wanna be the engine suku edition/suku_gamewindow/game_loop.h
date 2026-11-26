#pragma once
#include <suku_config/config_element.h>
#include <thread>
#include <memory>

namespace suku
{
	double getUpdateFPS();
	double getRenderFPS();
	void endGame();

	enum class GameLoopMode
	{
		Normal,
		Vsync
	};

	class GeneralGameLoopController
	{
	public:
		static GameLoopMode getMode();
		static void stop();
	private:
		GeneralGameLoopController() = delete;
		GeneralGameLoopController(const GeneralGameLoopController&) = delete;
	};

	class GameLoopSender
	{
	public:
		GameLoopSender(const GameLoopSender&) = delete;
		GameLoopSender& operator=(const GameLoopSender&) = delete;

		static GameLoopSender& getInstance() { static GameLoopSender instance; return instance; }
		void start();
		void stop();

	private:
		GameLoopSender() = default;
		std::unique_ptr<std::jthread> pThreadUpdateLoop_ = nullptr;
		std::unique_ptr<std::jthread> pThreadRenderLoop_ = nullptr;
	};

	class GameLoopVsyncSender
	{
	public:
		GameLoopVsyncSender(const GameLoopVsyncSender&) = delete;
		GameLoopVsyncSender& operator=(const GameLoopVsyncSender&) = delete;

		static GameLoopVsyncSender& getInstance() { static GameLoopVsyncSender instance; return instance; }
		void start(int _frameRate);
		void stop();
		int getFrameRate()const { return frameRate_; }
	private:
		GameLoopVsyncSender() = default;
		int frameRate_ = 1;
		std::unique_ptr<std::jthread> pThreadUpdateRenderLoop_ = nullptr;
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