#include "pch.h"
#include "file.h"
#include "save.h"
#include "message.h"
#include "string.h"
#include <wchar.h>
#include <filesystem>

namespace suku
{
	std::filesystem::path exeParentPath;

	void suku_file_init()
	{
		if (!exeParentPath.empty())
			return;

		std::wstring buffer;
		buffer.resize(256);
		DWORD len = 0;
		while (true)
		{
			len = GetModuleFileNameW(nullptr, buffer.data(), (DWORD)buffer.size());
			if (len < buffer.size())
				break;
			buffer.resize(buffer.size() * 2);
		}
		buffer.resize(len);
		exeParentPath = std::filesystem::path(buffer).parent_path();
	}

	std::filesystem::path getExeParentPath()
	{
		if (exeParentPath.empty())
			suku_file_init();
		return exeParentPath;
	}

	std::filesystem::path absolutePath(std::filesystem::path _relativePath)
	{
		return getExeParentPath() / _relativePath;
	}

	String absolutePath(String _relativePath)
	{
		std::filesystem::path path(_relativePath.content);
		return String(getExeParentPath() / path);
	}

	String absolutePath(const wchar_t* _relativePath)
	{
		std::filesystem::path path(_relativePath);
		return String(getExeParentPath() / path);
	}

	bool isAbsolutePath(std::filesystem::path _path)
	{
		return _path.is_absolute();
	}

	bool isAbsolutePath(String _path)
	{
		return std::filesystem::path(_path.content).is_absolute();
	}

	void createPath(std::filesystem::path _path)
	{
		if (!isAbsolutePath(_path))
			_path = absolutePath(_path);
		std::error_code errorCode;
		std::filesystem::create_directories(_path, errorCode);
		if (errorCode)
		{
			ERRORWINDOW_GLOBAL("Failed to create path: " + errorCode.message());
		}
	}

	void createPath(const wchar_t* _path)
	{
		createPath(std::filesystem::path(_path));
	}

	void createPath(String _path)
	{
		createPath(_path.content);
	}

	void File::create() const
	{
		std::ofstream ofsForCreating(absolutePath(/*SaveDir + */path_).contentInWString());
		ofsForCreating.close();
	}

	bool File::isExist()
	{
		String absPath = absolutePath(path_);
		if (absPath.content == nullptr)
			return false;
		DWORD attrs = GetFileAttributesW(absPath.content);
		if (attrs == INVALID_FILE_ATTRIBUTES)
			return false;
		if (attrs & FILE_ATTRIBUTE_DIRECTORY)
			return false;
		return true;
	}

	bool File::isOpenedForWrite() const
	{
		return ofs_.is_open();
	}

	void File::openForWrite()
	{
		if (ofs_.is_open())
			ofs_.close();
		ofs_.open(absolutePath(path_).contentInWString(), std::ios::binary | std::ios::trunc);
		if (!ofs_.is_open())
		{
			create();
			ofs_.open(absolutePath(path_).contentInWString(), std::ios::binary | std::ios::trunc);
		}
	}

	bool File::tryOpenForWrite()
	{
		if (ofs_.is_open())
			ofs_.close();
		ofs_.open(absolutePath(path_).contentInWString(), std::ios::binary);
		if (!ofs_.is_open())
			return false;
		return true;
	}

	bool File::isOpenedForRead() const
	{
		return ifs_.is_open();
	}

	void File::openForRead()
	{
		if (ifs_.is_open())
			ifs_.close();
		ifs_.open(absolutePath(path_).contentInWString(), std::ios::binary);
		if (!ifs_.is_open())
		{
			create();
			ifs_.open(absolutePath(path_).contentInWString(), std::ios::binary);
		}
	}

	bool File::tryOpenForRead()
	{
		if (ifs_.is_open())
			ifs_.close();
		ifs_.open(absolutePath(path_).contentInWString(), std::ios::binary);
		if (!ifs_.is_open())
			return false;
		return true;
	}

	void File::close()
	{
		if (ofs_.is_open())
			ofs_.close();
		if (ifs_.is_open())
			ifs_.close();
	}

	void File::closeWrite()
	{
		if (ofs_.is_open())
			ofs_.close();
	}

	void File::closeRead()
	{
		if (ifs_.is_open())
			ifs_.close();
	}

	void File::write(const char* _ptrData, size_t _size)
	{
		if (!ofs_.is_open())
			openForWrite();
		ofs_.write(_ptrData, _size);
	}

	void File::read(char* _ptrData, size_t _size)
	{
		if (!ifs_.is_open())
			openForRead();
		ifs_.read(_ptrData, _size);
	}

	void File::write(const std::vector<char>& _data)
	{
		if (!ofs_.is_open())
			openForWrite();
		write(_data.data(), _data.size());
	}

	void File::read(std::vector<char>& _data)
	{
		if (!ifs_.is_open())
			openForRead();
		ifs_.seekg(0, std::ios::end);
		std::streamsize _fileSize = ifs_.tellg();
		ifs_.seekg(0, std::ios::beg);
		read(_data, static_cast<size_t>(_fileSize));
	}

	void File::read(std::vector<char>& _data, size_t _size)
	{
		if (!ifs_.is_open())
			openForRead();
		if (_size == 0) return;
		if (_data.size() < _size) _data.resize(_size);
		read(_data.data(), _size);
	}

	void File::writeDataPtrMap(const std::map<unsigned long long, std::pair<char*, size_t>>& _dataPtrMap)
	{
		if (!ofs_.is_open())
			openForWrite();
		
		for (const auto& [id, data] : _dataPtrMap)
		{
			ofs_.write(reinterpret_cast<const char*>(&id), sizeof(unsigned long long));
			ofs_.write(reinterpret_cast<const char*>(&data.second), sizeof(size_t));
			ofs_.write(data.first, data.second);
		}
	}

	std::map<unsigned long long, bool> File::readDataPtrMap(std::map<unsigned long long, std::pair<char*, size_t>>& _dataPtrMap)
	{
		if (!ifs_.is_open())
			openForRead();
		std::map<unsigned long long, bool> existenceMap;
		unsigned long long id = 0LL;
		while (true)
		{
			if (!ifs_.read(reinterpret_cast<char*>(&id), sizeof(unsigned long long)))
			{
				if (ifs_.eof())
					break;
				else
				{
					WARNINGWINDOW("Failed to read ID. File may be corrupted.");
					break;
				}
			}

			size_t dataSize = 0;
			if (!ifs_.read(reinterpret_cast<char*>(&dataSize), sizeof(size_t)))
			{
				WARNINGWINDOW("Failed to read data size. File may be corrupted.");
				break;
			}

			existenceMap[id] = true;

			if (_dataPtrMap.find(id) == _dataPtrMap.end())
			{
				ifs_.seekg(dataSize, std::ios::cur);
				continue;
			}

			auto& data = _dataPtrMap[id];

			if (!ifs_.read(data.first, data.second))
			{
				WARNINGWINDOW("Unexpected file end when reading data. File may be corrupted.");
				break;
			}
		}
		return existenceMap;
	}
}