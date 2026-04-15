#include "message.h"

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
			std::ostringstream oss;
			oss << "Information sent" << std::endl
				<< "In function: " << _callerInfo.contentInString() << std::endl
				<< String(_message).contentInString()
				<< std::endl << std::endl << getStackTrace();
			MessageBoxExW(NULL,
				String(oss.str()).content,
				L"Info", MB_OK | MB_ICONINFORMATION, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));
#endif
		}

		void showWarningMessage(const String& _callerInfo, const String& _message)
		{
#ifdef _DEBUG
			std::ostringstream oss;
			oss << "WARNING" << std::endl
				<< "In function: " << _callerInfo.contentInString() << std::endl
				<< String(_message).contentInString()
				<< std::endl << std::endl << getStackTrace();
			MessageBoxExW(NULL,
				String(oss.str()).content,
				L"Warning", MB_OK | MB_ICONWARNING, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));
#endif
		}

		void showErrorMessage(const String& _callerInfo, const String& _message)
		{
#ifdef _DEBUG
			std::ostringstream oss;
			oss << "An ERROR occurred!" << std::endl
				<< "In function: " << _callerInfo.contentInString() << std::endl
				<< String(_message).contentInString()
				<< std::endl << std::endl << getStackTrace();
			MessageBoxExW(NULL,
				String(oss.str()).content,
				L"Error", MB_OK | MB_ICONERROR, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT));
#endif
		}
	}
}