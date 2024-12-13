#pragma once
#include "framework.h"
#include "suku_maths.h"
#include "global_value.h"

namespace suku
{
	class String
	{
	public:
		wchar_t* content;
		String();
		String(const char* _string);
		String(std::string _string);
		String(const wchar_t* _wstring);
		String(std::wstring _wstring);
		String(const String& _other);
		void operator=(const String& _other);
		~String();
	};

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

	extern std::map<unsigned long long, std::pair<char*, size_t>> varSaveList;
	extern std::map<unsigned long long, Var> varFinderList;
	extern std::map<char*, unsigned long long> varIdList;

	extern int saveFileId;
	extern wchar_t saveFilePath[512];
	extern wchar_t exePath[MAX_PATH + 1];
	extern size_t Path_len;

	wchar_t getWideChar(const char& _multiByteChar);
	wchar_t* getWideString(const char* _multiByteString);

	void createPath(const wchar_t* _path);
	void createPath(String _path);

	void saveToFile();
	void loadFromFile();
	template<typename T> bool setSavable(T& _x, std::string _name);
	template<typename T> void saveVar(T& _x);
	template<typename T> void loadVar(T& _x);

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

	template<typename T>
	inline bool setSavable(T& _x, std::string _name)
	{
		unsigned long long id = hash(_name);
		if (varSaveList.find(id) != varSaveList.end())
			return false;
		T* pointer = new T;
		*pointer = _x;
		char* address = reinterpret_cast<char*>(pointer);
		Var pointerInVar;
		pointerInVar << pointer;
		varSaveList[id] = std::make_pair(address, sizeof(_x));
		varFinderList[id] = pointerInVar;
		varIdList[reinterpret_cast<char*>(&_x)] = id;
		return true;
	}

	template<typename T>
	inline void saveVar(T& _x)
	{
		Var pointerInVar = varFinderList[varIdList[reinterpret_cast<char*>(&_x)]];
		T* pointer;
		pointerInVar >> pointer;
		*pointer = _x;
		saveToFile();
	}

	template<typename T>
	inline void loadVar(T& _x)
	{
		loadFromFile();
		Var pointerInVar = varFinderList[varIdList[reinterpret_cast<char*>(&_x)]];
		T* pointer;
		pointerInVar >> pointer;
		_x = *pointer;
	}
}