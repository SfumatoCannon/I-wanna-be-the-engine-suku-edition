#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
#include <wincodec.h>
#include <mmsystem.h>
#include <d2d1.h>
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>
// C++ 运行时头文件
#include <tuple>
#include <initializer_list>
#include <functional>
#include <list>
#include <queue>
#include <map>
#include <ctime>
#include <random>
#include <string>
#include <thread>
#include <mutex>
#include <fstream>
#include <future>
#include <any>


#pragma comment (lib,"d2d1")
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib,"winmm.lib")