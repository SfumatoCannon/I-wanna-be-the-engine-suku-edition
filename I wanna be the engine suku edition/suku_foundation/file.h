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
		File(String _path) : name_(L"save0"), path_(_path) {}
		File(String _name, String _path) : name_(_name), path_(_path) {}

		void setName(String _name) { name_ = _name; }
		String getName()const { return name_; }
		String getPath()const { return path_; }

		void create()const;
		bool isExist();
		void openForWrite();
		bool tryOpenForWrite();
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
		void readDataPtrMap(std::map<unsigned long long, std::pair<char*, size_t>>& _dataPtrMap);
	private:
		std::ofstream ofs_;
		std::ifstream ifs_;
		String name_;
		String path_;
	};

	void suku_file_init();

	String getExePath();

	String absolutePath(String _relativePath);
	const wchar_t* absolutePath(const wchar_t* _relativePath);
	const wchar_t* absolutePath(const char* _relativePath);

	void createPath(const wchar_t* _path);
	void createPath(String _path);
}