#include "framework.h"
#include "I wanna be the engine suku edition.h"

#define MAX_LOADSTRING 100
#define WM_CREATEFINISHED (WM_USER + 1)

HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

HWND suku::GameWindow::hWnd;
suku::String suku::GameWindow::title = L"I wanna be the engine suku edition";
int suku::GameWindow::width = WINDOWWIDTH;
int suku::GameWindow::height = WINDOWHEIGHT;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

bool gameEndFlag = false;
void endGame() {gameEndFlag = true;}

int fps = MAXFPS;

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
				PostMessage(suku::GameWindow::hWnd, WM_QUIT, NULL, NULL);
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

void startSender()
{
	std::thread thread(Sender);
	thread.detach();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	using namespace suku;
	//if (message > WM_SUKUAUDIO_START && message < WM_SUKUAUDIO_END)
	//{
	//	Sound::onWindowMessage(message, wParam, lParam);
	//	return true;
	//}
	switch (message)
	{
	case WM_CREATE:
		PostMessage(hWnd, WM_CREATEFINISHED, 0, 0);
		break;
	case WM_CREATEFINISHED:		
		timeBeginPeriod(1);
		suku_save_init();
		suku_drawing_init(hWnd);
		soundInit();
		init();
		startSender();
		break;
	case WM_KEYDOWN:case WM_KEYUP:
		pushKeyMessage(message, wParam);
		break;
	case MM_MCINOTIFY:
		//suku::Sound::onWindowMessageCallback(wParam, lParam);
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
	suku::GameWindow::hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
		rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance, nullptr);

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
