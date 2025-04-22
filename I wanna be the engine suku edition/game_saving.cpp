#include "game_saving.h"
#include <locale>
#include <codecvt>

namespace suku
{

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
