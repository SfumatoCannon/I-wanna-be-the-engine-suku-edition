#pragma once
#include "var.h"
#include "maths.h"
#include "string.h"
#include <map>
#include <utility>
#include <windows.h>

namespace suku
{
	class SaveAssetGlobal
	{
	private:
		void suku_save_init();
		SaveAssetGlobal() { suku_save_init(); }
	public:
		SaveAssetGlobal(const SaveAssetGlobal&) = delete;
		void operator=(const SaveAssetGlobal&) = delete;
		static SaveAssetGlobal& getInstance()
		{
			static SaveAssetGlobal instance;
			return instance;
		}

		std::map<unsigned long long, std::pair<char*, size_t>> varSaveList;
		std::map<unsigned long long, Var> varFinderList;
		std::map<char*, unsigned long long> varIdList;

		int saveFileId;
		//extern wchar_t saveFilePath[512];
		wchar_t exePath[MAX_PATH + 1];
		size_t Path_len;
	};

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
		auto& varSaveList = SaveAssetGlobal::getInstance().varSaveList;
		auto& varFinderList = SaveAssetGlobal::getInstance().varFinderList;
		auto& varIdList = SaveAssetGlobal::getInstance().varIdList;
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
		auto& varFinderList = SaveAssetGlobal::getInstance().varFinderList;
		auto& varIdList = SaveAssetGlobal::getInstance().varIdList;
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
		auto& varFinderList = SaveAssetGlobal::getInstance().varFinderList;
		auto& varIdList = SaveAssetGlobal::getInstance().varIdList;
		Var pointerInVar = varFinderList[varIdList[reinterpret_cast<char*>(&_x)]];
		T* pointer;
		pointerInVar >> pointer;
		_x = *pointer;
	}
}