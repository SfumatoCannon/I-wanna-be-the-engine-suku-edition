#include "config_element.h"
#include "config_file.h"
#include "../suku_foundation/suku_string.h"

namespace suku
{
	template<typename T>
	inline ConfigElement<T>::ConfigElement(std::string _name, T _defaultValue) : name_(String(_name).contentInWString())
	{
		T valueT = ConfigFile::loadVar(_name, _defaultValue);
		value_ << valueT;
	}

	template<typename T>
	inline ConfigElement<T>::ConfigElement(std::wstring _name, T _defaultValue) : name_(_name)
	{
		T valueT = ConfigFile::loadVar(_name, _defaultValue);
		value_ << valueT;
	}

	template<typename T>
	inline void ConfigElement<T>::setValue(T _value)
	{
		value_ << _value;
		ConfigFile::saveVar(name_, _value);
	}

	template<typename T>
	inline T ConfigElement<T>::value()
	{
		return value_.getValue<T>();
	}
}