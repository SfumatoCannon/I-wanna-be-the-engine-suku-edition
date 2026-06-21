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
		ConfigElement(String _name, T _defaultValue, T _minValue, T _maxValue);
		ConfigElement(String _category, String _name, T _defaultValue, T _minValue, T _maxValue);
		ConfigElement(String _name, T _defaultValue, std::vector<T> _valueList);
		ConfigElement(String _category, String _name, T _defaultValue, std::vector<T> _valueList);
		void setValue(T _value);
		T value();
		T getValue() { return value(); }
		bool hasRangeConstraint() { return hasRangeConstraint_; }
		bool hasListConstraint() { return hasListConstraint_; }
		String getName() { return String(name_); }
		std::pair<T, T> getRange() { return { minValue_, maxValue_ }; }
		std::vector<T> getValueList() { return valueList_; }
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
		const std::vector<T> valueList_;
	};
}

#include "config_element.inl"