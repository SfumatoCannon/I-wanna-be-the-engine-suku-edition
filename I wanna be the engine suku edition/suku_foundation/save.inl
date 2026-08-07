#include "save.h"
#include "maths.h"
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
		if (byteDataPool.find(id) != byteDataPool.end())
			return false;
		T* pointer = new T;
		char* address = reinterpret_cast<char*>(pointer);
		byteDataPool[id] = std::make_pair(address, sizeof(T));
		SaveAssetGlobal::getInstance().refreshLoadTag();
		return true;
	}

	template<typename T>
	inline bool setSavable(T& _x, std::string _name)
	{
		unsigned long long id = maths::hash(_name);
		auto& byteDataPool = SaveAssetGlobal::getInstance().byteDataPool;
		auto& varIdMappingPool = SaveAssetGlobal::getInstance().varIdMappingPool;
		if (byteDataPool.find(id) != byteDataPool.end())
			return false;
		T* pointer = new T;
		*pointer = _x;
		char* address = reinterpret_cast<char*>(pointer);
		byteDataPool[id] = std::make_pair(address, sizeof(_x));
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
		auto& byteDataPool = SaveAssetGlobal::getInstance().byteDataPool;
		auto iter = byteDataPool.find(id);
		if (iter == byteDataPool.end())
		{
			WARNINGWINDOW("Variable name \"" + _name + "\" not set as savable.Attempting to set it as savable.");
			setSavable<T>(_name);
		}
		T* dataPtr = reinterpret_cast<T*>(iter->second.first);
		*dataPtr = _val;
		file_->writeDataPtr(id, iter->second.first, iter->second.second);
	}

	template<typename T>
	void SaveFile::saveVar(const std::string _name, Property<T>& _val)
	{
		//saveVar(_name, _val.value_);
	}

	template<typename T>
	inline void SaveFile::saveVar(T& _x)
	{
		auto& varIdMappingPool = SaveAssetGlobal::getInstance().varIdMappingPool;
		auto& byteDataPool = SaveAssetGlobal::getInstance().byteDataPool;
		if (varIdMappingPool.find(reinterpret_cast<char*>(&_x)) == varIdMappingPool.end())
		{
			ERRORWINDOW("Variable not set as savable");
			return;
		}
		unsigned long long id = varIdMappingPool[reinterpret_cast<char*>(&_x)];
		auto iter = byteDataPool.find(id);
		if (iter == byteDataPool.end())
		{
			ERRORWINDOW("Variable not set as savable. (Var name mapping exists but failed to get data pointer var)");
			return;
		}
		T* dataPtrT = reinterpret_cast<T*>(iter->second.first);
		*dataPtrT = _x;
		file_->writeDataPtr(id, iter->second.first, sizeof(T));
	}

	template<typename T>
	void SaveFile::saveVar(Property<T>& _x)
	{
		//saveVar(_x.value_);
	}

	template<typename T>
	inline void SaveFile::loadVar(T& _x, T _defaultValue)
	{
		auto& idPool = SaveAssetGlobal::getInstance().varIdMappingPool;
		auto iter = idPool.find(reinterpret_cast<char*>(&_x));
		if (iter == idPool.end())
		{
			ERRORWINDOW("Variable not set as savable");
			return;
		}
		unsigned long long id = (*iter);
		auto& [dataPtr, size] = SaveAssetGlobal::getInstance().byteDataPool[id];
		if (file_->readDataPtr(id, dataPtr, size))
		{
			T* dataPtrT = reinterpret_cast<T*>(dataPtr);
			_x = *dataPtrT;
		}
		else
		{
			_x = _defaultValue;
		}
	}

	template<typename T>
	void SaveFile::loadVar(Property<T>& _x, T _defaultValue)
	{
		//loadVar(_x.value_, _defaultValue);
	}

	template<typename T>
	T SaveFile::loadVar(const std::string _name, T _defaultValue)
	{
		unsigned long long id = maths::hash(_name);
		auto& byteDataPool = SaveAssetGlobal::getInstance().byteDataPool;
		if (byteDataPool.find(id) == byteDataPool.end())
		{
			ERRORWINDOW("Variable not set as savable");
			return _defaultValue;
		}
		auto& [dataPtr, size] = byteDataPool[id];
		if (file_->readDataPtr(id, dataPtr, size))
		{
			T* dataPtrT = reinterpret_cast<T*>(dataPtr);
			return *dataPtrT;
		}
		else
		{
			return _defaultValue;
		}
	}
}