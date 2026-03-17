#pragma once
#include "var.h"
#include <map>
#include <utility>
#include <windows.h>
#include <fstream>

namespace suku
{
	class SaveAssetGlobal
	{
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
	private:
		SaveAssetGlobal();
	};

	void saveToFile();
	void loadFromFile();
	template<typename T> bool setSavable(T& _x, std::string _name);
	template<typename T> void saveVar(T& _x);
	template<typename T> void loadVar(T& _x);
}

#include "save.inl"