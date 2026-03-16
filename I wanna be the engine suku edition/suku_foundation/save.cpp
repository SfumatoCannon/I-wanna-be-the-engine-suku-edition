#include "save.h"
#include "../global_value.h"
#include "message.h"
#include <ios>

namespace suku
{
	void SaveAssetGlobal::suku_save_init()
	{
		GetModuleFileNameW(NULL, exePath, MAX_PATH);
		(wcsrchr(exePath, L'\\'))[1] = 0;
		Path_len = wcslen(exePath);
	}

	String absolutePath(String _relativePath)
	{
		if (_relativePath.content == nullptr) return String();
		if (_relativePath.content[1] == ':') // the parameter is already an absolute path
			return _relativePath;
		static wchar_t result[MAX_PATH + 1] = { 0 };
		result[0] = L'\0';
		auto exePath = SaveAssetGlobal::getInstance().exePath;
		lstrcatW(result, exePath);
		lstrcatW(result, _relativePath.content);
		return String(result);
	}

	const wchar_t* absolutePath(const wchar_t* _relativePath)
	{
		if (!_relativePath) return nullptr;
		if (_relativePath[1] == L':') // the parameter is already an absolute path
			return _relativePath;
		static wchar_t result[MAX_PATH + 1] = { 0 };
		result[0] = L'\0';
		auto exePath = SaveAssetGlobal::getInstance().exePath;
		lstrcatW(result, exePath);
		lstrcatW(result, _relativePath);
		return result;
	}

	const wchar_t* absolutePath(const char* _relativePath)
	{
		if (!_relativePath) return nullptr;
		if (_relativePath[1] == ':') // the parameter is already an absolute path
			return getWideString(_relativePath);
		wchar_t* wideCharRelativePath = getWideString(_relativePath);
		static wchar_t result[MAX_PATH + 1] = { 0 };
		result[0] = L'\0';
		auto exePath = SaveAssetGlobal::getInstance().exePath;
		lstrcatW(result, exePath);
		lstrcatW(result, wideCharRelativePath);
		delete[] wideCharRelativePath;
		return result;
	}

	void createPath(const wchar_t* _path)
	{
		size_t length = wcslen(_path);
		std::wstring str = L"";
		int varForNoWarning;
		for (size_t i = 0; i < length; i++)
		{
			if (_path[i] == L'\\' || _path[i] == L'/')
			{
				if (str != L"")
					varForNoWarning = _wmkdir(str.c_str());
			}
			str += _path[i];
		}
		varForNoWarning = _wmkdir(str.c_str());
	}

	void createPath(String _path)
	{
		createPath(_path.content);
	}

	void saveToFile()
	{
		wchar_t saveFilePath[512];
		auto saveFileId = SaveAssetGlobal::getInstance().saveFileId;
		swprintf_s(saveFilePath, L"%ls\\save%d", SaveDir, saveFileId);
		std::ofstream ofs;
		ofs.open(saveFilePath, std::ios::binary);
		if (!ofs.is_open())
		{
			createPath(SaveDir);
			std::ofstream ofsForCreating(saveFilePath);
			ofsForCreating.close();
			ofs.open(saveFilePath, std::ios::binary);
		}
		auto& varSaveList = SaveAssetGlobal::getInstance().varSaveList;
		for (const auto& i : varSaveList)
		{
			unsigned long long id = i.first;
			ofs.write(reinterpret_cast<char*>(&id), sizeof(unsigned long long));
			ofs.write(i.second.first, i.second.second);
		}
		ofs.close();
	}

	void loadFromFile()
	{
		wchar_t saveFilePath[512];
		auto saveFileId = SaveAssetGlobal::getInstance().saveFileId;
		swprintf_s(saveFilePath, L"%ls\\save%d", SaveDir, saveFileId);
		std::ifstream ifs;
		ifs.open(saveFilePath, std::ios::binary);
		if (!ifs.is_open())
		{
			std::ofstream ofsForCreating(saveFilePath);
			ofsForCreating.close();
			ifs.open(saveFilePath, std::ios::binary);
		}
		auto& varSaveList = SaveAssetGlobal::getInstance().varSaveList;
		while (ifs.good())
		{
			unsigned long long id;
			ifs.read(reinterpret_cast<char*>(&id), sizeof(unsigned long long));
			ifs.read(varSaveList[id].first, varSaveList[id].second);
		}
		ifs.close();
	}

	void File::create() const
	{
		std::ofstream ofsForCreating(absolutePath(/*SaveDir + */path_).contentInWString());
		ofsForCreating.close();
	}

	void File::openForWrite()
	{
		if (ofs_.is_open())
			ofs_.close();
		ofs_.open(absolutePath(path_).contentInWString(), std::ios::binary);
		if (!ofs_.is_open())
		{
			create();
			ofs_.open(absolutePath(path_).contentInWString(), std::ios::binary);
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
		write(_data.data(), _data.size());
	}

	void File::read(std::vector<char>& _data, size_t _size)
	{
		if (_size == 0) return;
		if (_data.size() < _size) _data.resize(_size);
		read(_data.data(), _size);
	}

	void File::writeDataPtrMap(const std::map<unsigned long long, std::pair<char*, size_t>>& _dataPtrMap)
	{
		for (const auto& [id, data] : _dataPtrMap)
		{
			ofs_.write(reinterpret_cast<const char*>(&id), sizeof(unsigned long long));
			ofs_.write(data.first, data.second);
		}
	}

	void File::readDataPtrMap(std::map<unsigned long long, std::pair<char*, size_t>>& _dataPtrMap)
	{
		while (ifs_.good())
		{
			unsigned long long id;
			ifs_.read(reinterpret_cast<char*>(&id), sizeof(unsigned long long));
			if (ifs_.eof())
			{
				WARNINGWINDOW("Unexpected file end. The save file may be corrupted.");
				break;
			}
			auto& data = _dataPtrMap[id];
			ifs_.read(data.first, data.second);
		}
	}
}
