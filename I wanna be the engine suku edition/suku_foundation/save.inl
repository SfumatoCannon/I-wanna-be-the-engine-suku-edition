#include "save.h"

namespace suku
{
	template<typename T>
	inline bool setSavable(T& _x, std::string _name)
	{
		unsigned long long id = maths::hash(_name);
		auto& varSaveList = SaveAssetGlobal::getInstance().varSaveList;
		auto& varFinderList = SaveAssetGlobal::getInstance().varFinderList;
		auto& varIdList = SaveAssetGlobal::getInstance().varIdList;
		if (varSaveList.find(id) != varSaveList.end())
			return false;
		T* pointer = new T;
		*pointer = _x;
		char* address = reinterpret_cast<char*>(pointer);
		Var pointerInVar;
		pointerInVar << pointer;
		varSaveList[id] = std::make_pair(address, sizeof(_x));
		varFinderList[id] = pointerInVar;
		varIdList[reinterpret_cast<char*>(&_x)] = id;
		return true;
	}

	template<typename T>
	inline void saveVar(T& _x)
	{
		auto& varFinderList = SaveAssetGlobal::getInstance().varFinderList;
		auto& varIdList = SaveAssetGlobal::getInstance().varIdList;
		Var pointerInVar = varFinderList[varIdList[reinterpret_cast<char*>(&_x)]];
		T* pointer;
		pointerInVar >> pointer;
		*pointer = _x;
		saveToFile();
	}

	template<typename T>
	inline void loadVar(T& _x)
	{
		loadFromFile();
		auto& varFinderList = SaveAssetGlobal::getInstance().varFinderList;
		auto& varIdList = SaveAssetGlobal::getInstance().varIdList;
		Var pointerInVar = varFinderList[varIdList[reinterpret_cast<char*>(&_x)]];
		T* pointer;
		pointerInVar >> pointer;
		_x = *pointer;
	}
}