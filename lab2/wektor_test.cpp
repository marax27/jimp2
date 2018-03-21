#include <iostream>
#include <cmath>

using std::cout;

#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

//************************************************************

struct Vector3{
	//Vector3() : x(0), y(0), z(0) {}
	Vector3() = default;
	Vector3(double x, double y, double z) : x(x), y(y), z(z) {}

	double x = {0.0}, y = {0.0}, z = {0.0};

	double length() const {
		return sqrt(x*x + y*y + z*z);
	}

	Vector3 operator+(const Vector3 &right) const {
		return Vector3{x+right.x, y+right.y, z+right.z};
	}

	Vector3 operator-(const Vector3 &right) const {
		return Vector3{x-right.x, y-right.y, z-right.z};
	}

	Vector3 operator*(double right) const {
		return Vector3{x*right, y*right, z*right};
	}

	Vector3 operator/(double right) const {
		return *this * (1.0 / right);
	}

	static Vector3 cross(const Vector3 &left, const Vector3 &right);
	static double dot(const Vector3 &left, const Vector3 &right);

	Vector3 normalized() const {
		return *this / length();
	}

	void normalize(){
		*this = normalized();
	};
};

/* Rownowazne metodom powyzej, ale wymagaja nieco wiecej kodu.
Vector3 operator+(const Vector3 &left, const Vector3 &right);
Vector3 operator-(const Vector3 &left, const Vector3 &right);
Vector3 operator*(const Vector3 &left, double right);
Vector3 operator/(const Vector3 &left, double right); */

Vector3 inline operator*(double left, const Vector3 &right){
	return right * left;
}

//#define EXACT_COMPARE
bool operator==(const Vector3 &left, const Vector3 &right);
bool operator!=(const Vector3 &left, const Vector3 &right){
	return !(left == right);
}

// Wypisywanie wektora na ekran.
std::ostream& operator<<(std::ostream &output, const Vector3 &v);

//************************************************************

bool operator==(const Vector3 &left, const Vector3 &right){
	#ifdef EXACT_COMPARE
		return left.x == right.x &&
		       left.y == right.y &&
		       left.z == right.z;
	#else
		const double EPSILON = 10e-9;
		return abs(left.x - right.x) < EPSILON &&
		       abs(left.y - right.y) < EPSILON &&
		       abs(left.z - right.z) < EPSILON;
	#endif
}

//************************************************************

Vector3 Vector3::cross(const Vector3 &left, const Vector3 &right){
	return Vector3{
		left.y*right.z - left.z*right.y,
		left.z*right.x - left.x*right.z,
		left.x*right.y - left.y*right.x
	};
}

double Vector3::dot(const Vector3 &left, const Vector3 &right){
	return left.x*right.x + left.y*right.y + left.z*right.z;
}

//************************************************************

std::ostream& operator<<(std::ostream &output, const Vector3 &v){
	output << '[' << v.x << ", " << v.y << ", " << v.z << ']';
	return output;
}

//************************************************************

TEST_CASE("Vector initialization", "[Vector::Vector]"){
	Vector3 A, B{5, 10, 15};
	REQUIRE(A.x == 0);
	REQUIRE(A.y == 0);
	REQUIRE(A.z == 0);
	REQUIRE(B.x == 5);
	REQUIRE(B.y == 10);
	REQUIRE(B.z == 15);
}

TEST_CASE("Vector length/norm/magnitude", "[Vector::length]"){
	Vector3 A{2, 4, 4}, B{-2, 4, -4};

	REQUIRE(A.length() == Approx(6.0));
	REQUIRE(B.length() == Approx(6.0));
	REQUIRE(Vector3().length() == 0.0);
}

TEST_CASE("Vector / number", "[Vector::operator/]"){
	Vector3 A{6, 8, 1};
	REQUIRE(A/4 == Vector3{1.5, 2.0, 0.25});
}

TEST_CASE("Cross product", "[Vector::cross]"){
	Vector3 A{3, -3, 1}, B{4, 9, 2};
	REQUIRE(Vector3::cross(A, B) == Vector3{-15, -2, 39});
}

TEST_CASE("Dot product", "[Vector::dot]"){
	Vector3 A{3, -3, 1}, B{4, 9, 2};
	REQUIRE(Vector3::dot(A, B) == Approx(12 - 27 + 2));
}
