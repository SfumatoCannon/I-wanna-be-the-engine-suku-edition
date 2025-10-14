#pragma once
namespace suku
{
	template<typename T> T** new_memory_2d(UINT _width, UINT _height);
	template<typename T> void delete_memory_2d(T** _pointer, UINT _width, UINT _height);

	template<typename T>
	inline T** new_memory_2d(UINT _width, UINT _height)
	{
		T** resultPointer = new T * [_height];
		if (resultPointer != nullptr)
		{
			for (UINT i = 0; i < _height; i++)
				resultPointer[i] = new T[_width];
		}
		return resultPointer;
	}

	template<typename T>
	inline void delete_memory_2d(T** _pointer, UINT _width, UINT _height)
	{
		if (_pointer == nullptr)
			return;
		for (UINT i = 0; i < _height; i++)
			if (_pointer[i] != nullptr)
				delete[] _pointer[i];
		delete[] _pointer;
	}
}