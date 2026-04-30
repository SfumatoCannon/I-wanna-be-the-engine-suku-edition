#pragma once
#include <string>
#include "../suku_foundation/suku_string.h"

namespace suku
{
	class ConfigFile
	{
	public:
		template<typename T> static T loadVar(std::wstring _name, T _defaultValue = T());
		template<typename T> static T loadVar(std::wstring _name, std::wstring _category, T _defaultValue = T());
		template<typename T> static void saveVar(std::wstring _name, T _value);
		template<typename T> static void saveVar(std::wstring _name, std::wstring _category, T _value);
		template<typename T> static T loadVar(String _name, T _defaultValue = T());
		template<typename T> static T loadVar(String _name, String _category, T _defaultValue = T());
		template<typename T> static void saveVar(String _name, T _value);
		template<typename T> static void saveVar(String _name, String _category, T _value);
	private:
		inline static std::wstring fileName = L"config.ini";
	};
}

#include "config_file.inl"