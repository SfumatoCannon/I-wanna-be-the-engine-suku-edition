#pragma once
#include <string>
#include "../suku_foundation/var.h"

namespace suku
{
	class ConfigElement
	{
	public:
		template<typename T> ConfigElement(std::wstring _name, T _defaultValue);
		template<typename T> void setValue(T _value);
		template<typename T> T value();
		template<typename T> void operator =(T _value) { setValue<T>(_value); }
	private:
		std::wstring name_;
		Var value_;
	};

	class ConfigElementPool
	{

	};
}

#include "config_element_pool.inl"