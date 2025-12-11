#pragma once
#include <utility>
#include <vector>

namespace suku::memory
{
	template<typename T>
	class Array2D
	{
	public:
		Array2D() : rows_(0), cols_(0) {}
		Array2D(size_t _rows, size_t _cols) : rows_(_rows), cols_(_cols), data_(_rows* _cols) {}

		std::pair<size_t, size_t> getSize()const { return { rows_, cols_ }; }
		size_t getRows()const { return rows_; }
		size_t getCols()const { return cols_; }
		bool isEmpty()const { return data_.empty(); }
		void resize(size_t _rows, size_t _cols)
		{
			rows_ = _rows;
			cols_ = _cols;
			data_.resize(_rows * _cols);
		}

		std::vector<T>::reference operator()(size_t _rowIndex, size_t _colIndex)
		{
			return data_[_rowIndex * cols_ + _colIndex];
		}
		std::vector<T>::const_reference operator()(size_t _rowIndex, size_t _colIndex)const
		{
			return data_[_rowIndex * cols_ + _colIndex];
		}
	private:
		size_t rows_, cols_;
		std::vector<T> data_;
	};
}