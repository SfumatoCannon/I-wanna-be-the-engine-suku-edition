#include "string.h"
#include <tchar.h>
#include "windows.h"

namespace suku
{
	std::wstring String::contentInWString()const
	{
		return std::wstring(content);
	}

	std::string String::contentInString()const
	{
		char* str = getMultiByteString(content);
		std::string result(str);
		delete[] str;
		return result;
	}

	String::String(const char* _string)
	{
		content = getWideString(_string);
	}

	String::String(std::string _string)
	{
		content = getWideString(_string.c_str());
	}

	String::String(const wchar_t* _wstring)
	{
		size_t length = wcslen(_wstring);
		content = new wchar_t[length + 1];
		memcpy_s(content, length * sizeof(wchar_t), _wstring, length * sizeof(wchar_t));
		content[length] = L'\0';
	}

	String::String(std::wstring _wstring)
	{
		const wchar_t* wstring = _wstring.c_str();
		size_t length = wcslen(wstring);
		content = new wchar_t[length + 1];
		memcpy_s(content, length * sizeof(wchar_t), wstring, length * sizeof(wchar_t));
		content[length] = L'\0';
	}

	String::String(const String& _other)
	{
		const wchar_t* wstring = _other.content;
		size_t length = wcslen(wstring);
		content = new wchar_t[length + 1];
		memcpy_s(content, length * sizeof(wchar_t), wstring, length * sizeof(wchar_t));
		content[length] = L'\0';
	}

	bool String::operator==(const String& _other)const
	{
		if (content == nullptr && _other.content == nullptr)
			return true;
		if (content == nullptr || _other.content == nullptr)
			return false;
		return (lstrcmpW(content, _other.content) == 0);
	}

	auto String::operator<=>(const String& _other)const
	{
		if (content == nullptr && _other.content == nullptr)
			return std::strong_ordering::equal;
		if (content == nullptr)
			return std::strong_ordering::less;
		if (_other.content == nullptr)
			return std::strong_ordering::greater;
		return (lstrcmpW(content, _other.content) <=> 0);
	}

	void String::operator=(const String& _other)
	{
		if (content != nullptr)
		{
			delete[] content;
			content = nullptr;
		}
		if (_other.content == nullptr)
			return;
		const wchar_t* wstring = _other.content;
		size_t length = wcslen(wstring);
		content = new wchar_t[length + 1];
		memcpy_s(content, length * sizeof(wchar_t), wstring, length * sizeof(wchar_t));
		content[length] = L'\0';
	}

	String String::operator+(const String& _other)
	{
		return String(contentInWString() + _other.contentInWString());
	}

	String::~String()
	{
		if (content != nullptr)
			delete[] content;
	}
	String operator+(const char* _string1, const String& _string2)
	{
		String result(_string1);
		result = result + _string2;
		return result;
	}

	String operator+(const wchar_t* _string1, const String& _string2)
	{
		String result(_string1);
		result = result + _string2;
		return result;
	}

	String operator+(std::string _string1, const String& _string2)
	{
		String result(_string1);
		result = result + _string2;
		return result;
	}

	String operator+(std::wstring _string1, const String& _string2)
	{
		String result(_string1);
		result = result + _string2;
		return result;
	}

	String getFileTypeFromURL(const String& _url)
	{
		wchar_t* content = _url.content;
		size_t length = wcslen(_url.content);
		for (int i = (int)length - 1; i >= 0; i--)
		{
			if (content[i] == L'.')
			{
				if (i == length - 1)
					return String();

				String result;
				result.content = new wchar_t[length - i];
				memcpy_s(result.content, (length - i - 1) * sizeof(wchar_t),
					content + (i + 1), (length - i - 1) * sizeof(wchar_t));
				result.content[length - i - 1] = L'\0';
				return result;
			}
		}
		return String();
	}

	wchar_t getWideChar(char _multiByteChar)
	{
		return wchar_t(_multiByteChar);
	}

	char getMultiByteChar(wchar_t _wideChar)
	{
		return char(_wideChar);
	}

	wchar_t* getWideString(const char* _multiByteString)
	{
		int length = MultiByteToWideChar(CP_ACP, 0, _multiByteString, (int)strlen(_multiByteString), nullptr, 0);
		wchar_t* result = new wchar_t[length + 1];
		MultiByteToWideChar(CP_ACP, 0, _multiByteString, (int)strlen(_multiByteString), result, length);
		result[length] = L'\0';
		return result;
	}

	char* getMultiByteString(const wchar_t* _wideString)
	{
		int length = WideCharToMultiByte(CP_ACP, 0, _wideString, (int)wcslen(_wideString), nullptr, 0, nullptr, nullptr);
		char* result = new char[length + 1];
		WideCharToMultiByte(CP_ACP, 0, _wideString, (int)wcslen(_wideString), result, length, nullptr, nullptr);
		result[length] = '\0';
		return result;
	}

}