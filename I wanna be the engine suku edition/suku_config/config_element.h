#pragma once
#include <string>
#include <vector>
#include "../suku_foundation/var.h"

namespace suku
{
	class suku::String;

	template<typename T>
	class ConfigElement
	{
	public:
		ConfigElement(String _name, T _defaultValue);
		ConfigElement(String _category, String _name, T _defaultValue);
		void setValue(T _value);
		T value();
		T getValue() { return value(); }
		void operator =(const ConfigElement& _other) = delete;
		void operator =(ConfigElement&& _other) = delete;
		void operator =(T _value) { setValue(_value); }
	private:
		const std::wstring name_;
		const std::wstring category_;
		Var value_;
		const bool hasRangeConstraint_ = false;
		const T minValue_ = T();
		const T maxValue_ = T();
		const bool hasListConstraint_ = false;
		std::vector<T> valueList_;
	};
}

#include "config_element.inl"