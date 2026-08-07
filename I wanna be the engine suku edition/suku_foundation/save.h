#pragma once
#include <map>
#include <utility>
#include <windows.h>
#include <fstream>
#include "../suku_core/suku_property.h"

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
		// mapping from variable address to savable_var id
		std::map<const char*, unsigned long long> varIdMappingPool;

		int saveFileId;

		void setSaveFile(SaveFile* _saveFile);
		SaveFile* getSaveFile() { return saveFile_; }

		void refreshLoadTag() { loadTag_ = false; }
	private:
		bool loadTag_ = false;
		SaveFile* saveFile_ = nullptr;
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

		template<typename T> void saveVar(const std::string _name, T _val);
		template<typename T> void saveVar(const std::string _name, Property<T>& _val);
		template<typename T> void saveVar(T& _x);
		template<typename T> void saveVar(Property<T>& _x);
		template<typename T> void loadVar(T& _x, T _defaultValue = T());
		template<typename T> void loadVar(Property<T>& _x, T _defaultValue = T());
		template<typename T> T loadVar(const std::string _name, T _defaultValue = T());

		bool hasValue(const std::string _name);
	private:
		std::unique_ptr<File> file_ = nullptr;
	};

	void setSaveFile(SaveFile* _saveFile);
	SaveFile* getGlobalSaveFile();
	template<typename T> bool setSavable(const std::string _name);
	template<typename T> bool setSavable(T& _x, const std::string _name);
	template<typename T> bool setSavable(Property<T>& _x, const std::string _name);
	bool isSavable(const std::string _name);
}

#include "save.inl"