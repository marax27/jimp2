#ifndef MATRIX_3X3_HPP
#define MATRIX_3X3_HPP

#include <initializer_list>
#include <iostream>

//************************************************************

class Matrix
{
	typedef unsigned int ui;

public:
	Matrix(){ init(); }
	Matrix(const Matrix &model){ *this = model; }
	Matrix(const std::initializer_list<std::initializer_list<double>> &values);

	// Element access.
	// Convention: matrix.at(number_of_row, number_of_column)
	// Note: row and column numbering starts from 0.
	double& at(ui r, ui c);
	const double& at(ui r, ui c) const;

	// Copy a matrix.
	Matrix& operator=(const Matrix &model);	

	// Basic matrix operations.
	Matrix operator+(const Matrix &right) const;
	Matrix operator-(const Matrix &right) const;
	Matrix operator*(const Matrix &right) const;
	Matrix operator*(double right) const;

	// Transposition.
	Matrix transpose();

	ui getColumns() const { return SIZE; }
	ui getRows() const { return SIZE; }

	static Matrix identity();

	//--------------------
	class MatrixException : public std::exception {
		std::string message;
	public:
		MatrixException(std::string msg="Error")
			: message(msg) {}
		virtual const char* what() const noexcept{
			return message.c_str();
		}
	};
	//--------------------

private:
	const static int SIZE = 3;
	double data[SIZE*SIZE];

	void init();
};

std::ostream& operator<<(std::ostream &out, const Matrix &m);

//************************************************************
#endif  //MATRIX_3X3_HPP