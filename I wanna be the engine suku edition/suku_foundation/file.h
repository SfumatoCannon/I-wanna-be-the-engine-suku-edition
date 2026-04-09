#pragma once
#include "string.h"
#include <vector>
#include <fstream>
#include <map>

namespace suku
{
	class File
	{
	public:
		File(String _path) : name_(L""), path_(_path) {}
		File(String _name, String _path) : name_(_name), path_(_path) {}

		void setName(String _name) { name_ = _name; }
		String getName()const { return name_; }
		String getPath()const { return path_; }

		void create()const;
		bool isExist();
		bool isOpenedForWrite()const;
		void openForWrite();
		bool tryOpenForWrite();
		bool isOpenedForRead()const;
		void openForRead();
		bool tryOpenForRead();
		void close();
		void closeWrite();
		void closeRead();

		void write(const char* _ptrData, size_t _size);
		void read(char* _ptrData, size_t _size);
		void write(const std::vector<char>& _data);
		void read(std::vector<char>& _data);
		void read(std::vector<char>& _data, size_t _size);

		void writeDataPtrMap(const std::map<unsigned long long, std::pair<char*, size_t>>& _dataPtrMap);
		std::map<unsigned long long, bool> readDataPtrMap(std::map<unsigned long long, std::pair<char*, size_t>>& _dataPtrMap);
	private:
		std::ofstream ofs_;
		std::ifstream ifs_;
		String name_;
		String path_;
	};

	namespace filesystem
	{
		void suku_file_init();

		std::filesystem::path getExeParentPath();

		std::filesystem::path absolutePath(std::filesystem::path _relativePath);
		String absolutePath(String _relativePath);
		String absolutePath(const wchar_t* _relativePath);

		bool isAbsolutePath(std::filesystem::path _path);
		bool isAbsolutePath(String _path);

		void createPath(std::filesystem::path _path);
		void createPath(const wchar_t* _path);
		void createPath(String _path);
	};
}