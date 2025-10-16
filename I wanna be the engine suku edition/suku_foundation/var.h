#pragma once
#include <any>

namespace suku
{
	class Var
	{
	private:
		std::any value_;
	public:
		Var() {};
		template<typename T> Var(T _x);
		template<typename T> Var& operator <<(T _x);
		template<typename T> T& operator >>(T& _x);
		template<typename T> bool operator ==(T _x);
		template<typename T> Var& operator +=(T _x);
		template<typename T> Var& operator -=(T _x);
		template<typename T> Var& operator *=(T _x);
		template<typename T> Var& operator /=(T _x);
		template<typename T> bool operator !=(T _x);
		template<typename T> bool operator <(T _x);
		template<typename T> bool operator >(T _x);
		template<typename T> bool operator <=(T _x);
		template<typename T> bool operator >=(T _x);
		template<typename T> Var operator +(T _x);
		template<typename T> Var operator -(T _x);
		template<typename T> Var operator *(T _x);
		template<typename T> Var operator /(T _x);
		template<typename T> void assign(T _x);
		template<typename T> T getValue(T _x);
		template<typename T> T getValue();
	};
}

#include "var.inl"