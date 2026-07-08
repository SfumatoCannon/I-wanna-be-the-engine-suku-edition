#pragma once
#include <string>
#include "../suku_foundation/suku_string.h"
#include "suku_config_concepts.h"

namespace suku
{
	class ConfigFile
	{
	public:
		template<suku_config_var_type T> static T loadVar(String _name, T _defaultValue = T());
		template<suku_config_var_type T> static T loadVar(String _name, String _category, T _defaultValue = T());
		template<suku_config_var_type T> static void saveVar(String _name, T _value);
		template<suku_config_var_type T> static void saveVar(String _name, String _category, T _value);
	private:
		inline static std::wstring fileName_ = L"config.ini";
	};
}

#include "config_file.inl"