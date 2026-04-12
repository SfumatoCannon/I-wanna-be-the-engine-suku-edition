#include "pch.h"
#include "win32_window_creator.h"
#include "win32_message_handler.h"
#include "game_window.h"
#include "../Resource.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

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
