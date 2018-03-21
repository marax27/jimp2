#include "matrix3x3.hpp"

#include <iostream>
using std::cout;

void printMatrix(const Matrix &m){
	for(unsigned y=0; y!=m.getRows(); ++y){
		for(unsigned x=0; x!=m.getColumns(); ++x)
			cout << m.at(y, x) << '\t';
		cout << '\n';
	}
}

//************************************************************

Matrix::Matrix(const std::initializer_list<std::initializer_list<double>> &values){
	//values == {{row #1}, {row #2}, ..., {row #n}}
	if(values.size() != SIZE)
		throw MatrixException("Invalid initializer list: 3 rows required.");
	else{
		for(const auto &i : values){
			if(i.size() != SIZE)
				throw MatrixException("Invalid initializer list: 3 columns required.");
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

double& Matrix::at(ui r, ui c){
	if( r >= SIZE || c >= SIZE )
		throw MatrixException("Indices out of range.");
	return data[c + SIZE*r];
}

const double& Matrix::at(ui r, ui c) const{
	if( r >= SIZE || c >= SIZE )
		throw MatrixException("Indices out of range.");
	return data[c + SIZE*r];
}

//************************************************************

Matrix& Matrix::operator=(const Matrix &model){
	for(ui i=0; i!=SIZE*SIZE; ++i)
		data[i] = model.data[i];
	return *this;
}

//************************************************************

Matrix Matrix::operator+(const Matrix &right) const{
	Matrix result;
	for(ui i=0; i!=SIZE*SIZE; ++i)
		result.data[i] = data[i] + right.data[i];
	return result;
}

//************************************************************

Matrix Matrix::operator-(const Matrix &right) const{
	Matrix result;
	for(ui i=0; i!=SIZE*SIZE; ++i)
		result.data[i] = data[i] - right.data[i];
	return result;
}

//************************************************************

Matrix Matrix::operator*(const Matrix &right) const{
	Matrix result;
	for(ui y=0; y!=SIZE; ++y){
		for(ui x=0; x!=SIZE; ++x){
			double *elem = &result.at(y, x);
			*elem = 0.0;
			for(ui k=0; k!=SIZE; ++k)
				*elem += at(y, k) * right.at(k, x);
		}
	}
	return result;
}

//************************************************************

Matrix Matrix::operator*(double right) const{
	Matrix result;
	
	for(ui i=0; i!=SIZE*SIZE; ++i)
		result.data[i] = data[i] * right;
	return result;
}

//************************************************************

Matrix Matrix::transpose()
{
	//Matrix[x, y] == Transpose[y, x]
	Matrix result;

	for(ui i = 0; i != SIZE; ++i)
		for(ui j = 0; j != SIZE; ++j)
			result.at(i, j) = at(j, i);

	return result;
}

//************************************************************

Matrix Matrix::identity(){
	return Matrix{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
}

//************************************************************

// Initialize data memory.
void Matrix::init(){
	// Initialize all fields with zeroes.
	for(ui i = 0; i != SIZE*SIZE; ++i)
		data[i] = 0.0;
}

//************************************************************

std::ostream& operator<<(std::ostream &out, const Matrix &m){
	for(unsigned i=0; i != m.getRows(); ++i){
		for(unsigned j=0; j != m.getColumns(); ++j)
			out << m.at(i, j) << '\t';
		out << '\n';
	}
	return out;
}

//************************************************************