#include "framework.h"
#include "I wanna be the engine suku edition.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

HWND hWnd;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

bool gameEndFlag = false;
void endGame() {gameEndFlag = true;}

int fps = MAXFPS;

void Initialization()
{
	using namespace suku;
	GetModuleFileName(NULL, exePath, MAX_PATH);
	(_tcsrchr(exePath, L'\\'))[1] = 0;
	Path_len = _tcsclen(exePath);
	HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr))
	{
		hr = CoCreateInstance(
			CLSID_WICImagingFactory1,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&g_pIWICFactory)
		);
	}
	if (SUCCEEDED(hr))
		spritePreSet();
}

void Sender()
{
	double updateInterval = 20;	//  1000 / 50
	double paintInterval = 1000.0 / MAXFPS;
	double updateTick = 0;
	double paintTick = 0;
	auto updateAndPaintWork = []() {updateWork(), paintWork(); };
	
	std::future<void> task = std::async(updateAndPaintWork);

	if (MAXFPS == 50)
	{
		while (1)
		{
			Sleep(20);
			task.get();
			if (gameEndFlag == true)
			{
				PostMessage(hWnd, WM_QUIT, NULL, NULL);
				break;
			}
			else
				task = std::async(updateAndPaintWork);
		}
	}
}

std::mutex threadLock;

void updateWork()
{
	using namespace suku;
	if (threadLock.try_lock())
	{
		keyCheck();

		if (isKeyDown[VK_ESCAPE] && !gameEndFlag)
			endGame();
		else
		{
			if (nowRoom)
				nowRoom->update();
		}

		resetKey();

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
			beginDraw(hWnd);
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
		devmode.dmDisplayFrequency = fps;
		LONG res = ChangeDisplaySettingsEx(mi.szDevice, &devmode, nullptr, 0, nullptr);
	}
	return true;
}
void setFPS(int _fps)
{
	fps = _fps;
	EnumDisplayMonitors(nullptr, nullptr, monitorEnumProc, NULL);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	using namespace suku;
	switch (message)
	{
	case WM_CREATE:
	{
		timeBeginPeriod(1);
		createD2DResource(hWnd);
		Initialization();
		init();
		std::thread thread(Sender);
		thread.detach();

		break;
	}
	case WM_KEYDOWN:case WM_KEYUP:
		pushKeyMessage(message, wParam);
		break;
	case WM_ERASEBKGND:
		return TRUE;
		break;
	case WM_DESTROY:
		timeEndPeriod(1);
		cleanup();
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
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IWANNAENGINE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_IWANNAENGINE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	RECT rc;
	SetRect(&rc, WINDOWX, WINDOWY, WINDOWX + WINDOWWIDTH, WINDOWY + WINDOWHEIGHT);
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
		rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

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
