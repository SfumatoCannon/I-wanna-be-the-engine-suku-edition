#include "framework.h"
#include "global_value.h"
#include "I wanna be the engine suku edition.h"

#define MAX_LOADSTRING 100
#define WM_CREATEFINISHED (WM_USER + 1)

HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

HWND suku::GameWindow::hWnd;
suku::String suku::GameWindow::title = L"I wanna be the engine suku edition";
int suku::GameWindow::width = WindowWidth;
int suku::GameWindow::height = WindowHeight;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

bool gameEndFlag = false;
void endGame() { gameEndFlag = true; }

double fps = 50.0;

void Sender()
{
	//double updateInterval = 20;	//  1000 / 50
	//double paintInterval = 1000.0 / 50.0;
	//double updateTick = 0;
	//double paintTick = 0;
	auto updateAndPaintWork = []() {updateWork(), paintWork(); };

	std::future<void> task = std::async(updateAndPaintWork);

	while (1)
	{
		Sleep((DWORD)(1000.0 / fps));
		task.get();
		if (gameEndFlag == true)
		{
			PostMessage(suku::GameWindow::hWnd, WM_QUIT, NULL, NULL);
			break;
		}
		else
			task = std::async(updateAndPaintWork);
	}
}

std::mutex threadLock;

void updateWork()
{
	using namespace suku;
	if (threadLock.try_lock())
	{
		suku::input::keyCheck();

		if (suku::input::isKeyDown(VK_ESCAPE) && !gameEndFlag)
			endGame();
		else
		{
			if (nowRoom)
				nowRoom->update();
		}

		suku::input::resetKey();

		threadLock.unlock();
	}
}

void paintWork()
{
	using namespace suku;
	if (threadLock.try_lock())
	{
		if (!gameEndFlag)
		{
			beginDraw(suku::GameWindow::hWnd);
			nowRoom->paint();
			endDraw();
		}
		threadLock.unlock();
	}
}

BOOL monitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	MONITORINFOEX mi;
	mi.cbSize = sizeof(MONITORINFOEX);
	GetMonitorInfo(hMonitor, &mi);
	DEVMODE devmode;
	devmode.dmSize = sizeof(DEVMODE);
	EnumDisplaySettings(mi.szDevice, ENUM_CURRENT_SETTINGS, &devmode);
	if (devmode.dmDisplayFrequency != fps)
	{
		devmode.dmDisplayFlags &= !DM_INTERLACED;
		devmode.dmDisplayFrequency = (DWORD)fps;
		LONG res = ChangeDisplaySettingsEx(mi.szDevice, &devmode, nullptr, 0, nullptr);
	}
	return true;
}

void setFPS(int _fps)
{
	fps = _fps;
	EnumDisplayMonitors(nullptr, nullptr, monitorEnumProc, NULL);
}

void startSender()
{
	std::thread thread(Sender);
	thread.detach();
}

RAWINPUTDEVICE rid;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	using namespace suku;
	switch (message)
	{
	case WM_CREATE:
		rid.usUsagePage = 0x01;
		rid.usUsage = 0x06;
		rid.dwFlags = 0;
		rid.hwndTarget = hWnd;
		RegisterRawInputDevices(&rid, 1, sizeof(rid));
		PostMessage(hWnd, WM_CREATEFINISHED, 0, 0);
		break;
	case WM_CREATEFINISHED:
		timeBeginPeriod(1);
		//suku_save_init();
		WICFactoryGlobal::getWICFactory();
		//suku_drawing_preinit_WIC();
		suku_drawing_postinit(hWnd);
		soundInit();
		init();
		startSender();
		break;
	case WM_INPUT:
		suku::input::onWindowInput(lParam);
		break;
	case WM_ERASEBKGND:
		return true;
		break;
	case WM_DESTROY:
		timeEndPeriod(1);
		suku_drawing_uninit();
		soundUninit();
		PostQuitMessage(0);
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
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
	RECT rc;
	SetRect(&rc, suku::WindowX, suku::WindowY, suku::WindowX + suku::WindowWidth, suku::WindowY + suku::WindowHeight);
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

#ifdef FULLSCREEN_MODE
	suku::GameWindow::hWnd = CreateWindowW(szWindowClass, szTitle, WS_POPUP,
		0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), nullptr, nullptr, hInstance, nullptr);
#else
	suku::GameWindow::hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
		rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);
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
