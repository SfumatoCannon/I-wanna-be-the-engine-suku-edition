#pragma once
#include <string>
#include <compare>
#include <Windows.h>

namespace suku
{
	class String
	{
	public:
		wchar_t* content;
		std::wstring contentInWString()const;
		std::string contentInString()const;
		String() : content(new wchar_t[1] {L'\0'}) {}
		String(const char* _string);
		String(std::string _string);
		String(const wchar_t* _wstring);
		String(std::wstring _wstring);
		String(const String& _other);

		void clear();
		bool isEmpty()const;

		bool operator==(const String& _other)const;
		std::strong_ordering operator<=>(const String& _other)const
		{
			if (content == nullptr && _other.content == nullptr)
				return std::strong_ordering::equal;
			if (content == nullptr)
				return std::strong_ordering::less;
			if (_other.content == nullptr)
				return std::strong_ordering::greater;
			return (lstrcmpW(content, _other.content) <=> 0);
		}
		void operator=(const String& _other);
		String operator+(const String& _other);
		String operator+(const char* _string);
		String operator+(const wchar_t* _wstring);
		String operator+(std::string _string);
		String operator+(std::wstring _wstring);
		void operator+=(const String& _other);
		void operator+=(const char* _string);
		void operator+=(const wchar_t* _wstring);
		void operator+=(std::string _string);
		void operator+=(std::wstring _wstring);
		~String();
	};
	String operator+(const char* _string1, const String& _string2);
	String operator+(const wchar_t* _string1, const String& _string2);
	String operator+(std::string _string1, const String& _string2);
	String operator+(std::wstring _string1, const String& _string2);

	String getFileTypeFromURL(const String& _url);

	wchar_t getWideChar(char _multiByteChar);
	char getMultiByteChar(wchar_t _wideChar);
	wchar_t* getWideString(const char* _multiByteString);
	char* getMultiByteString(const wchar_t* _wideString);
}