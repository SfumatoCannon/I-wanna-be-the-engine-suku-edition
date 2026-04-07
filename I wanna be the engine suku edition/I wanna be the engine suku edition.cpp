#include "pch.h"
#include "global_value.h"
#include "I wanna be the engine suku edition.h"
#include <chrono>
#include <thread>

#define MAX_LOADSTRING 100
#define WM_CREATEFINISHED (WM_USER + 1)

HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

bool gameEndFlag = false;
void endGame() { gameEndFlag = true; }

constexpr double updateFPS = 50.0;
double renderFPS = 50.0;

void Sender()
{
	const double updateFrameTime = 1000.0 / updateFPS;
	const double renderFrameTime = 1000.0 / renderFPS;

	auto updatePeriod = std::chrono::duration<double, std::milli>(updateFrameTime);
	auto renderPeriod = std::chrono::duration<double, std::milli>(renderFrameTime);
	auto updateCooldownTimer = std::chrono::duration<double, std::milli>(0.0);
	auto renderCooldownTimer = std::chrono::duration<double, std::milli>(0.0);

	auto next = std::chrono::steady_clock::now();

	while (!gameEndFlag)
	{

		if (updateCooldownTimer.count() <= renderCooldownTimer.count())
		{
			next += std::chrono::duration_cast<std::chrono::steady_clock::duration>(updateCooldownTimer);
			std::this_thread::sleep_until(next);
			updateWork();
			renderCooldownTimer -= updateCooldownTimer;
			updateCooldownTimer = updatePeriod;
		}
		else
		{
			next += std::chrono::duration_cast<std::chrono::steady_clock::duration>(renderCooldownTimer);
			std::this_thread::sleep_until(next);
			paintWork();
			updateCooldownTimer -= renderCooldownTimer;
			renderCooldownTimer = renderPeriod;
		}

		if (gameEndFlag)
		{
			PostMessage(suku::GameWindow::hWnd, WM_QUIT, NULL, NULL);
			break;
		}

		
	}
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
			suku::nowRoom->additionalFramePaint(_additionalFrameRate);

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
		devmode.dmDisplayFlags &= !DM_INTERLACED;
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

void startSender()
{
	std::thread thread(Sender);
	thread.detach();
}

RAWINPUTDEVICE rid;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	using namespace suku;
	std::pair<UINT, USHORT> inputResult;
	switch (message)
	{
	case WM_CREATE:
		rid.usUsagePage = 0x01;
		rid.usUsage = 0x06;
		rid.dwFlags = 0;
		rid.hwndTarget = hWnd;
		// 输入法设置为英文
		static HKL hkl = LoadKeyboardLayout(L"00000409", KLF_ACTIVATE);
		ActivateKeyboardLayout(hkl, KLF_SETFORPROCESS);
		// reduce input delay
		RegisterRawInputDevices(&rid, 1, sizeof(rid));

		PostMessage(hWnd, WM_CREATEFINISHED, 0, 0);
		break;
	case WM_CREATEFINISHED:
		timeBeginPeriod(1);
		//suku_save_init();
		graphics::WICFactoryGlobal::getWICFactory();
		graphics::D2DFactoryGlobal::getD2DFactory();
		graphics::suku_drawing_postinit(hWnd);
		soundInit();
		init();
		startSender();
		break;
	case WM_INPUT:
		inputResult = suku::input::onWindowInput(lParam);
		if (inputResult.first == INPUT_KEYDOWN && inputResult.second == VK_F1)
		{
			static bool fullscreenTag = false;
			fullscreenTag = !fullscreenTag;
			suku::GameWindow::setFullscreen(fullscreenTag);
		}
		break;
	case WM_ERASEBKGND:
		return true;
		break;
	case WM_DESTROY:
		timeEndPeriod(1);
		graphics::suku_drawing_uninit();
		soundUninit();
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		suku::GameWindow::RefreshSizeInfo();
		break;
	case WM_MOUSEMOVE:
		suku::input::Mouse::refreshPosition(lParam);
		break;
	case WM_LBUTTONDOWN:
		suku::input::Mouse::pushMouseMessage(MOUSE_INPUT_DOWN, MOUSE_VK_LEFT);
		SetCapture(hWnd);
		break;
	case WM_LBUTTONUP:
		suku::input::Mouse::pushMouseMessage(MOUSE_INPUT_UP, MOUSE_VK_LEFT);
		ReleaseCapture();
		break;
	case WM_LBUTTONDBLCLK:
		suku::input::Mouse::pushMouseMessage(MOUSE_INPUT_DOUBLECLICK, MOUSE_VK_LEFT);
		break;
	case WM_RBUTTONDOWN:
		suku::input::Mouse::pushMouseMessage(MOUSE_INPUT_DOWN, MOUSE_VK_RIGHT);
		break;
	case WM_RBUTTONUP:
		suku::input::Mouse::pushMouseMessage(MOUSE_INPUT_UP, MOUSE_VK_RIGHT);
		break;
	case WM_MBUTTONDOWN:
		suku::input::Mouse::pushMouseMessage(MOUSE_INPUT_DOWN, MOUSE_VK_MIDDLE);
		break;
	case WM_MBUTTONUP:
		suku::input::Mouse::pushMouseMessage(MOUSE_INPUT_UP, MOUSE_VK_MIDDLE);
		break;
	case WM_MOUSEWHEEL:
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			suku::input::Mouse::pushMouseMessage(MOUSE_INPUT_SCROLLUP, 0);
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
			suku::input::Mouse::pushMouseMessage(MOUSE_INPUT_SCROLLDOWN, 0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SUKU));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_IWANNAENGINE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SUKUSMALL));

	return RegisterClassExW(&wcex);
}

// #define FULLSCREEN_MODE

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

#ifdef FULLSCREEN_MODE
	suku::GameWindow::hWnd = CreateWindowW(szWindowClass, szTitle, WS_POPUP,
		0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), nullptr, nullptr, hInstance, nullptr);
#else
	UINT dpi = GetDpiForSystem();
	float scale = (float)dpi / 96.0f;
	if (scale < 1.0f)
		scale = 1.0f;

	RECT rc = { 0, 0,
		static_cast<LONG>(suku::WindowWidth * scale),
		static_cast<LONG>(suku::WindowHeight * scale) };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	int winW = rc.right - rc.left;
	int winH = rc.bottom - rc.top;

	int screenW = GetSystemMetrics(SM_CXSCREEN);
	int screenH = GetSystemMetrics(SM_CYSCREEN);

	int x = (screenW - winW) / 2;
	int y = (screenH - winH) / 2;

	suku::GameWindow::hWnd = CreateWindowW(
		szWindowClass,
		szTitle,
		WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_SIZEBOX | WS_MAXIMIZEBOX,
		x, y,
		winW, winH,
		nullptr, nullptr, hInstance, nullptr
	);
#endif

	if (!suku::GameWindow::hWnd)
	{
		return FALSE;
	}

	ShowWindow(suku::GameWindow::hWnd, nCmdShow);
	UpdateWindow(suku::GameWindow::hWnd);

	return TRUE;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_IWANNAENGINE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_IWANNAENGINE));

	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}
