#pragma once

#include "suku_string.h"
#include <sstream>
#include <stacktrace>
#include <string>
#include <Windows.h>

#define IN_SHORT_PATH(str) (strrchr(str, '\\') ? strrchr(str, '\\') + 1 : str)
#define CALLER_INFO_CLASS (suku::String(typeid(this).name()) + "." + __func__ + " (" + IN_SHORT_PATH(__FILE__) + ": " + std::to_string(__LINE__) + ")")
#define CALLER_INFO_GLOBAL (__func__ + std::string(" (") + IN_SHORT_PATH(__FILE__) + ": " + std::to_string(__LINE__) + ")")

#define INFOWINDOW(msg)				suku::message::showInfoMessage(CALLER_INFO_CLASS, msg)
#define INFOWINDOW_GLOBAL(msg)		suku::message::showInfoMessage(CALLER_INFO_GLOBAL, msg)
#define WARNINGWINDOW(msg)			suku::message::showWarningMessage(CALLER_INFO_CLASS, msg)
#define WARNINGWINDOW_GLOBAL(msg)	suku::message::showWarningMessage(CALLER_INFO_GLOBAL, msg)
#define ERRORWINDOW(msg)			suku::message::showErrorMessage(CALLER_INFO_CLASS, msg)
#define ERRORWINDOW_GLOBAL(msg)		suku::message::showErrorMessage(CALLER_INFO_GLOBAL, msg)

namespace suku
{
	namespace message
	{
		std::string getStackTrace();
		void showInfoMessage(const String& _callerInfo, const String& _message);
		void showWarningMessage(const String& _callerInfo, const String& _message);
		void showErrorMessage(const String& _callerInfo, const String& _message);
	}
}