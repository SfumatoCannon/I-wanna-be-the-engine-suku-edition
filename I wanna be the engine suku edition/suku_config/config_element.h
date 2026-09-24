#pragma once
#include <string>
#include <vector>
#include "../suku_foundation/var.h"
#include "suku_config_concepts.h"

namespace suku
{
	class String;

	template<suku_config_var_type T>
	class ConfigElement
	{
	public:
		ConfigElement(const ConfigElement<T>& _other);
		ConfigElement(const String& _name, T _defaultValue);
		ConfigElement(const String& _category, const String& _name, T _defaultValue);
		ConfigElement(const String& _name, T _defaultValue, T _minValue, T _maxValue);
		ConfigElement(const String& _category, const String& _name, T _defaultValue, T _minValue, T _maxValue);
		ConfigElement(const String& _name, T _defaultValue, std::vector<T> _valueList);
		ConfigElement(const String& _category, const String& _name, T _defaultValue, std::vector<T> _valueList);
		void setValue(T _value);
		T value();
		T getValue() { return value(); }
		bool hasRangeConstraint() { return hasRangeConstraint_; }
		bool hasListConstraint() { return hasListConstraint_; }
		const String& getName() { return name_; }
		const String& getCategory() { return category_; }
		const std::pair<T, T>& getRange() { return { minValue_, maxValue_ }; }
		const std::vector<T>& getValueList() { return valueList_; }
		void operator =(const ConfigElement& _other) = delete;
		void operator =(ConfigElement&& _other) = delete;
		void operator =(T _value) { setValue(_value); }
	private:
		const String name_;
		const String category_;
		T value_;
		const bool hasRangeConstraint_ = false;
		const T minValue_ = T();
		const T maxValue_ = T();
		const bool hasListConstraint_ = false;
		const std::vector<T> valueList_;
		std::vector<bool> isValueEnable_;
	};
}

#include "config_element.inl"