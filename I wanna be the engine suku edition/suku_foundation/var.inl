#pragma once

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
	}

	template<typename T>
	inline T Var::getValue(T _x)
	{
		return _x = std::any_cast<T>(value_);
	}

	template<typename T>
	inline T Var::getValue()
	{
		return std::any_cast<T>(value_);
	}

	template<typename T>
	inline Var::Var(T _x)
	{
		value_ = _x;
	}

	template<typename T>
	inline Var& Var::operator<<(T _x)
	{
		value_ = _x;
		return *this;
	}

	template<typename T>
	inline T& Var::operator>>(T& _x)
	{
		_x = std::any_cast<T>(value_);
		return _x;
	}
}