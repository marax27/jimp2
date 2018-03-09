#ifndef MATRIX_H
#define MATRIX_H

#include <initializer_list>
#include <exception>
#include <string>

//************************************************************

class Matrix
{
	typedef unsigned int ui;

public:
	Matrix(ui x, ui y);
	Matrix(const Matrix &model){ *this = model; }
	Matrix(const std::initializer_list<std::initializer_list<double>> &values);
	~Matrix(){ free(); }

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

	ui getColumns() const { return columns; }
	ui getRows() const { return rows; }

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
	ui rows, columns;
	double *data;

	void init(bool init_with_zeroes=true);
	void free();
};

//************************************************************
#endif  //MATRIX_H