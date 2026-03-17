#include "save.h"
#include "../global_value.h"
#include "message.h"
#include "file.h"
#include <ios>

namespace suku
{
	SaveAssetGlobal::SaveAssetGlobal()
	{
		saveFileId = 0;
		suku_file_init();
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
}
