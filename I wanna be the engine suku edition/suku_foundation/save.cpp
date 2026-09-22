#include "save.h"
#include "../suku_constants.h"
#include "suku_string.h"
#include "message.h"
#include "file.h"
#include <suku_core/room_pool.h>

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

	SaveFile::SaveFile(const SaveFile& _other)
	{
		setFileName(_other.getFileName());
	}

	bool SaveFile::isExist() const
	{
		return file_->isExist();
	}

	void SaveFile::clear()
	{
		if (!file_)
		{
			ERRORWINDOW("SaveFile is NULL");
			return;
		}
		if (!file_->isExist())
		{
			WARNINGWINDOW("SaveFile doesn't exist: " + file_->getPath());
			return;
		}
		bool result = file_->tryOpenForWrite(true);
		if (result)
		{
			file_->closeWrite();
		}
	}

	void SaveFile::deleteFile()
	{
		if (!file_)
		{
			ERRORWINDOW("SaveFile is NULL");
			return;
		}
		file_->deleteFile();
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

	String SaveFile::getFileName()const
	{
		if (!file_)
			return String();
		return file_->getName();
	}

	void SaveFile::save()
	{
		SaveFile* savefile = getGlobalSaveFile();
		Room* nowRoom = RoomPool::getNowRoom();
		if (savefile == nullptr || nowRoom == nullptr)
			return;
		savefile->saveVar("roomid", nowRoom->getRoomId());
		savefile->writeData();
		nowRoom->onSave();
	}

	bool SaveFile::load()
	{
		SaveFile* savefile = getGlobalSaveFile();
		if (savefile == nullptr)
			return false;
		Typecode roomid = savefile->loadVar<Typecode>("roomid");
		RoomPool::gotoRoom(roomid);
		savefile->readData();
		return true;
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

	void setGlobalSaveFile(SaveFile* _saveFile)
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
