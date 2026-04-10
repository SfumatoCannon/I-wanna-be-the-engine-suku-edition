#pragma once

#include "string.h"
#include <sstream>
#include <stacktrace>
#include <Windows.h>

#ifdef _DEBUG

#define IN_SHORT_PATH(str) \
    (strrchr(str, '\\') ? strrchr(str, '\\') + 1 : str)

std::string getStackTrace();

#define INFOWINDOW(message) { \
	static bool flag = true; if (flag) { flag = false; \
	std::ostringstream oss; \
oss << "Information sent" << std::endl \
<< "In function: " << typeid(this).name() << "." << __func__ << " (" << IN_SHORT_PATH(__FILE__) << ": " << __LINE__ << ")" << std::endl \
<< message \
<< std::endl << std::endl << getStackTrace(); \
MessageBoxExW(NULL, \
	String(oss.str()).content, \
	L"Info", MB_OK | MB_ICONINFORMATION, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)); \
} \
}

#define INFOWINDOW_GLOBAL(message) { \
	static bool flag = true; if (flag) { flag = false; \
	std::ostringstream oss; \
	oss << "Information sent" << std::endl \ 
<< "In function: " << __func__ << " (" << IN_SHORT_PATH(__FILE__) << ": " << __LINE__ << ")" << std::endl \
<< message \
<< std::endl << std::endl << getStackTrace(); \
MessageBoxExW(NULL, \
	String(oss.str()).content, \
	L"Info", MB_OK | MB_ICONINFORMATION, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)); \
} \
}

#define WARNINGWINDOW(message) { \
	static bool flag = true; if (flag) { flag = false; \
	std::ostringstream oss; \
	oss << "WARNING" << std::endl \
<< "In function: " << typeid(this).name() << "." << __func__ << " (" << IN_SHORT_PATH(__FILE__) << ": " << __LINE__ << ")" << std::endl \
<< message \
<< std::endl << std::endl << getStackTrace(); \
MessageBoxExW(NULL, \
	String(oss.str()).content, \
	L"Warning", MB_OK | MB_ICONWARNING, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)); \
} \
}

#define WARNINGWINDOW_GLOBAL(message) { \
	static bool flag = true; if (flag) { flag = false; \
	std::ostringstream oss; \
	oss << "WARNING" << std::endl \
<< "In function: " << __func__ << " (" << IN_SHORT_PATH(__FILE__) << ": " << __LINE__ << ")" << std::endl \
<< message \
<< std::endl << std::endl << getStackTrace(); \
MessageBoxExW(NULL, \
	String(oss.str()).content, \
	L"Warning", MB_OK | MB_ICONWARNING, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)); \
} \
}

#define ERRORWINDOW(message) { \
	static bool flag = true; if (flag) { flag = false; \
	std::ostringstream oss; \
	oss << "An ERROR occurred!" << std::endl \
<< "In function: " << typeid(this).name() << "." << __func__ << " (" << IN_SHORT_PATH(__FILE__) << ": " << __LINE__ << ")" << std::endl \
<< message \
<< std::endl << std::endl << getStackTrace(); \
MessageBoxExW(NULL, \
	String(oss.str()).content, \
	L"Warning", MB_OK | MB_ICONERROR, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)); \
} \
}

#define ERRORWINDOW_GLOBAL(message){ \
	static bool flag = true; if (flag) { flag = false; \
	std::ostringstream oss; \
	oss << "An ERROR occurred!" << std::endl \
<< "In function: " << __func__ << " (" << IN_SHORT_PATH(__FILE__) << ": " << __LINE__ << ")" << std::endl \
<< message \
<< std::endl << std::endl << getStackTrace(); \
MessageBoxExW(NULL, \
	String(oss.str()).content, \
	L"Warning", MB_OK | MB_ICONERROR, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)); \
} \
}

#else
#define INFOWINDOW(message)
#define INFOWINDOW_GLOBAL(message)
#define WARNINGWINDOW(message)
#define WARNINGWINDOW_GLOBAL(message)
#define ERRORWINDOW(message)
#define ERRORWINDOW_GLOBAL(message)

#endif