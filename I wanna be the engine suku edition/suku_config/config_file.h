#pragma once
#include <string>
#include "../suku_foundation/suku_string.h"

namespace suku
{
	template<typename T>
	concept config_var_type = std::is_arithmetic_v<T>
		|| std::is_same_v<T, std::string>
		|| std::is_same_v<T, std::wstring>
		|| std::is_same_v<T, String>;

	class ConfigFile
	{
	public:
		template<config_var_type T> static T loadVar(String _name, T _defaultValue = T());
		template<config_var_type T> static T loadVar(String _name, String _category, T _defaultValue = T());
		template<config_var_type T> static void saveVar(String _name, T _value);
		template<config_var_type T> static void saveVar(String _name, String _category, T _value);
	private:
		inline static std::wstring fileName_ = L"config.ini";
	};
}

#include "config_file.inl"