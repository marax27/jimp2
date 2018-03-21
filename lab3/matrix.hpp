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
	Matrix(const Matrix &model) : data(nullptr) { *this = model; }
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

std::ostream& operator<<(std::ostream &out, const Matrix &m);
Matrix operator*(double left, const Matrix &right);

#endif  //MATRIX_H