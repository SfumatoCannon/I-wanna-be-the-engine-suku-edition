#pragma once

#ifdef _DEBUG
#define INFOWINDOW(message) {static bool flag = true; if (flag) flag = false, MessageBoxExW(NULL, \
	String(String("Information sent\n") + \
	"In function: " + typeid(this).name() + "." + __func__ + "\n" + message).content, \
	L"Info", MB_OK | MB_ICONINFORMATION, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));}

#define INFOWINDOW_GLOBAL(message) {static bool flag = true; if (flag) flag = false, MessageBoxExW(NULL, \
	String(String("Information sent\n") + \
	"In function: " + __func__ + "\n" + message).content, \
	L"Info", MB_OK | MB_ICONINFORMATION, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));}

#define WARNINGWINDOW(message) {static bool flag = true; if (flag) flag = false, MessageBoxExW(NULL, \
	String(String("WARNING\n") + \
	"In function: " + typeid(this).name() + "." + __func__ + "\n" + message).content, \
	L"Warning", MB_OK | MB_ICONWARNING, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));}

#define WARNINGWINDOW_GLOBAL(message) {static bool flag = true; if (flag) flag = false, MessageBoxExW(NULL, \
	String(String("WARNING\n") + \
	"In function: " + __func__ + "\n" + message).content, \
	L"Warning", MB_OK | MB_ICONWARNING, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));}

#define ERRORWINDOW(message) {static bool flag = true; if (flag) flag = false, MessageBoxExW(NULL, \
	String(String("An ERROR occurred!\n") + \
	"In function: " + typeid(this).name() + "." + __func__ + "\n" + message).content, \
	L"Error", MB_OK | MB_ICONERROR, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));}

#define ERRORWINDOW_GLOBAL(message) {static bool flag = true; if (flag) flag = false, MessageBoxExW(NULL, \
	String(String("An ERROR occurred!\n") + \
	"In function: " + __func__ + "\n" + message).content, \
	L"Error", MB_OK | MB_ICONERROR, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));}
#else
#define INFOWINDOW(message)
#define INFOWINDOW_GLOBAL(message)
#define WARNINGWINDOW(message)
#define WARNINGWINDOW_GLOBAL(message)
#define ERRORWINDOW(message)
#define ERRORWINDOW_GLOBAL(message)

#endif