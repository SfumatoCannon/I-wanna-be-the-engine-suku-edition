#include "save.h"
#include "../suku_constants.h"
#include "suku_string.h"
#include "message.h"
#include "file.h"
#include <ios>

namespace suku
{
	void SaveAssetGlobal::setSaveFile(SaveFile* _saveFile)
	{
		saveFile_ = _saveFile;
		refreshLoadTag();
	}

	void SaveAssetGlobal::writeData()
	{
		refreshLoadTag();
		if (!saveFile_)
		{
			ERRORWINDOW("SaveFile is NULL");
			return;
		}
		saveFile_->writeData();
	}

	void SaveAssetGlobal::readData()
	{
		if (loadTag_ == true)
			return;
		if (!saveFile_)
		{
			ERRORWINDOW("SaveFile is NULL");
			return;
		}
		dataExistInFilePool = saveFile_->readData();
		loadTag_ = true;
	}

	SaveAssetGlobal::SaveAssetGlobal()
	{
		saveFileId = 0;
		filesystem::suku_file_init();
	}

	SaveFile::SaveFile(String _fileName)
	{
		setFileName(_fileName);
	}

	void SaveFile::writeData()
	{
		if (!file_)
		{
			ERRORWINDOW("SaveFile is NULL");
			return;
		}
		if (!file_->isExist())
		{
			file_->create();
			if (!file_->isExist())
			{
				ERRORWINDOW("Failed to create save file :" 
					+ file_->getName() + "(" + file_->getPath() + ")");
				return;
			}
		}
		file_->writeDataPtrMap(SaveAssetGlobal::getInstance().byteDataPool);
		file_->closeWrite();
	}

	std::map<unsigned long long, bool> SaveFile::readData()
	{
		if (!file_)
		{
			ERRORWINDOW("SaveFile is NULL");
			return std::map<unsigned long long, bool>();
		}
		if (!file_->isExist())
		{
			WARNINGWINDOW("SaveFile doesn't exist in the given path: " + file_->getPath());
			return std::map<unsigned long long, bool>();
		}
		auto existenceMap = file_->readDataPtrMap(SaveAssetGlobal::getInstance().byteDataPool);
		file_->closeRead();
		return existenceMap;
	}

	void SaveFile::setFileName(String _fileName)
	{
		if (file_)
		{
			file_->close();
		}
		filesystem::createPath(L"Save");
		file_ = std::make_unique<File>(_fileName, filesystem::absolutePath(L"Save\\" + _fileName + ".sav"));
	}

	String SaveFile::getFileName()
	{
		if (!file_)
			return String();
		return file_->getName();
	}

	void setSaveFile(SaveFile* _saveFile)
	{
		SaveAssetGlobal::getInstance().setSaveFile(_saveFile);
	}

	SaveFile* getSaveFile()
	{
		return SaveAssetGlobal::getInstance().getSaveFile();
	}

	bool isSavable(const std::string _name)
	{
		unsigned long long id = maths::hash(_name);
		auto& dataPointerVarPool = SaveAssetGlobal::getInstance().dataPointerVarPool;
		return dataPointerVarPool.find(id) != dataPointerVarPool.end();
	}

	bool hasValueInFile(const std::string _name)
	{
		SaveAssetGlobal::getInstance().readData();
		auto& dataExistInFilePool = SaveAssetGlobal::getInstance().dataExistInFilePool;
		unsigned long long id = maths::hash(_name);
		return dataExistInFilePool.find(id) != dataExistInFilePool.end() && dataExistInFilePool[id] == true;
	}
}
