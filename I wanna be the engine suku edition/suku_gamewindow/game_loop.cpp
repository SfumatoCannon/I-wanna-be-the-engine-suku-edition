#include "pch.h"
#include "game_loop.h"
#include "game_window.h"
#include "../suku_foundation/includes.h"
#include "../suku_core/includes.h"
#include "../suku_draw/includes.h"
#include "../suku_config/includes.h"

// Private functions declaration
// ----------------------------------------------------------------------------
namespace 
{
	bool debugMessage = true;
	bool gameEndFlag = false;
	void updateWork();
	void paintWork();
	void paintWork(double _additionalFrameRate);
	double getMonitoredFPS(bool _isUpdate);
}
// ----------------------------------------------------------------------------
// End of private functions declaration

namespace suku
{
	void endGame() { gameEndFlag = true; }

	namespace game_loop
	{
		double renderFPS = 240.0;

		void vsyncLoopSender()
		{
			const double frameTime = 1000.0 / updateFPS;
			auto frameDuration = std::chrono::duration_cast<std::chrono::steady_clock::duration>
				(std::chrono::duration<double, std::milli>(frameTime));

			auto next = std::chrono::steady_clock::now();
			while (!gameEndFlag)
			{
				next += frameDuration;
				std::this_thread::sleep_until(next);
				updateWork();
				paintWork();
				if (gameEndFlag)
				{
					PostMessage(suku::GameWindow::hWnd, WM_QUIT, NULL, NULL);
					break;
				}
			}
		}

		void vsyncLoopSenderExtraFrames(int _frameRate)
		{
			const double frameTime = 1000.0 / updateFPS;
			auto frameDuration = std::chrono::duration_cast<std::chrono::steady_clock::duration>
				(std::chrono::duration<double, std::milli>(frameTime) / (double)_frameRate);

			auto next = std::chrono::steady_clock::now();
			while (!gameEndFlag)
			{
				next += frameDuration;
				std::this_thread::sleep_until(next);
				updateWork();
				for (int i = 1; i < _frameRate; i++)
				{
					paintWork(1.0 / (double)_frameRate * i);
					next += frameDuration;
					std::this_thread::sleep_until(next);
				}
				paintWork(1.0);
				if (gameEndFlag)
				{
					PostMessage(suku::GameWindow::hWnd, WM_QUIT, NULL, NULL);
					break;
				}
			}
		}

		// WIP
		std::thread threadVsyncUpdate;
		std::thread threadUpdate;
		std::thread threadRender;

		std::atomic<std::chrono::steady_clock::time_point> lastUpdateTime;

		void updateSender()
		{
			const double frameTime = 1000.0 / updateFPS;
			auto frameDuration = std::chrono::duration_cast<std::chrono::steady_clock::duration>
				(std::chrono::duration<double, std::milli>(frameTime));

			auto next = std::chrono::steady_clock::now();
			while (!gameEndFlag)
			{
				next += frameDuration;
				std::this_thread::sleep_until(next);
				updateWork();
				lastUpdateTime.store(std::chrono::steady_clock::now());
				if (gameEndFlag)
				{
					PostMessage(suku::GameWindow::hWnd, WM_QUIT, NULL, NULL);
					break;
				}
			}
		}

		void renderSender()
		{
			const double updateFrameTime = 1000.0 / updateFPS;
			double frameTime = 1000.0 / (double)suku::ConfigElementPool::renderFPS.value();
			auto frameDuration = std::chrono::duration_cast<std::chrono::steady_clock::duration>
				(std::chrono::duration<double, std::milli>(frameTime));

			auto next = std::chrono::steady_clock::now();
			while (!gameEndFlag)
			{
				next += frameDuration;
				std::this_thread::sleep_until(next);
				auto nowTime = std::chrono::steady_clock::now();
				double additionalFrameRate
					= std::chrono::duration<double, std::milli>(nowTime - lastUpdateTime.load()).count() / updateFrameTime;
				paintWork(additionalFrameRate);
				if (gameEndFlag)
				{
					PostMessage(suku::GameWindow::hWnd, WM_QUIT, NULL, NULL);
					break;
				}
			}
		}

		void debugMessageDisplay(bool _isDisplay)
		{
			debugMessage = _isDisplay;
		}

		void start()
		{
			std::thread threadUpdate(updateSender);
			threadUpdate.detach();
			std::thread threadRender(renderSender);
			threadRender.detach();
		}

		void startWithVsync(int _frameRate)
		{
			if (_frameRate <= 1)
			{
				std::thread thread(vsyncLoopSender);
				thread.detach();
			}
			else
			{
				std::thread thread(vsyncLoopSenderExtraFrames, _frameRate);
				thread.detach();
			}
		}
	}
}































// Private functions implementation
// ----------------------------------------------------------------------------
namespace
{
	void updateWork()
	{
		using namespace suku;
		//if (threadLock.try_lock())
		//{
		suku::input::frameStateUpdate();
		suku::input::Mouse::frameStateUpdate();

		if (suku::input::isKeyDown(VK_ESCAPE) && !gameEndFlag)
			endGame();

		if (nowRoom)
			nowRoom->update();

		suku::input::resetKeyState();
		suku::input::Mouse::resetButtonState();

		//	threadLock.unlock();
		//}
	}

	double getMonitoredFPS(bool _isUpdate = false)
	{
		static LARGE_INTEGER prevCounter = { 0 };
		static LARGE_INTEGER frequency = { 0 };
		static int frameCount = 0;
		static double accumSeconds = 0.0;
		static double lastReportedFPS = 0.0;
		static auto initializeFuction = []()->bool
			{
				QueryPerformanceFrequency(&frequency);
				QueryPerformanceCounter(&prevCounter);
				return true;
			}();

		if (_isUpdate == false)
			return lastReportedFPS;

		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		double delta = 0.0;
		if (frequency.QuadPart != 0)
			delta = double(now.QuadPart - prevCounter.QuadPart) / double(frequency.QuadPart);
		prevCounter = now;

		frameCount++;
		accumSeconds += delta;
		if (accumSeconds >= 1.0)
		{
			lastReportedFPS = double(frameCount) / accumSeconds;
			frameCount = 0;
			accumSeconds = 0.0;
		}

		return lastReportedFPS;
	}

	void paintWork()
	{
		double renderFPS = 0.0f;
		if (!gameEndFlag)
		{
			renderFPS = getMonitoredFPS(true);

			suku::graphics::beginDrawGlobal();
			suku::nowRoom->paint();

			if (debugMessage)
			{
				static suku::Text a("Consolas", 16);
				a.setBrush(suku::graphics::createSolidColorBrush(suku::Color(255, 255, 255, 1.0f)));
				a.textContent = L"FPS: " + std::to_wstring(renderFPS);
				a.paint(10, 10);
			}

			suku::graphics::endDrawGlobal();
		}
	}

	void paintWork(double _additionalFrameRate)
	{
		double renderFPS = 0.0f;
		if (!gameEndFlag)
		{
			renderFPS = getMonitoredFPS(true);

			suku::graphics::beginDrawGlobal();
			suku::nowRoom->additionalFramePaint((float)_additionalFrameRate);

			if (debugMessage)
			{
				static suku::Text a("Consolas", 16);
				if (_additionalFrameRate > 1.0)
					a.setBrush(suku::graphics::createSolidColorBrush(suku::Color(255, 0, 0, 1.0f)));
				else
					a.setBrush(suku::graphics::createSolidColorBrush(suku::Color(0, 0, 0, 1.0f)));
				a.textContent = L"FPS: " + std::to_wstring(renderFPS)
					+ L"\nAdditional Frame Rate: " + std::to_wstring(_additionalFrameRate);
				a.paint(10, 10);
			}

			suku::graphics::endDrawGlobal();
		}
	}
}
// ----------------------------------------------------------------------------
// End of private functions implementation