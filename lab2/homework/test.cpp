#include "matrix3x3.hpp"
#include <iostream>

using namespace std;

int main(){
	Matrix A, B{{1,2,3}, {4,0,5}, {6,7,8}};
	Matrix C(B), D(Matrix::identity());

	cout << A << '\n'
	     << B << '\n'
	     << C << '\n'
	     << D << '\n';
}