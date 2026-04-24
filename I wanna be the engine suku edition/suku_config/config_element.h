#pragma once
#include <string>
#include "../suku_foundation/var.h"

namespace suku
{
	template<typename T>
	class ConfigElement
	{
	public:
		ConfigElement(std::string _name, T _defaultValue);
		ConfigElement(std::wstring _name, T _defaultValue);
		void setValue(T _value);
		T value();
		void operator =(const ConfigElement& _other) = delete;
		void operator =(ConfigElement&& _other) = delete;
		void operator =(T _value) { setValue(_value); }
	private:
		std::wstring name_;
		Var value_;
	};
}

#include "config_element.inl"