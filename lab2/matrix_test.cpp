#include "matrix.hpp"
#include <iostream>

#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

TEST_CASE("Generate matrices from initializer_list", "[Matrix(initializer_list)]"){
	Matrix A{{3, 2, 1}};
	Matrix B{{3}, {2}, {1}};
	REQUIRE(A.at(0, 0) == 3);
	REQUIRE(A.at(0, 1) == 2);
	REQUIRE(A.at(0, 2) == 1);
	REQUIRE(B.at(0, 0) == 3);
	REQUIRE(B.at(1, 0) == 2);
	REQUIRE(B.at(2, 0) == 1);
}

TEST_CASE("Initialize matrix of given dimensions", "[Matrix(rows, columns)]"){
	SECTION("Both dimensions positive."){
		Matrix A(21, 37);
		REQUIRE(A.getRows() == 21);
		REQUIRE(A.getColumns() == 37);
	}
	SECTION("At least one dimension is zero"){
		REQUIRE_THROWS( Matrix(7, 0) );
		REQUIRE_THROWS( Matrix(0, 7) );
		REQUIRE_THROWS( Matrix(0, 0) );
	}
	SECTION("Trying to allocate too much memory"){
		REQUIRE_THROWS( Matrix(999999, 999999) );
	}
}

TEST_CASE("Element access", "[at]"){
	Matrix A{{1, 2, 3}, {4, 5, 6}};
	// | 1 2 3 |
	// | 4 5 6 |
	REQUIRE( A.at(1, 2) == Approx(6.0) );
	REQUIRE_THROWS( A.at(2, 0) );
}

TEST_CASE("Matrix assignment", "operator="){
	Matrix M{{0, 1, 2, 3}, {5, 5, 5, 5}};
	Matrix A{{1, 2, 3}};
	A = M;
	
	REQUIRE( A.getColumns() == M.getColumns() );
	REQUIRE( A.getRows() == M.getRows() );
	REQUIRE( A.at(0,0) == M.at(0,0) );
	REQUIRE( A.at(1,2) == M.at(1,2) );
}

TEST_CASE("Sum", "[operator+]"){
	Matrix A{{0, 2}, {3, 4}}, B{{3, 1}, {0, -1}};
	REQUIRE( A+B == Matrix{{3, 3}, {3, 3}} );
}

TEST_CASE("Substraction", "[operator-]"){
	Matrix A{{1, 2}, {3, 4}}, B{{2, 1}, {5, 3}};
	REQUIRE(A-A == Matrix(2, 2));
	REQUIRE(A-B != Matrix(2, 2));
}

TEST_CASE("Multiplication", "[operator*]"){
	Matrix A{{1, 2}, {3, 4}}, I{{1, 0}, {0, 1}};
	SECTION("Matrix * number"){
		REQUIRE(A+A+A == 3*A);
		REQUIRE(-5*A == -1*A*5);
	}
	SECTION("Matrix * matrix"){
		REQUIRE(A*I == I*A);
		REQUIRE(A*I == A);
		
		Matrix C{{7, 6, 5}, {1, 2, 3}};
		Matrix mul{{9, 10, 11}, {25, 26, 27}};
		REQUIRE_THROWS(C*A);
		REQUIRE(A*C == mul);
	}
}

TEST_CASE("Division", "[operator/]"){
	Matrix A{{4, 6}, {2, 2}}, B{{2, 3}, {1, 1}};
	REQUIRE(A/2 == B);
	REQUIRE(0.5*A == A/2);
	REQUIRE(A*0.5 == A/2);
}

TEST_CASE("Transpose", "[transpose]"){
	Matrix A{{1, 2, 3, 4}, {5, 6, 7, 8}};
	Matrix T{{1, 5}, {2, 6}, {3, 7}, {4, 8}};
	REQUIRE(A.transpose() == T);
	REQUIRE(T.transpose() == A);
}

TEST_CASE("Identity matrix", "[identity]"){
	Matrix I = Matrix::identity(4);
	Matrix A{{5, 6, 7, 8}};
	REQUIRE(A*I == A);
	REQUIRE(Matrix::identity(1) == Matrix{{1}});
	REQUIRE_THROWS(Matrix::identity(0));
}

TEST_CASE("at", "[at]"){
	Matrix a{{0, 1}};
	const Matrix b{{7, 8}};
	
	REQUIRE((a.at(0,0) = 13));
	REQUIRE(a.at(0,0) == 13);

	REQUIRE(b.at(0,0) == 7);
}

TEST_CASE("boolean tests", "***"){
	Matrix I = Matrix::identity(5),
	       A = {{3, 0, 0}, {0, -2, 0}, {0, 0, 0.25}},
	       B = {{3, 0, 1}, {0, -2, 3}, {0, 0, 0.25}};

	REQUIRE(Matrix(21, 12).isZero());
	REQUIRE(!I.isZero());
	REQUIRE(!A.isZero());

	REQUIRE(I.isIdentity());
	REQUIRE(!A.isIdentity());

	REQUIRE(I.isSquare());
	REQUIRE(A.isSquare());
	REQUIRE(!Matrix(21, 12).isSquare());

	REQUIRE(I.isDiagonal());
	REQUIRE(A.isDiagonal());
	REQUIRE(!B.isDiagonal());
}