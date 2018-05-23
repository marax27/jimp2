#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <new>  //std::bad_alloc
#include <cstdint>  //uint32_t
#include <initializer_list>

template<typename T, uint32_t _Rows, uint32_t _Columns>
class Matrix{
public:
	typedef std::size_t size_type;

	Matrix();
	Matrix(Matrix &&model);
	Matrix(const Matrix &model);
	Matrix(std::initializer_list<std::initializer_list<T>> list);

	~Matrix();

	// Not actual matrix multiplication; instead, multiples
	// coresponding elements independently.
	Matrix operator*(const Matrix &right) const;

	// Data access.

	T& at(size_type row, size_type column){
		if(row >= _Rows || column >= _Columns)
			throw std::out_of_range("Matrix::at: demanded element out of range.");
		return operator()(row, column);
	}

	const T& at(size_type row, size_type column) const{
		if(row >= _Rows || column >= _Columns)
			throw std::out_of_range("Matrix::at: demanded element out of range.");
		return operator()(row, column);
	}

	T& operator()(size_type row, size_type column){
		return data[row*_Columns + column];
	}

	const T& operator()(size_type row, size_type column) const{
		return data[row*_Columns + column];
	}

private:
	T *data;
};

//************************************************************

// Default constructor.
template<typename T, uint32_t _Rows, uint32_t _Columns>
Matrix<T,_Rows,_Columns>::Matrix(){
	data = new T[_Rows * _Columns];
}

// Move constructor.
template<typename T, uint32_t _Rows, uint32_t _Columns>
Matrix<T,_Rows,_Columns>::Matrix(Matrix<T,_Rows,_Columns> &&model)
	: data(model.data) {}

// Copy constructor.
template<typename T, uint32_t _Rows, uint32_t _Columns>
Matrix<T,_Rows,_Columns>::Matrix(const Matrix<T,_Rows,_Columns> &model){
	size_type size = _Rows * _Columns;
	data = new T[size];
	for(size_type i = 0; i != size; ++i)
		data[i] = model.data[i];
}

// An initializer_list-based constructor.
template<typename T, uint32_t _Rows, uint32_t _Columns>
Matrix<T,_Rows,_Columns>::Matrix(
	std::initializer_list<std::initializer_list<T>> list){
	// Check if numbers of rows and columns match.
	if(list.size() != _Rows)
		throw std::invalid_argument("Matrix::Matrix: invalid number of rows.");
	for(auto i : list){
		if(i.size() != _Columns)
			throw std::invalid_argument("Matrix::Matrix: invalid number of columns.");
	}
		
	// Copy data from initializer_list.
	data = new T[_Rows * _Columns];
	size_type idx = 0;
	for(auto row : list){
		for(auto elem : row)
			data[idx++] = elem;
	}
}

//************************************************************

// Destructor.
template<typename T, uint32_t _Rows, uint32_t _Columns>
Matrix<T,_Rows,_Columns>::~Matrix(){
	if(data){
		delete[] data;
		data = nullptr;
	}
}

//************************************************************

#endif  //_MATRIX_H_
