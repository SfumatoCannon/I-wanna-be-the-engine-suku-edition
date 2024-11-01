#include "game_saving.h"

namespace suku
{
	std::map<unsigned long long, std::pair<char*, size_t>> varSaveList;
	std::map<unsigned long long, Var> varFinderList;
	std::map<char*, unsigned long long> varIdList;

	int saveFileId;
	wchar_t saveFilePath[512];

	void createPath(std::string _path)
	{
		_path = "md " + _path;
		system(_path.c_str());
	}

	void createPath(const wchar_t* _path)
	{
		char pathInChar[512] = { 0 };
		sprintf_s(pathInChar, "%ls", _path);
		std::string pathInString;
		pathInString = pathInChar;
		createPath(pathInString);
	}

	void saveToFile()
	{
		wchar_t saveFilePath[512];
		_stprintf_s(saveFilePath, _T("%ls\\save%d"), SAVE_DIR, saveFileId);
		std::ofstream ofs;
		ofs.open(saveFilePath, std::ios::binary);
		if (!ofs.is_open())
		{
			createPath(SAVE_DIR);
			std::ofstream ofsForCreating(saveFilePath);
			ofsForCreating.close();
			ofs.open(saveFilePath, std::ios::binary);
		}
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
		_stprintf_s(saveFilePath, L"%ls\\save%d", SAVE_DIR, saveFileId);
		std::ifstream ifs;
		ifs.open(saveFilePath, std::ios::binary);
		if (!ifs.is_open())
		{
			std::ofstream ofsForCreating(saveFilePath);
			ofsForCreating.close();
			ifs.open(saveFilePath, std::ios::binary);
		}
		while (ifs.good())
		{
			unsigned long long id;
			ifs.read(reinterpret_cast<char*>(&id), sizeof(unsigned long long));
			ifs.read(varSaveList[id].first, varSaveList[id].second);
		}
		ifs.close();
	}
}
