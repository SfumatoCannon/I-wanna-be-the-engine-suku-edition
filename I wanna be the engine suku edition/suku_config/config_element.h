#pragma once
#include <string>
#include <vector>
#include "../suku_foundation/var.h"
#include "suku_config_concepts.h"

namespace suku
{
	class String;

	class IConfigElement
	{
	public:
		virtual ~IConfigElement() = default;
		virtual const String& getName()const = 0;
		virtual const String& getCategory()const = 0;
		virtual String getValueString()const = 0;
		virtual void selectPrev() = 0;
		virtual void selectNext() = 0;
		virtual bool hasPrev()const = 0;
		virtual bool hasNext()const = 0;
	protected:
		IConfigElement() = default;
		IConfigElement(const IConfigElement& _other) = default;
	};

	template<suku_config_var_type T>
	class ConfigElement : public IConfigElement
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
		T value()const;
		T getValue()const { return value(); }
		bool hasRangeConstraint()const { return hasRangeConstraint_; }
		bool hasListConstraint()const { return hasListConstraint_; }

		virtual const String& getName()const override { return name_; }
		virtual const String& getCategory()const override { return category_; }
		virtual String getValueString()const override;
		virtual void selectPrev() override;
		virtual void selectNext() override;
		virtual bool hasPrev()const override;
		virtual bool hasNext()const override;

		std::pair<T, T> getRange()const { return { minValue_, maxValue_ }; }
		const std::vector<T>& getValueList()const { return valueList_; }
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