#pragma once
namespace suku::memory
{
	template<typename T> T** new_2d(UINT _rows, UINT _cols);
	template<typename T> void delete_2d(T** _pointer, UINT _rows, UINT _cols);

	template<typename T>
	inline T** new_2d(UINT _rows, UINT _cols)
	{
		T** resultPointer = new T * [_rows];
		if (resultPointer != nullptr)
		{
			for (UINT i = 0; i < _rows; i++)
				resultPointer[i] = new T[_cols];
		}
		return resultPointer;
	}

	template<typename T>
	inline void delete_2d(T** _pointer, UINT _rows, UINT _cols)
	{
		if (_pointer == nullptr)
			return;
		for (UINT i = 0; i < _rows; i++)
			if (_pointer[i] != nullptr)
				delete[] _pointer[i];
		delete[] _pointer;
	}
}