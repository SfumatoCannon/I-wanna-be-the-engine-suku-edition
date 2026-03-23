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

		std::map<unsigned long long, std::pair<char*, size_t>> byteDataPool;
		std::map<unsigned long long, Var> savableVarPool;
		std::map<char*, unsigned long long> varIdMappingPool;

		int saveFileId;
	private:
		SaveAssetGlobal();
	};

	class SaveFile
	{
		template<typename T> void save(T& _x);
		void save();
		template<typename T> void load(T& _x);
		void load();
	};

	void saveToFile();
	void loadFromFile();
	template<typename T> bool setSavable(T& _x, std::string _name);
	template<typename T> void saveVar(T& _x);
	template<typename T> void loadVar(T& _x);
}

#include "save.inl"