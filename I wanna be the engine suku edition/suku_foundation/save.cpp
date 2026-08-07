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

	void SaveFile::readData()
	{
		if (!file_)
		{
			ERRORWINDOW("SaveFile is NULL");
			return;
		}
		if (!file_->isExist())
		{
			WARNINGWINDOW("SaveFile doesn't exist in the given path: " + file_->getPath());
			return;
		}
		file_->readDataPtrMap(SaveAssetGlobal::getInstance().byteDataPool);
		file_->closeRead();
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

	bool SaveFile::hasValue(const std::string _name)
	{
		auto idList = file_->readDataPtrMapIdList();
		unsigned long long id = maths::hash(_name);
		for (auto& i : idList)
		{
			if (i.first == id)
				return true;
		}
		return false;
	}

	void setSaveFile(SaveFile* _saveFile)
	{
		SaveAssetGlobal::getInstance().setSaveFile(_saveFile);
	}

	SaveFile* getGlobalSaveFile()
	{
		return SaveAssetGlobal::getInstance().getSaveFile();
	}

	bool isSavable(const std::string _name)
	{
		unsigned long long id = maths::hash(_name);
		auto& byteDataPool = SaveAssetGlobal::getInstance().byteDataPool;
		return byteDataPool.find(id) != byteDataPool.end();
	}
}
