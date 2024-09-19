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

int fps = MAXFPS;

void Initialization()
{
	using namespace suku;
	GetModuleFileName(NULL, strPath, MAX_PATH);
	(_tcsrchr(strPath, _T('\\')))[1] = 0;
	Path_len = _tcsclen(strPath);
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
	
	if (MAXFPS == 50)
	{
		while (1)
		{
			Sleep(20);
			std::thread thread(updateAndPaintWork);
			thread.detach();
		}
	}
}

bool isThreadWorking = false;

void updateWork()
{
	using namespace suku;
	if (!isThreadWorking)
	{
		isThreadWorking = true;

		keyCheck();

		if (isKeyDown[VK_ESCAPE])
			PostMessage(hWnd, WM_QUIT, 0, 0);

		if (nowRoom)
			nowRoom->update();

		resetKey();

		isThreadWorking = false;
	}
}

void paintWork()
{
	using namespace suku;
	if (!isThreadWorking)
	{
		isThreadWorking = true;
		beginDraw(hWnd);
		nowRoom->paint();
		static SquareShape a(100.0, 96, 96);
		static SquareShape b(100.0, 96, 96);
		a.setTransform(rotation(96 + 50, 96 + 50, 30) * translation(96, 0));
		static ShapeSpriteZ c(a | b);
		c.setFillColor(150, 30, 70, 1.0);
		c.setOutlineColor(0, 0, 0, 1.0);      
		c.outlineWidth = 4;

		endDraw();
		isThreadWorking = false;
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
