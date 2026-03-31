#include "save.h"
#include "../global_value.h"
#include "string.h"
#include "message.h"
#include "file.h"
#include <ios>

namespace suku
{
	void SaveAssetGlobal::writeData()
	{
		if (!saveFile)
		{
			ERRORWINDOW("SaveFile is NULL");
			return;
		}
		saveFile->writeData();
	}

	void SaveAssetGlobal::readData()
	{
		if (!saveFile)
		{
			ERRORWINDOW("SaveFile is NULL");
			return;
		}
		saveFile->readData();
	}

	SaveAssetGlobal::SaveAssetGlobal()
	{
		saveFileId = 0;
		suku_file_init();
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
		file_->readDataPtrMap(SaveAssetGlobal::getInstance().byteDataPool);
		file_->closeRead();
	}

	void SaveFile::setFileName(String _fileName)
	{
		if (file_)
		{
			file_->close();
		}
		file_ = std::make_unique<File>(_fileName, absolutePath(L"Save\\" + _fileName + ".sav"));
	}

	String SaveFile::getFileName()
	{
		if (!file_)
			return String();
		return file_->getName();
	}

	void setSaveFile(SaveFile* _saveFile)
	{
		SaveAssetGlobal::getInstance().saveFile = _saveFile;
	}

	SaveFile* getSaveFile()
	{
		return SaveAssetGlobal::getInstance().saveFile;
	}
}
