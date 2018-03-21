#include "matrix.hpp"

#include <iostream>
using std::cout;
using std::cerr;
using std::ostream;

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
		throw MatrixException("Non-positive dimension.");
	
	bool set_columns = true;
	for(const auto &i : values){
		if(set_columns){
			columns = i.size();
			set_columns = false;
		}
		else if(i.size() != columns)
			throw MatrixException("Invalid initializer list: rows' lengths do not match.");
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
	rows = model.rows;
	columns = model.columns;
	init(false);
	for(ui i=0; i!=rows*columns; ++i)
		data[i] = model.data[i];
	return *this;
}

//************************************************************

double& Matrix::at(ui r, ui c){
	if( r >= rows || c >= columns )
		throw MatrixException("Indices out of range.");
	return data[c + columns*r];
}

const double& Matrix::at(ui r, ui c) const{
	if( r >= rows || c >= columns )
		throw MatrixException("Indices out of range.");
	return data[c + columns*r];
}

//************************************************************

Matrix Matrix::operator+(const Matrix &right) const{
	if(rows != right.rows || columns != right.columns)
		throw MatrixException("Operator '+' not permitted: dimensions do not match.");
	
	Matrix result(rows, columns);
	for(ui i=0; i!=rows*columns; ++i)
		result.data[i] = data[i] + right.data[i];
	return result;
}

//************************************************************

Matrix Matrix::operator-(const Matrix &right) const{
	if(rows != right.rows || columns != right.columns)
		throw MatrixException("Operator '-' not permitted: dimensions do not match.");
	
	Matrix result(rows, columns);
	for(ui i=0; i!=rows*columns; ++i)
		result.data[i] = data[i] - right.data[i];
	return result;
}

//************************************************************

Matrix Matrix::operator*(const Matrix &right) const{
	if(columns != right.rows)
		throw MatrixException("Operator '*' not permitted: dimensions do not match.");
	
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

Matrix Matrix::transpose() const
{
	//Matrix[x, y] == Transpose[y, x]
	Matrix result(columns, rows);

	for(ui i = 0; i != rows; ++i)
		for(ui j = 0; j != columns; ++j)
			result.at(j, i) = at(i, j);

	return result;
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
	if(data)
		free();

	if(rows*columns == 0)
		throw MatrixException("Cannot create empty matrix.");
	data = new double[rows*columns];

	if(init_with_zeroes){
		// Initialize all fields with zeroes.
		for(ui i = 0; i != rows*columns; ++i)
			data[i] = 0.0;
	}
}

// Free memory and reset all fields.
void Matrix::free(){
	if(data != nullptr)
		delete[] data;
	data = nullptr;
	rows = columns = 0;
}

//************************************************************
