#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN
// Windows
#include <windows.h>
#include <wincodec.h>
#include <mmsystem.h>
#include <d2d1.h>
// C
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>
// C++
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
#include <fstream>
#include <future>
#include <any>
#include <mutex>

#pragma comment (lib, "d2d1")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
// for sound (XAudio2 & Media Foundation)
#pragma comment (lib, "xaudio2.lib")
#pragma comment (lib, "mfplat.lib")
#pragma comment (lib, "mfreadwrite.lib")
#pragma comment (lib, "mf.lib")
#pragma comment (lib, "mfuuid.lib")
#pragma comment (lib, "ole32.lib")
#pragma comment (lib, "propsys.lib")