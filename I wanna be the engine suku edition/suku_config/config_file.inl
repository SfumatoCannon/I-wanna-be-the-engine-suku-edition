#include "config_file.h"
#include "../suku_foundation/file.h"
#include <type_traits>
#include <Windows.h>

namespace suku
{
	template<typename T>
	inline T ConfigFile::loadVar(std::wstring _name, T _defaultValue)
	{
		return loadVar(_name, L"Config", _defaultValue);
	}

	template<typename T>
	inline T ConfigFile::loadVar(std::wstring _name, std::wstring _category, T _defaultValue)
	{
		std::wstring defaultValueStr;
		if constexpr (std::is_same_v<T, bool>)
		{
			defaultValueStr = _defaultValue ? L"true" : L"false";
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			defaultValueStr = std::wstring(_defaultValue.begin(), _defaultValue.end());
		}
		else if constexpr (std::is_same_v<T, std::wstring>)
		{
			defaultValueStr = std::wstring(_defaultValue);
		}
		else if constexpr (std::is_same_v<T, String>)
		{
			defaultValueStr = std::wstring(_defaultValue.content);
		}
		else if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T>)
		{
			defaultValueStr = std::to_wstring(_defaultValue);
		}
		else
		{
			static_assert(std::_Always_false<T>::value, "Unsupported var type for loadVar");
		}

		wchar_t buffer[256];
		GetPrivateProfileStringW(_category.c_str(), _name.c_str(), defaultValueStr.c_str(), buffer, 256,
			filesystem::absolutePath(fileName.c_str()).content);

		if constexpr (std::is_same_v<T, bool>)
		{
			return std::wstring(buffer) == L"true";
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			return String(buffer).contentInString();
		}
		else if constexpr (std::is_same_v<T, std::wstring>)
		{
			return std::wstring(buffer);
		}
		else if constexpr (std::is_same_v<T, String>)
		{
			return String(buffer);
		}
		else if constexpr (std::is_integral_v<T>)
		{
			return static_cast<T>(std::wcstoll(buffer, nullptr, 10));
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			return static_cast<T>(std::wcstold(buffer, nullptr));
		}
	}

	template<typename T>
	inline void ConfigFile::saveVar(std::wstring _name, T _value)
	{
		saveVar(_name, L"Config", _value);
	}

	template<typename T>
	inline void ConfigFile::saveVar(std::wstring _name, std::wstring _category, T _value)
	{
		std::wstring valueStr;
		if constexpr (std::is_same_v<T, bool>)
		{
			valueStr = _value ? L"true" : L"false";
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			valueStr = std::wstring(_value.begin(), _value.end());
		}
		else if constexpr (std::is_same_v<T, std::wstring>)
		{
			valueStr = std::wstring(_value);
		}
		else if constexpr (std::is_same_v<T, String>)
		{
			valueStr = std::wstring(_value.content);
		}
		else if constexpr (std::is_integral_v<T> || std::is_floating_point_v<T>)
		{
			valueStr = std::to_wstring(_value);
		}
		else
		{
			static_assert(std::_Always_false<T>::value, "Unsupported var type for saveVar");
		}

		WritePrivateProfileStringW(_category.c_str(), _name.c_str(), valueStr.c_str(),
			filesystem::absolutePath(fileName.c_str()).content);
	}

	template<typename T>
	inline T ConfigFile::loadVar(String _name, T _defaultValue)
	{
		return loadVar(_name.contentInWString(), _defaultValue);
	}

	template<typename T>
	inline T ConfigFile::loadVar(String _name, String _category, T _defaultValue)
	{
		return loadVar(_name.contentInWString(), _category.contentInWString(), _defaultValue);
	}

	template<typename T>
	inline void ConfigFile::saveVar(String _name, T _value)
	{
		saveVar(_name.contentInWString(), _value);
	}

	template<typename T>
	inline void ConfigFile::saveVar(String _name, String _category, T _value)
	{
		saveVar(_name.contentInWString(), _category.contentInWString(), _value);
	}
}