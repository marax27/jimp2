#include <initializer_list>
#include <iostream>
#include <cstring>  //memcpy
#include <string>

using std::cout;
using std::ostream;

/*
 * Implementacja macierzy NxM w C++.
 * 
 */

//************************************************************

class Matrix
{
	typedef unsigned int ui;

public:
	Matrix(ui x, ui y);
	Matrix(const Matrix &model) { copy(model); }
	Matrix(const std::initializer_list<std::initializer_list<double>> &values);
	~Matrix(){ free(); }

	// Copy a matrix.
	Matrix& operator=(const Matrix &model);

	// Element access.
	// Convention: matrix.at(number_of_row, number_of_column)
	// Note: row and column numbering starts from 0.
	double& at(ui r, ui c);
	const double& at(ui r, ui c) const;

	// Basic matrix operations.
	Matrix operator+(const Matrix &right) const;
	Matrix operator-(const Matrix &right) const;
	Matrix operator*(const Matrix &right) const;
	Matrix operator*(double right) const;
	Matrix operator/(double right) const;

	// Unary operators.
	Matrix operator-() const;

	Matrix operator+() const {
		return *this;
	}

	// Transposition.
	Matrix transpose() const;

	// Determinant.
	double det() const;

	// Getters.
	ui getColumns() const { return columns; }
	ui getRows() const { return rows; }

	// Matrices comparison.
	friend bool operator==(const Matrix &left, const Matrix &right);
	friend bool operator!=(const Matrix &left, const Matrix &right);

	// Create identity matrix.
	static Matrix identity(ui dimension);

	// Check matrix's properties.
	bool isZero() const;
	bool isIdentity() const;
	bool isSquare() const;
	bool isDiagonal() const;

	//--------------------
	class MatrixException : public std::exception {};
	
	struct InitializerListDimensionException : public MatrixException {
		virtual const char* what() const noexcept{
			return "Cannot convert initializer_list to a matrix.";
		}
	};
	struct OutOfRangeException : public MatrixException {
		virtual const char* what() const noexcept{
			return "Indices out of range.";
		}
	};
	struct OperationForbiddenException : public MatrixException {
		virtual const char* what() const noexcept{
			return "Required operation cannot be performed: invalid argument/arguments.";
		}
	};
	struct ZeroDimensionException : public MatrixException {
		virtual const char* what() const noexcept{
			return "Cannot initialize an empty matrix.";
		}
	};
	struct InitializationException : public MatrixException {
		virtual const char* what() const noexcept{
			return "Failed to allocate memory for a matrix.";
		}
	};
	//--------------------

private:
	ui rows, columns;
	double *data;

	void init(bool init_with_zeroes=true);
	void copy(const Matrix &model);
	void free();
};

//************************************************************

std::ostream& operator<<(std::ostream &out, const Matrix &m);
Matrix operator*(double left, const Matrix &right);

//************************************************************

int main(){
	Matrix A{{1, 2, 3, 4},
	         {5, 6, 7, 8}};
	Matrix B{{0, 2, 4, 6},
	         {1, 0, 0, 1}};
	Matrix C{{2},
	         {3},
			 {5},
			 {7}};

	cout << A+B << '\n'
	     << A-B << '\n'
		 << 5*C << '\n'
		 << C/2 << '\n'
	     << A*C << '\n'
		 << C.transpose() << '\n'
		 << Matrix::identity(5) << '\n';
	
	Matrix D{{6, 0, -1, 2}, {-1, 2, 3, 6}, {4, -3, 0, 0}, {1, 5, 7, 2}};
	cout << "Determinant of\n" << D << "is: " << D.det() << '\n';

	Matrix E(C), F = D, G(2, 4);
	G = A;
}

//////////////////////////////////////////////////////////////

ostream& operator<<(ostream &out, const Matrix &m){
	for(unsigned i=0; i!=m.getRows(); ++i){
		for(unsigned j=0; j!=m.getColumns(); ++j)
			out << m.at(i, j) << '\t';
		out << '\n';
	}
	return out;
}

Matrix operator*(double left, const Matrix &right){
	return right * left;
}

//************************************************************

Matrix::Matrix(ui rows, ui columns)
	: rows(rows), columns(columns), data(nullptr){
	init();
}

//************************************************************

Matrix::Matrix(const std::initializer_list<std::initializer_list<double>> &values){
	//values == {{row #1}, {row #2}, ..., {row #n}}
	rows = values.size();
	if(rows){
		columns = 0;
		data = nullptr;
	}
	else
		throw InitializerListDimensionException();
	
	bool set_columns = true;
	for(const auto &i : values){
		if(set_columns){
			columns = i.size();
			set_columns = false;
		}
		else if(i.size() != columns)
			throw InitializerListDimensionException();
	}

	init();
	ui index = 0;
	for(const auto &i : values){
		for(const double j : i)
			data[index++] = j;
	}
}

//************************************************************

Matrix& Matrix::operator=(const Matrix &model){
	free();
	copy(model);
	return *this;
}

//************************************************************

double& Matrix::at(ui r, ui c){
	if( r >= rows || c >= columns )
		throw OutOfRangeException();
	return data[c + columns*r];
}

const double& Matrix::at(ui r, ui c) const{
	if( r >= rows || c >= columns )
		throw OutOfRangeException();
	return data[c + columns*r];
}

//************************************************************

Matrix Matrix::operator+(const Matrix &right) const{
	if(rows != right.rows || columns != right.columns)
		throw OperationForbiddenException();
	
	Matrix result(rows, columns);
	for(ui i=0; i!=rows*columns; ++i)
		result.data[i] = data[i] + right.data[i];
	return result;
}

//************************************************************

Matrix Matrix::operator-(const Matrix &right) const{
	if(rows != right.rows || columns != right.columns)
		throw OperationForbiddenException();
	
	Matrix result(rows, columns);
	for(ui i=0; i!=rows*columns; ++i)
		result.data[i] = data[i] - right.data[i];
	return result;
}

//************************************************************

Matrix Matrix::operator*(const Matrix &right) const{
	if(columns != right.rows)
		throw OperationForbiddenException();
	
	Matrix result(rows, right.columns);
	for(ui y=0; y!=result.rows; ++y){
		for(ui x=0; x!=result.columns; ++x){
			double *elem = &result.at(y, x);
			*elem = 0.0;
			for(ui k=0; k!=this->columns; ++k)
				*elem += at(y, k) * right.at(k, x);
		}
	}
	return result;
}

//************************************************************

Matrix Matrix::operator*(double right) const{
	Matrix result(rows, columns);
	
	for(ui i=0; i!=rows*columns; ++i)
		result.data[i] = data[i] * right;
	return result;
}

//************************************************************

Matrix Matrix::operator/(double right) const{
	return *this * (1.0 / right);
}

//************************************************************

// Slightly faster than -1.0 * matrix
Matrix Matrix::operator-() const{
	Matrix result(rows, columns);
	for(ui i = 0; i != rows*columns; ++i)
		result.data[i] = -this->data[i];
	return result;
}

//************************************************************

Matrix Matrix::transpose() const{
	//Matrix[x, y] == Transpose[y, x]
	Matrix result(columns, rows);

	for(ui i = 0; i != rows; ++i)
		for(ui j = 0; j != columns; ++j)
			result.at(j, i) = at(i, j);

	return result;
}

//************************************************************

// Determinant; algorithm based on Gaussian elimination.
double Matrix::det() const{
	if(rows != columns)
		throw OperationForbiddenException();

	Matrix m(*this);

	double data_holder = 0,
	       result = 1.0;
	int counter = 0,
	    det_sign = 1,
		size = rows;
	
	for(int i=0; i<size-1; ++i){
		while(!m.at(i, i)){
			if(counter == size-i-1)
				return 0;
			++counter;
			for(int n=0; n<size; ++n)
				std::swap(m.at(i, n), m.at(i+counter, n));
			
			det_sign = -det_sign;
		}

		counter = 0;

		for(int j=i+1; j<size; ++j){
			data_holder = m.at(j, i);
			for(int n=0; n<size; ++n)
				m.at(j, n) -= data_holder * m.at(i, n) / m.at(i, i);
		}
	}

	for(int i=0; i!=size; ++i)
		result *= m.at(i, i);
	
	if(!result)
		result = 0.0;  //in case of -0;
	
	return det_sign * result;
}

//************************************************************

bool operator==(const Matrix &left, const Matrix &right){
	if(left.columns != right.columns || left.rows != right.rows)
		return false;
	
	for(Matrix::ui i = 0; i != left.rows*left.columns; ++i){
		if(abs(left.data[i] - right.data[i]) > 10e-9)
			return false;
	}
	return true;
}

bool operator!=(const Matrix &left, const Matrix &right){
	return !(left == right);
}

//************************************************************

Matrix Matrix::identity(ui dimension){
	Matrix result(dimension, dimension);
	for(ui i=0; i!=dimension; ++i)
		result.at(i, i) = 1;
	return result;
}

//************************************************************

bool Matrix::isZero() const{
	return *this == Matrix(rows, columns);
}

bool Matrix::isIdentity() const{
	return isSquare() && (*this == Matrix::identity(rows));
}

bool Matrix::isSquare() const{
	return rows == columns;
}

bool Matrix::isDiagonal() const{
	for(ui y=0; y!=rows; ++y){
		for(ui x=0; x!=columns; ++x){
			if(x != y && at(y, x))
				return false;
		}
	}
	return true;
}

//*** Private methods ****************************************

// Allocate and initialize data memory.
void Matrix::init(bool init_with_zeroes){
	if(rows*columns == 0)
		throw ZeroDimensionException();
	data = new double[rows*columns];
	if(!data)
		throw InitializationException();

	if(init_with_zeroes){
		// Initialize all fields with zeroes.
		for(ui i = 0; i != rows*columns; ++i)
			data[i] = 0.0;
	}
}

// Copy model's elements to *this. ASSUME no memory is allocated in *this.
void Matrix::copy(const Matrix &model){
	rows = model.rows;
	columns = model.columns;
	init(false);
	memcpy(data, model.data, rows*columns*sizeof(double));
}

// Free memory and reset all fields.
void Matrix::free(){
	if(data){
		delete[] data;
		data = nullptr;
	}
	rows = columns = 0;
}

//************************************************************
