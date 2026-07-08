#pragma once
#include <concepts>
#include <string>

namespace suku
{
	class String;

	template<typename T>
	concept suku_config_var_type = std::is_arithmetic_v<T>
		|| std::is_same_v<T, std::string>
		|| std::is_same_v<T, std::wstring>
		|| std::is_same_v<T, String>;
}