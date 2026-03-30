#include "save.h"
#include "maths.h"

namespace suku
{
	template<typename T>
	inline bool setSavable(T& _x, std::string _name)
	{
		unsigned long long id = maths::hash(_name);
		auto& byteDataPool = SaveAssetGlobal::getInstance().byteDataPool;
		auto& savableVarPool = SaveAssetGlobal::getInstance().savableVarPool;
		auto& varIdMappingPool = SaveAssetGlobal::getInstance().varIdMappingPool;
		if (byteDataPool.find(id) != byteDataPool.end())
			return false;
		T* pointer = new T;
		*pointer = _x;
		char* address = reinterpret_cast<char*>(pointer);
		Var pointerInVar;
		pointerInVar << pointer;
		byteDataPool[id] = std::make_pair(address, sizeof(_x));
		savableVarPool[id] = pointerInVar;
		varIdMappingPool[reinterpret_cast<char*>(&_x)] = id;
		return true;
	}

	template<typename T>
	inline void saveVar(T& _x)
	{
		auto& savableVarPool = SaveAssetGlobal::getInstance().savableVarPool;
		auto& varIdMappingPool = SaveAssetGlobal::getInstance().varIdMappingPool;
		Var pointerInVar = savableVarPool[varIdMappingPool[reinterpret_cast<char*>(&_x)]];
		T* pointer;
		pointerInVar >> pointer;
		*pointer = _x;
		SaveAssetGlobal::getInstance().writeData();
	}

	template<typename T>
	inline void loadVar(T& _x)
	{
		SaveAssetGlobal::getInstance().readData();
		auto& savableVarPool = SaveAssetGlobal::getInstance().savableVarPool;
		auto& varIdMappingPool = SaveAssetGlobal::getInstance().varIdMappingPool;
		Var pointerInVar = savableVarPool[varIdMappingPool[reinterpret_cast<char*>(&_x)]];
		T* pointer;
		pointerInVar >> pointer;
		_x = *pointer;
	}
}