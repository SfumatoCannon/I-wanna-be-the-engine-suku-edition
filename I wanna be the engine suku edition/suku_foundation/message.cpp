#include "pch.h"
#include "message.h"
#include <format>

namespace suku
{
	namespace message
	{
		std::string getStackTrace()
		{
			std::ostringstream oss;
			auto st = std::stacktrace::current();

			int index = 0;
			for (auto& e : st)
			{
				if (!e.source_file().empty())
				{
					if (index > 16)
					{
						oss << "..." << std::endl;
						break;
					}
					if (index > 0)
						oss << "at " << IN_SHORT_PATH(e.source_file().c_str()) << ":" << e.source_line() << std::endl;
					index++;
				}
			}

			return oss.str();
		}

		void showInfoMessage(const String& _callerInfo, const String& _message)
		{
#ifdef _DEBUG
			std::string formatted = std::format("Information sent\nIn function: {}\n{}\n\n{}",
				_callerInfo.toString(), String(_message).toString(), getStackTrace());
			MessageBoxExW(NULL,
				String(formatted).content,
				L"Info", MB_OK | MB_ICONINFORMATION, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));
#endif
		}

		void showWarningMessage(const String& _callerInfo, const String& _message)
		{
#ifdef _DEBUG
			std::string formatted = std::format("WARNING\nIn function: {}\n{}\n\n{}",
				_callerInfo.toString(), String(_message).toString(), getStackTrace());
			MessageBoxExW(NULL,
				String(formatted).content,
				L"Warning", MB_OK | MB_ICONWARNING, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));
#endif
		}

		void showErrorMessage(const String& _callerInfo, const String& _message)
		{
#ifdef _DEBUG
			std::string formatted = std::format("An ERROR occurred!\nIn function: {}\n{}\n\n{}",
				_callerInfo.toString(), String(_message).toString(), getStackTrace());
			MessageBoxExW(NULL,
				String(formatted).content,
				L"Error", MB_OK | MB_ICONERROR, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));
#endif
		}
	}
}