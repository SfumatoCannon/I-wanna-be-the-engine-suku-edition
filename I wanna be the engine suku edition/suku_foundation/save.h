#pragma once
#include "var.h"
#include "maths.h"
#include "string.h"
#include <map>
#include <utility>
#include <windows.h>

namespace suku
{
	void suku_save_init();

	extern std::map<unsigned long long, std::pair<char*, size_t>> varSaveList;
	extern std::map<unsigned long long, Var> varFinderList;
	extern std::map<char*, unsigned long long> varIdList;

	extern int saveFileId;
	//extern wchar_t saveFilePath[512];
	extern wchar_t exePath[MAX_PATH + 1];
	extern size_t Path_len;

	const wchar_t* AbsolutePath(const wchar_t* _relativePath);
	const wchar_t* AbsolutePath(const char* _relativePath);

	void createPath(const wchar_t* _path);
	void createPath(String _path);

	void saveToFile();
	void loadFromFile();
	template<typename T> bool setSavable(T& _x, std::string _name);
	template<typename T> void saveVar(T& _x);
	template<typename T> void loadVar(T& _x);

	template<typename T>
	inline bool setSavable(T& _x, std::string _name)
	{
		unsigned long long id = maths::hash(_name);
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