#include "config_element.h"
#include "config_file.h"
#include "../suku_foundation/suku_string.h"

namespace suku
{
	template<suku_config_var_type T>
	inline ConfigElement<T>::ConfigElement(const ConfigElement<T>& _other)
		: name_(_other.getName()), category_(_other.getCategory()),
		hasRangeConstraint_(_other.hasRangeConstraint()), hasListConstraint_(_other.hasListConstraint()),
		minValue_(_other.getRange().first), maxValue_(_other.getRange().second),
		valueList_(_other.getValueList()), isValueEnable_(_other.isValueEnable_)
	{}
	
	template<suku_config_var_type T>
	inline ConfigElement<T>::ConfigElement(const String& _name, T _defaultValue)
		: name_(_name), category_(L"Config")
	{
		T valueT = ConfigFile::loadVar(_name, _defaultValue);
		value_ = valueT;
	}

	template<suku_config_var_type T>
	inline ConfigElement<T>::ConfigElement(const String& _category, const String& _name, T _defaultValue)
		: name_(_name), category_(_category)
	{
		T valueT = ConfigFile::loadVar(_name, _category, _defaultValue);
		value_ = valueT;
	}

	template<suku_config_var_type T>
	inline ConfigElement<T>::ConfigElement(const String& _name, T _defaultValue, T _minValue, T _maxValue)
		: name_(_name), category_(L"Config"), hasRangeConstraint_(true), minValue_(_minValue), maxValue_(_maxValue)
	{
		T valueT = ConfigFile::loadVar(_name, _defaultValue);
		if (valueT < minValue_)
		{
			valueT = minValue_;
		}
		else if (valueT > maxValue_)
		{
			valueT = maxValue_;
		}
		value_ = valueT;
	}

	template<suku_config_var_type T>
	inline ConfigElement<T>::ConfigElement(const String& _category, const String& _name, T _defaultValue, T _minValue, T _maxValue)
		: name_(_name), category_(_category), hasRangeConstraint_(true), minValue_(_minValue), maxValue_(_maxValue)
	{
		T valueT = ConfigFile::loadVar(_name, _category, _defaultValue);
		if (valueT < minValue_)
		{
			valueT = minValue_;
		}
		else if (valueT > maxValue_)
		{
			valueT = maxValue_;
		}
		value_ = valueT;
	}

	template<suku_config_var_type T>
	inline ConfigElement<T>::ConfigElement(const String& _name, T _defaultValue, std::vector<T> _valueList)
		: name_(_name), category_(L"Config"), hasListConstraint_(true), valueList_(_valueList), isValueEnable_(_valueList.size(), true)
	{
		T valueT = ConfigFile::loadVar(_name, _defaultValue);
		if (std::find(valueList_.begin(), valueList_.end(), valueT) != valueList_.end())
		{
			value_ = valueT;
		}
		else
		{
			value_ = _defaultValue;
		}
	}

	template<suku_config_var_type T>
	inline ConfigElement<T>::ConfigElement(const String& _category, const String& _name, T _defaultValue, std::vector<T> _valueList)
		: name_(_name), category_(_category), hasListConstraint_(true), valueList_(_valueList), isValueEnable_(_valueList.size(), true)
	{
		T valueT = ConfigFile::loadVar(_name, _category, _defaultValue);
		if (std::find(valueList_.begin(), valueList_.end(), valueT) != valueList_.end())
		{
			value_ = valueT;
		}
		else
		{
			value_ = _defaultValue;
		}
	}

	template<suku_config_var_type T>
	inline void ConfigElement<T>::setValue(T _value)
	{
		if (hasListConstraint_)
		{
			auto iter = std::find(valueList_.begin(), valueList_.end(), _value);
			if (iter == valueList_.end())
			{
				WARNINGWINDOW("Attempted to set config variable " + String(name_)
					+ " to a value that is not in the allowed list. Value not set.");
				return;
			}
			else if (isValueEnable_[iter - valueList_.begin()] == false)
			{
				WARNINGWINDOW("Attempted to set config variable " + String(name_)
					+ " to a value that is disabled. Value not set.");
				return;
			}
		}
		else if (hasRangeConstraint_)
		{
			if (_value < minValue_)
				_value = minValue_;
			else if (_value > maxValue_)
				_value = maxValue_;
		}
		value_ = _value;
		ConfigFile::saveVar(name_, category_, _value);
	}

	template<suku_config_var_type T>
	inline T ConfigElement<T>::value() const
	{
		return value_;
	}

	template<suku_config_var_type T>
	inline String ConfigElement<T>::getValueString() const
	{
		if constexpr (std::is_same_v<T, bool>)
		{
			if (value_ == true)
				return "true";
			else
				return "false";
		}
		else
		{
			return String(std::to_wstring(value_));
		}
	}

	template<suku_config_var_type T>
	inline void ConfigElement<T>::selectPrev()
	{
		if (hasRangeConstraint())
		{
			auto range = getRange();
			T newValue = getValue() - 1;
			if (newValue < range.first)
				newValue = range.first;
			setValue(newValue);
		}
		else if (hasListConstraint())
		{
			auto list = getValueList();
			auto it = std::find(list.begin(), list.end(), value());
			if (it != list.end() && it != list.begin())
				setValue(*(it - 1));
		}
		else
		{
			if constexpr (std::is_same_v<bool, T>)
			{
				setValue(!getValue());
			}
			else if constexpr (std::is_arithmetic_v<T>)
			{
				setValue(getValue() - 1);
			}
		}
	}

	template<suku_config_var_type T>
	inline void ConfigElement<T>::selectNext()
	{
		if (hasRangeConstraint())
		{
			auto range = getRange();
			T newValue = getValue() + 1;
			if (newValue > range.second)
				newValue = range.second;
			setValue(newValue);
		}
		else if (hasListConstraint())
		{
			auto list = getValueList();
			auto it = std::find(list.begin(), list.end(), value());
			if (it != list.end() && it != list.end() - 1)
				setValue(*(it + 1));
		}
		else
		{
			if constexpr (std::is_same_v<bool, T>)
			{
				setValue(!getValue());
			}
			else if constexpr (std::is_arithmetic_v<T>)
			{
				setValue(getValue() + 1);
			}
		}
	}

	template<suku_config_var_type T>
	inline bool ConfigElement<T>::hasPrev() const
	{
		if (hasRangeConstraint())
		{
			return value_ != getRange().first;
		}
		else if (hasListConstraint())
		{
			if (valueList_.size() == 0)
				return false;

			return value_ != valueList_[0];
		}
		else
		{
			if constexpr (std::is_same_v<bool, T>)
			{
				if (value_ == false)
					return false;
				else
					return true;
			}
			else
			{
				return true;
			}
		}
	}

	template<suku_config_var_type T>
	inline bool ConfigElement<T>::hasNext() const
	{
		if (hasRangeConstraint())
		{
			return value_ != getRange().second;
		}
		else if (hasListConstraint())
		{
			if (valueList_.size() == 0)
				return false;

			return value_ != valueList_[valueList_.size() - 1];
		}
		else
		{
			if constexpr (std::is_same_v<bool, T>)
			{
				if (value_ == true)
					return false;
				else
					return true;
			}
			else
			{
				return true;
			}
		}
	}
}