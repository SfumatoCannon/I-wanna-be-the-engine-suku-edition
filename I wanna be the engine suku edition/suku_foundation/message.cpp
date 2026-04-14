#include "message.h"

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
