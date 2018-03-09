#include "matrix.hpp"

#include <iostream>
using std::cout;

void printMatrix(const Matrix &m){
	for(unsigned y=0; y!=m.getRows(); ++y){
		for(unsigned x=0; x!=m.getColumns(); ++x)
			cout << m.at(y, x) << '\t';
		cout << '\n';
	}
}

int main(){
	Matrix A({{3, 2, 1}, {5, 5, 5}, {1, 0, 1}});
	Matrix B({{1, 0, 0}, {0, 1, 0}, {0, 0, 1}});

	Matrix AB = A*B,
	       BA = B*A;
	
	
	printMatrix(A); cout << '\n';
	printMatrix(A.transpose()); cout << '\n';
}


//************************************************************

Matrix::Matrix(const std::initializer_list<std::initializer_list<double>> &values){
	//values == {{row #1}, {row #2}, ..., {row #n}}
	rows = values.size();
	if(!rows){
		columns = 0;
		data = nullptr;
	}
	else{
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
}

//************************************************************

Matrix::Matrix(ui rows, ui columns)
	: rows(rows), columns(columns), data(nullptr){
	init();
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

// Allocate and initialize data memory.
void Matrix::init(bool init_with_zeroes){
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
			#warning Poprawne?
			//double &elem = result.at(y, x) = 0.0;
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

Matrix Matrix::transpose()
{
	//Matrix[x, y] == Transpose[y, x]
	Matrix result(rows, columns);

	for(ui i = 0; i != rows; ++i)
		for(ui j = 0; j != columns; ++j)
			result.at(i, j) = at(j, i);

	return result;
}

//************************************************************
