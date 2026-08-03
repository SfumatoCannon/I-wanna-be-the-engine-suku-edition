#include "save.h"
#include "maths.h"
#include "suku_string.h"
#include "message.h"
#include "file.h"

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
		SaveAssetGlobal::getInstance().refreshLoadTag();
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
		SaveAssetGlobal::getInstance().refreshLoadTag();
		return true;
	}

	template<typename T>
	bool setSavable(Property<T>& _x, const std::string _name)
	{
		//return setSavable(_x.value_, _name);
		return false;
	}

	template<typename T>
	inline void SaveFile::saveVar(const std::string _name, T _val)
	{
		unsigned long long id = maths::hash(_name);
		auto& dataPointerVarPool = SaveAssetGlobal::getInstance().dataPointerVarPool;
		if (dataPointerVarPool.find(id) == dataPointerVarPool.end())
		{
			WARNINGWINDOW("Variable name \"" + _name + "\" not set as savable.Attempting to set it as savable.");
			setSavable<T>(_name);
		}
		Var pointerInVar = dataPointerVarPool[id];
		T* pointer;
		pointerInVar >> pointer;
		*pointer = _val;
		file_->writeDataPtr(id, reinterpret_cast<char*>(pointer), sizeof(T));
	}

	template<typename T>
	void SaveFile::saveVar(const std::string _name, Property<T>& _val)
	{
		//saveVar(_name, _val.value_);
	}

	template<typename T>
	inline void SaveFile::saveVar(T& _x)
	{
		auto& dataPointerVarPool = SaveAssetGlobal::getInstance().dataPointerVarPool;
		auto& varIdMappingPool = SaveAssetGlobal::getInstance().varIdMappingPool;
		if (varIdMappingPool.find(reinterpret_cast<char*>(&_x)) == varIdMappingPool.end())
		{
			ERRORWINDOW("Variable not set as savable");
			return;
		}
		unsigned long long id = varIdMappingPool[reinterpret_cast<char*>(&_x)];
		auto iter = dataPointerVarPool.find(id);
		if (iter == dataPointerVarPool.end())
		{
			ERRORWINDOW("Variable not set as savable. (Var name mapping exists but failed to get data pointer var)");
			return;
		}
		Var pointerInVar = (*iter).second;
		T* pointer;
		pointerInVar >> pointer;
		*pointer = _x;
		file_->writeDataPtr(id, reinterpret_cast<char*>(pointer), sizeof(T));
	}

	template<typename T>
	void SaveFile::saveVar(Property<T>& _x)
	{
		//saveVar(_x.value_);
	}

	template<typename T>
	inline void SaveFile::loadVar(T& _x, T _defaultValue)
	{
		SaveAssetGlobal::getInstance().readData();
		auto& dataPointerVarPool = SaveAssetGlobal::getInstance().dataPointerVarPool;
		auto& varIdMappingPool = SaveAssetGlobal::getInstance().varIdMappingPool;
		if (varIdMappingPool.find(reinterpret_cast<char*>(&_x)) == varIdMappingPool.end())
		{
			ERRORWINDOW("Variable not set as savable");
			return;
		}
		auto iter = dataPointerVarPool.find(varIdMappingPool[reinterpret_cast<char*>(&_x)]);
		if (iter == dataPointerVarPool.end())
		{
			_x = _defaultValue;
			return;
		}
		Var pointerInVar = (*iter).second;
		T* pointer;
		pointerInVar >> pointer;
		_x = *pointer;
	}

	template<typename T>
	void SaveFile::loadVar(Property<T>& _x, T _defaultValue)
	{
		//loadVar(_x.value_, _defaultValue);
	}

	template<typename T>
	T SaveFile::loadVar(const std::string _name, T _defaultValue)
	{
		auto& dataPointerVarPool = SaveAssetGlobal::getInstance().dataPointerVarPool;
		unsigned long long id = maths::hash(_name);
		auto iter = dataPointerVarPool.find(id);
		if (iter == dataPointerVarPool.end())
		{
			return _defaultValue;
		}
		Var pointerInVar = (*iter).second;
		T* pointer;
		pointerInVar >> pointer;
		file_->readDataPtr(id, reinterpret_cast<char*>(pointer), sizeof(T));
		return (*pointer);
	}
}