#pragma once
#include <string>

namespace suku
{
	class String
	{
	public:
		wchar_t* content;
		std::wstring contentInWString()const;
		std::string contentInString()const;
		String() : content(nullptr) {}
		String(const char* _string);
		String(std::string _string);
		String(const wchar_t* _wstring);
		String(std::wstring _wstring);
		String(const String& _other);
		bool operator==(const String& _other)const;
		auto operator<=>(const String& _other)const;
		void operator=(const String& _other);
		String operator+(const String& _other);
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