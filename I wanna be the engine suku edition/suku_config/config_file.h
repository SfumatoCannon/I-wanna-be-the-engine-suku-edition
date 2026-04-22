#pragma once
#include <string>
#include "../suku_foundation/suku_string.h"

namespace suku
{
	class ConfigFile
	{
	public:
		template<typename T> static T loadVar(std::wstring _name);
		template<typename T> static void saveVar(std::wstring _name, T _value);
		template<typename T> static T loadVar(String _name);
		template<typename T> static void saveVar(String _name, T _value);
	private:
		inline static std::wstring fileName = L"config.ini";
	};
}