#pragma once

#include "message.h"

namespace suku
{
	template<typename T>
	inline bool Var::operator==(T _x)
	{
		return std::any_cast<T>(value_) == _x;
	}

	template<typename T>
	inline Var& Var::operator+=(T _x)
	{
		value_ = std::any_cast<T>(value_) + _x;
		return (*this);
	}

	template<typename T>
	inline Var& Var::operator-=(T _x)
	{
		value_ = std::any_cast<T>(value_) - _x;
		return (*this);
	}

	template<typename T>
	inline Var& Var::operator*=(T _x)
	{
		value_ = std::any_cast<T>(value_) * _x;
		return (*this);
	}

	template<typename T>
	inline Var& Var::operator/=(T _x)
	{
		value_ = std::any_cast<T>(value_) / _x;
		return (*this);
	}

	template<typename T>
	inline bool Var::operator!=(T _x)
	{
		return std::any_cast<T>(value_) != _x;
	}

	template<typename T>
	inline bool Var::operator<(T _x)
	{
		return std::any_cast<T>(value_) < _x;
	}

	template<typename T>
	inline bool Var::operator>(T _x)
	{
		return std::any_cast<T>(value_) > _x;
	}

	template<typename T>
	inline bool Var::operator<=(T _x)
	{
		return std::any_cast<T>(value_) <= _x;
	}

	template<typename T>
	inline bool Var::operator>=(T _x)
	{
		return std::any_cast<T>(value_) >= _x;
	}

	template<typename T>
	inline Var Var::operator+(T _x)
	{
		Var res;
		res.value_ = std::any_cast<T>(value_) + _x;
		return res;
	}

	template<typename T>
	inline Var Var::operator-(T _x)
	{
		Var res;
		res.value_ = std::any_cast<T>(value_) - _x;
		return res;
	}

	template<typename T>
	inline Var Var::operator*(T _x)
	{
		Var res;
		res.value_ = std::any_cast<T>(value_) * _x;
		return res;
	}

	template<typename T>
	inline Var Var::operator/(T _x)
	{
		Var res;
		res.value_ = std::any_cast<T>(value_) / _x;
		return res;
	}

	template<typename T>
	inline void Var::assign(T _x)
	{
		value_ = _x;
		pTypeInfo_ = &typeid(T);
	}

	template<typename T>
	inline T Var::getValue(T _x)
	{
		try
		{
			return _x = std::any_cast<T>(value_);
		}
		catch (std::bad_any_cast& e)
		{
			ERRORWINDOW("Failed to get value: " + std::string(e.what()) + "\n"
				+ "request type: " + typeid(T).name() + "\n"
				+ "storage type: " + pTypeInfo_->name());
			return _x;
		}
	}

	template<typename T>
	inline T Var::getValue()
	{
		try
		{
			return std::any_cast<T>(value_);
		}
		catch (std::bad_any_cast& e)
		{
			ERRORWINDOW("Failed to get value: " + std::string(e.what()) + "\n"
				+ "request type: " + typeid(T).name() + "\n"
				+ "storage type: " + pTypeInfo_->name());
			return T();
		}
	}

	template<typename T>
	inline Var::Var(T _x)
	{
		value_ = _x;
		pTypeInfo_ = &typeid(T);
	}

	template<typename T>
	inline Var& Var::operator<<(T _x)
	{
		value_ = _x;
		pTypeInfo_ = &typeid(T);
		return *this;
	}

	template<typename T>
	inline T& Var::operator>>(T& _x)
	{
		try
		{
			_x = std::any_cast<T>(value_);
			return _x;
		}
		catch (std::bad_any_cast& e)
		{
			ERRORWINDOW("Failed to get value: " + std::string(e.what()) + "\n"
				+ "request type: " + typeid(T).name() + "\n"
				+ "storage type: " + pTypeInfo_->name());
			return _x;
		}
	}
}