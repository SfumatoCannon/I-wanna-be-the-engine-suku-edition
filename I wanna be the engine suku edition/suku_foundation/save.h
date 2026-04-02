#pragma once
#include "var.h"
#include <map>
#include <utility>
#include <windows.h>
#include <fstream>

namespace suku
{
	class String;
	class SaveFile;
	class File;

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
		std::map<unsigned long long, Var> dataPointerVarPool;
		// mapping from variable address to savable_var id
		std::map<const char*, unsigned long long> varIdMappingPool;

		int saveFileId;
		SaveFile* saveFile = nullptr;
		void writeData();
		void readData();
	private:
		SaveAssetGlobal();
	};

	class SaveFile
	{
	public:
		SaveFile() = default;
		SaveFile(String _fileName);

		void writeData();
		void readData();
		void setFileName(String _fileName);
		String getFileName();
	private:
		std::unique_ptr<File> file_ = nullptr;
	};

	void setSaveFile(SaveFile* _saveFile);
	SaveFile* getSaveFile();
	template<typename T> bool setSavable(const std::string _name);
	template<typename T> bool setSavable(const std::string _name, T _initialVal);
	template<typename T> bool setSavable(T& _x, const std::string _name);
	template<typename T> void saveVar(const std::string _name, T _val);
	template<typename T> void saveVar(const T& _x);
	template<typename T> void loadVar(T& _x);
	template<typename T> T loadVar(const std::string _name);
	bool isSavable(const std::string _name);
}

#include "save.inl"