#pragma once
#include "var.h"
#include "maths.h"
#include "string.h"
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
		//extern wchar_t saveFilePath[512];
		wchar_t exePath[MAX_PATH + 1];
		size_t Path_len;

	private:
		void suku_save_init();
		SaveAssetGlobal() { suku_save_init(); }
	};

	class SaveFile
	{
	public:
		SaveFile(String _path) : name_(L"save0"), path_(_path) {}	
		SaveFile(String _name, String _path) : name_(_name), path_(_path) {}

		void create()const;
		void openForWrite();
		bool tryOpenForWrite();
		void openForRead();
		bool tryOpenForRead();
		void close();

		void write(char* _ptrData, size_t _size)const;
		void read(char* _ptrData, size_t _size)const;

		void writeDataPtrMap(const std::map<unsigned long long, std::pair<char*, size_t>>& _dataPtrMap);
		void readDataPtrMap(std::map<unsigned long long, std::pair<char*, size_t>>& _dataPtrMap);
	private:
		std::ofstream ofs_;
		std::ifstream ifs_;
		String name_;
		String path_;
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
}

#include "save.inl"