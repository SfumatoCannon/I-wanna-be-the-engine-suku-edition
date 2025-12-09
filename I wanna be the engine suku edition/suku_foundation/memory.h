#pragma once
#include <utility>
#include <vector>

namespace suku::memory
{
	template<typename T>
	class Array2D
	{
	public:
		Array2D(size_t _rows, size_t _cols) : rows_(_rows), cols_(_cols), data_(_rows* _cols) {}
		std::pair<size_t, size_t> getSize()const
		{
			return { rows_, cols_ };
		}
		T& operator[](size_t _rowIndex, size_t _colIndex)
		{
			return data_[_rowIndex * cols_ + _colIndex]
		}
		const T& operator[](size_t _rowIndex, size_t _colIndex)const
		{
			return data_[_rowIndex * cols_ + _colIndex];
		}
	private:
		size_t rows_, cols_;
		std::vector<T> data_;
	};

	// not recommended to use below functions
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