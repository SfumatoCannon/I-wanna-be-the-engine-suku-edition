#include "game_saving.h"
#include <locale>
#include <codecvt>

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

	String::String()
	{
		content = nullptr;
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

	std::map<unsigned long long, std::pair<char*, size_t>> varSaveList;
	std::map<unsigned long long, Var> varFinderList;
	std::map<char*, unsigned long long> varIdList;

	int saveFileId;
	wchar_t saveFilePath[512];
	wchar_t exePath[MAX_PATH + 1];
	size_t Path_len;

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

	void createPath(const wchar_t* _path)
	{
		size_t length = wcslen(_path);
		std::wstring str = L"";
		int varForNoWarning;
		for (size_t i = 0; i < length; i++)
		{
			if (_path[i] == L'\\' || _path[i] == L'/')
			{
				if (str != L"")
					varForNoWarning = _wmkdir(str.c_str());
			}
			str += _path[i];
		}
		varForNoWarning = _wmkdir(str.c_str());
	}

	void createPath(String _path)
	{
		createPath(_path.content);
	}

	void saveToFile()
	{
		wchar_t saveFilePath[512];
		swprintf_s(saveFilePath, L"%ls\\save%d", SAVE_DIR, saveFileId);
		std::ofstream ofs;
		ofs.open(saveFilePath, std::ios::binary);
		if (!ofs.is_open())
		{
			createPath(SAVE_DIR);
			std::ofstream ofsForCreating(saveFilePath);
			ofsForCreating.close();
			ofs.open(saveFilePath, std::ios::binary);
		}
		for (const auto& i : varSaveList)
		{
			unsigned long long id = i.first;
			ofs.write(reinterpret_cast<char*>(&id), sizeof(unsigned long long));
			ofs.write(i.second.first, i.second.second);
		}
		ofs.close();
	}

	void loadFromFile()
	{
		wchar_t saveFilePath[512];
		swprintf_s(saveFilePath, L"%ls\\save%d", SAVE_DIR, saveFileId);
		std::ifstream ifs;
		ifs.open(saveFilePath, std::ios::binary);
		if (!ifs.is_open())
		{
			std::ofstream ofsForCreating(saveFilePath);
			ofsForCreating.close();
			ifs.open(saveFilePath, std::ios::binary);
		}
		while (ifs.good())
		{
			unsigned long long id;
			ifs.read(reinterpret_cast<char*>(&id), sizeof(unsigned long long));
			ifs.read(varSaveList[id].first, varSaveList[id].second);
		}
		ifs.close();
	}
}
