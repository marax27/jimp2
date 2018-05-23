#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <new>  //std::bad_alloc
#include <cstdint>  //uint32_t
#include <initializer_list>

template<typename T, uint32_t _Rows, uint32_t _Columns>
class Matrix{
public:
	// typedef std::size_t size_type;
	typedef uint32_t size_type;	

	Matrix();
	Matrix(Matrix &&model);
	Matrix(const Matrix &model);
	Matrix(std::initializer_list<std::initializer_list<T>> list);

	~Matrix();

	Matrix& operator=(const Matrix &model){
		free();
		copyElements(model);
	}

	// Not actual matrix multiplication; instead, multiples
	// coresponding elements independently.
	Matrix operator*(const Matrix &right) const {
		Matrix result = *this;
		result *= right;
		return result;
	}

	Matrix& operator*=(const Matrix &right);

	// Multiply each element of a matrix by 'right'.
	Matrix operator*(double right) const {
		Matrix result = *this;
		result *= right;
		return result;
	}

	Matrix& operator*=(double right);

	// Data access.

	inline T& at(size_type row, size_type column){
		if(row >= _Rows || column >= _Columns)
			throw std::out_of_range("Matrix::at: demanded element out of range.");
		return operator()(row, column);
	}

	inline const T& at(size_type row, size_type column) const{
		if(row >= _Rows || column >= _Columns)
			throw std::out_of_range("Matrix::at: demanded element out of range.");
		return operator()(row, column);
	}

	inline T& operator()(size_type row, size_type column){
		return data[row*_Columns + column];
	}

	inline const T& operator()(size_type row, size_type column) const{
		return data[row*_Columns + column];
	}

	// Getters.

	constexpr size_type size() const { return _Rows * _Columns; }
	constexpr size_type rows() const { return _Rows; }
	constexpr size_type columns() const { return _Columns; }

private:
	T *data;

	inline void free(){
		if(data){
			delete[] data;
			data = nullptr;
		}
	}

	void copyElements(const Matrix &model);
};

//************************************************************

// Default constructor.
template<typename T, uint32_t _Rows, uint32_t _Columns>
Matrix<T,_Rows,_Columns>::Matrix(){
	data = new T[size()];
}

// Move constructor.
template<typename T, uint32_t _Rows, uint32_t _Columns>
Matrix<T,_Rows,_Columns>::Matrix(Matrix<T,_Rows,_Columns> &&model)
	: data(model.data) {}

// Copy constructor.
template<typename T, uint32_t _Rows, uint32_t _Columns>
Matrix<T,_Rows,_Columns>::Matrix(
	const Matrix<T,_Rows,_Columns> &model){

	copyElements(model);
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
	data = new T[size()];
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
	free();
}

//************************************************************

template<typename T, uint32_t _Rows, uint32_t _Columns>
Matrix<T,_Rows,_Columns>& Matrix<T,_Rows,_Columns>::operator*=(
	const Matrix<T,_Rows,_Columns> &right) {
	
	for(size_type i = 0; i != size(); ++i)
		data[i] *= right.data[i];
	return *this;
}

//************************************************************

template<typename T, uint32_t _Rows, uint32_t _Columns>
Matrix<T,_Rows,_Columns>& Matrix<T,_Rows,_Columns>::operator*=(
	double right) {
	
	for(size_type i = 0; i != size(); ++i)
		data[i] *= right;
	return *this;
}

//************************************************************

template<typename T, uint32_t _Rows, uint32_t _Columns>
void Matrix<T,_Rows,_Columns>::copyElements(
	const Matrix<T,_Rows,_Columns> &model){

	size_type sz = size();
	data = new T[sz];
	for(size_type i = 0; i != sz; ++i)
		data[i] = model.data[i];
}

//************************************************************


// double * Matrix
template<typename T, uint32_t _Rows, uint32_t _Columns>
Matrix<T,_Rows,_Columns> operator*(double left,
	const Matrix<T,_Rows,_Columns> &right) {
	
	return right * left;
}



#endif  //_MATRIX_H_
