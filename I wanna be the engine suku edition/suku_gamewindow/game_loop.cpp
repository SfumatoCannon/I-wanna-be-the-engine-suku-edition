#include "pch.h"
#include "game_loop.h"
#include "game_window.h"
#include "../suku_foundation/includes.h"
#include "../suku_core/includes.h"
#include "../suku_draw/includes.h"


bool gameEndFlag = false;
void endGame() { gameEndFlag = true; }

constexpr double updateFPS = 50.0;
double renderFPS = 240.0;

void vsyncLoopSender()
{
	const double frameTime = 1000.0 / updateFPS;
	auto framePeriod = std::chrono::duration<double, std::milli>(frameTime);

	auto next = std::chrono::steady_clock::now();
	while (!gameEndFlag)
	{
		next += std::chrono::duration_cast<std::chrono::steady_clock::duration>(framePeriod);
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

// WIP
std::thread threadVsyncUpdate;
std::thread threadUpdate;
std::thread threadRender;

std::chrono::steady_clock::time_point lastUpdateTime;

void updateSender()
{
	const double frameTime = 1000.0 / updateFPS;
	auto framePeriod = std::chrono::duration<double, std::milli>(frameTime);

	auto next = std::chrono::steady_clock::now();
	while (!gameEndFlag)
	{
		next += std::chrono::duration_cast<std::chrono::steady_clock::duration>(framePeriod);
		std::this_thread::sleep_until(next);
		lastUpdateTime = std::chrono::steady_clock::now();
		updateWork();
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
	const double frameTime = 1000.0 / renderFPS;
	auto framePeriod = std::chrono::duration<double, std::milli>(frameTime);

	auto next = std::chrono::steady_clock::now();
	while (!gameEndFlag)
	{
		next += std::chrono::duration_cast<std::chrono::steady_clock::duration>(framePeriod);
		std::this_thread::sleep_until(next);
		auto nowTime = std::chrono::steady_clock::now();
		double additionalFrameRate 
			= std::chrono::duration<double, std::milli>(nowTime - lastUpdateTime).count() / updateFrameTime;
		paintWork(additionalFrameRate);
		if (gameEndFlag)
		{
			PostMessage(suku::GameWindow::hWnd, WM_QUIT, NULL, NULL);
			break;
		}
	}
}

void startSender()
{
	std::thread threadUpdate(updateSender);
	threadUpdate.detach();
	std::thread threadRender(renderSender);
	threadRender.detach();
}

void startSenderVsync()
{
	std::thread thread(vsyncLoopSender);
	thread.detach();
}

//std::mutex threadLock;

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

		suku::Text a("Consolas", 16);
		a.setBrush(suku::graphics::createSolidColorBrush(suku::Color(255, 255, 255, 1.0f)));
		a.textContent = L"FPS: " + std::to_wstring(renderFPS);
		a.paint(10, 10);
		suku::graphics::endDrawGlobal();
	}
}

void paintWork(double _additionalFrameRate)
{
	double renderFPS = 0.0f;
	//if (threadLock.try_lock())
	//{
	if (!gameEndFlag)
	{
		renderFPS = getMonitoredFPS(true);

		suku::graphics::beginDrawGlobal();
		if (_additionalFrameRate == 0.0)
			suku::nowRoom->paint();
		else
			suku::nowRoom->additionalFramePaint((float)_additionalFrameRate);

		suku::Text a("Consolas", 16);
		a.setBrush(suku::graphics::createSolidColorBrush(suku::Color(255, 255, 255, 1.0f)));
		a.textContent = L"FPS: " + std::to_wstring(renderFPS);
		a.paint(10, 10);
		suku::graphics::endDrawGlobal();
	}
	//	threadLock.unlock();
	//}
}

BOOL monitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	MONITORINFOEX mi;
	mi.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(hMonitor, &mi);
	DEVMODE devmode;
	devmode.dmSize = sizeof(DEVMODE);
	EnumDisplaySettings(mi.szDevice, ENUM_CURRENT_SETTINGS, &devmode);
	if (devmode.dmDisplayFrequency != updateFPS)
	{
		devmode.dmDisplayFlags &= DM_INTERLACED;
		devmode.dmDisplayFrequency = (DWORD)updateFPS;
		LONG res = ChangeDisplaySettingsEx(mi.szDevice, &devmode, nullptr, 0, nullptr);
	}
	return true;
}

//void setFPS(int _fps)
//{
//	fps = _fps;
//	EnumDisplayMonitors(nullptr, nullptr, monitorEnumProc, NULL);
//}