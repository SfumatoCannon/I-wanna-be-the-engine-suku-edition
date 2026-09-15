#pragma once

// 包含 WinSDKVer.h 以便能在包含 SDKDDKVer.h 之前设置最低支持的 Windows 版本。
// 如果希望为之前的 Windows 平台构建应用程序，在包含 SDKDDKVer.h 之前请先包含 WinSDKVer.h 并
// 将 _WIN32_WINNT 宏设置为想要支持的平台。
#include <WinSDKVer.h>
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#include <SDKDDKVer.h>
