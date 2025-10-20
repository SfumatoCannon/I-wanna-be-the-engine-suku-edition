#include "save.h"
#include "../global_value.h"
#include "../framework.h"
#include <locale>
#include <codecvt>

namespace suku
{

	std::map<unsigned long long, std::pair<char*, size_t>> varSaveList;
	std::map<unsigned long long, Var> varFinderList;
	std::map<char*, unsigned long long> varIdList;

	int saveFileId;
	//wchar_t saveFilePath[512];
	wchar_t exePath[MAX_PATH + 1];
	size_t Path_len;

	void suku_save_init()
	{
		GetModuleFileNameW(NULL, exePath, MAX_PATH);
		(wcsrchr(exePath, L'\\'))[1] = 0;
		Path_len = wcslen(exePath);
	}

	const wchar_t* AbsolutePath(const wchar_t* _relativePath)
	{
		if (!_relativePath) return nullptr;
		if (_relativePath[1] == L':') // the parameter is already an absolute path
			return _relativePath;
		static wchar_t result[MAX_PATH + 1] = { 0 };
		result[0] = L'\0';
		lstrcatW(result, exePath);
		lstrcatW(result, _relativePath);
		return result;
	}

	const wchar_t* AbsolutePath(const char* _relativePath)
	{
		if (!_relativePath) return nullptr;
		if (_relativePath[1] == ':') // the parameter is already an absolute path
			return getWideString(_relativePath);
		wchar_t* wideCharRelativePath = getWideString(_relativePath);
		static wchar_t result[MAX_PATH + 1] = { 0 };
		result[0] = L'\0';
		lstrcatW(result, exePath);
		lstrcatW(result, wideCharRelativePath);
		delete[] wideCharRelativePath;
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
		swprintf_s(saveFilePath, L"%ls\\save%d", SaveDir, saveFileId);
		std::ofstream ofs;
		ofs.open(saveFilePath, std::ios::binary);
		if (!ofs.is_open())
		{
			createPath(SaveDir);
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
		swprintf_s(saveFilePath, L"%ls\\save%d", SaveDir, saveFileId);
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
