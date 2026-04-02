#include "save.h"
#include "maths.h"
#include "string.h"
#include "message.h"

namespace suku
{
	template<typename T>
	bool setSavable(const std::string _name)
	{
		// 无需做 varIdMappingPool 映射，其余保持一致
		unsigned long long id = maths::hash(_name);
		auto& byteDataPool = SaveAssetGlobal::getInstance().byteDataPool;
		auto& dataPointerVarPool = SaveAssetGlobal::getInstance().dataPointerVarPool;
		if (byteDataPool.find(id) != byteDataPool.end())
			return false;
		T* pointer = new T;
		char* address = reinterpret_cast<char*>(pointer);
		Var pointerInVar;
		pointerInVar << pointer;
		byteDataPool[id] = std::make_pair(address, sizeof(T));
		dataPointerVarPool[id] = pointerInVar;
		SaveAssetGlobal::getInstance().writeData();
		return true;
	}

	template<typename T>
	inline bool setSavable(std::string _name, T _initialVal)
	{
		unsigned long long id = maths::hash(_name);
		auto& byteDataPool = SaveAssetGlobal::getInstance().byteDataPool;
		auto& dataPointerVarPool = SaveAssetGlobal::getInstance().dataPointerVarPool;
		if (byteDataPool.find(id) != byteDataPool.end())
			return false;
		T* pointer = new T(_initialVal);
		char* address = reinterpret_cast<char*>(pointer);
		Var pointerInVar;
		pointerInVar << pointer;
		byteDataPool[id] = std::make_pair(address, sizeof(T));
		dataPointerVarPool[id] = pointerInVar;
		SaveAssetGlobal::getInstance().writeData();
		return true;
	}

	template<typename T>
	inline bool setSavable(T& _x, std::string _name)
	{
		unsigned long long id = maths::hash(_name);
		auto& byteDataPool = SaveAssetGlobal::getInstance().byteDataPool;
		auto& dataPointerVarPool = SaveAssetGlobal::getInstance().dataPointerVarPool;
		auto& varIdMappingPool = SaveAssetGlobal::getInstance().varIdMappingPool;
		if (byteDataPool.find(id) != byteDataPool.end())
			return false;
		T* pointer = new T;
		*pointer = _x;
		char* address = reinterpret_cast<char*>(pointer);
		Var pointerInVar;
		pointerInVar << pointer;
		byteDataPool[id] = std::make_pair(address, sizeof(_x));
		dataPointerVarPool[id] = pointerInVar;
		varIdMappingPool[reinterpret_cast<char*>(&_x)] = id;
		SaveAssetGlobal::getInstance().writeData();
		return true;
	}

	template<typename T>
	inline void saveVar(const std::string _name, T _val)
	{
		unsigned long long id = maths::hash(_name);
		auto& dataPointerVarPool = SaveAssetGlobal::getInstance().dataPointerVarPool;
		if (dataPointerVarPool.find(id) == dataPointerVarPool.end())
		{
			WARNINGWINDOW_GLOBAL("Variable name \"" + _name + "\" not set as savable.Attempting to set it as savable.");
			setSavable<T>(_name, _val);
		}
		Var pointerInVar = dataPointerVarPool[id];
		T* pointer;
		pointerInVar >> pointer;
		*pointer = _val;
		SaveAssetGlobal::getInstance().writeData();
	}

	template<typename T>
	inline void saveVar(const T& _x)
	{
		auto& dataPointerVarPool = SaveAssetGlobal::getInstance().dataPointerVarPool;
		auto& varIdMappingPool = SaveAssetGlobal::getInstance().varIdMappingPool;
		if (varIdMappingPool.find(reinterpret_cast<const char*>(&_x)) == varIdMappingPool.end())
		{
			ERRORWINDOW_GLOBAL("Variable not set as savable");
			return;
		}
		auto iter = dataPointerVarPool.find(varIdMappingPool[reinterpret_cast<const char*>(&_x)]);
		if (iter == dataPointerVarPool.end())
		{
			ERRORWINDOW_GLOBAL("Variable not set as savable. (Var name mapping exists but failed to get data pointer var)");
			return;
		}
		Var pointerInVar = (*iter);
		T* pointer;
		pointerInVar >> pointer;
		*pointer = _x;
		SaveAssetGlobal::getInstance().writeData();
	}

	template<typename T>
	inline void loadVar(T& _x)
	{
		SaveAssetGlobal::getInstance().readData();
		auto& dataPointerVarPool = SaveAssetGlobal::getInstance().dataPointerVarPool;
		auto& varIdMappingPool = SaveAssetGlobal::getInstance().varIdMappingPool;
		Var pointerInVar = dataPointerVarPool[varIdMappingPool[reinterpret_cast<char*>(&_x)]];
		T* pointer;
		pointerInVar >> pointer;
		_x = *pointer;
	}

	template<typename T>
	T loadVar(const std::string _name)
	{
		SaveAssetGlobal::getInstance().readData();
		auto& dataPointerVarPool = SaveAssetGlobal::getInstance().dataPointerVarPool;
		unsigned long long id = maths::hash(_name);
		Var pointerInVar = dataPointerVarPool[id];
		T* pointer;
		pointerInVar >> pointer;
		return (*pointer);
	}
}