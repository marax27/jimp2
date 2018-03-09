#include <iostream>
#include <cmath>

using std::cout;

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
};

/* Rownowazne metodom powyzej, ale wymagaja nieco wiecej kodu.
Vector3 operator+(const Vector3 &left, const Vector3 &right);
Vector3 operator-(const Vector3 &left, const Vector3 &right);
Vector3 operator*(const Vector3 &left, double right);
Vector3 operator/(const Vector3 &left, double right);*/

//#define EXACT_COMPARE
bool operator==(const Vector3 &left, const Vector3 &right);

Vector3 operator*(double left, const Vector3 &right){
	return right * left;
}

// Wypisywanie wektora na ekran.
std::ostream& operator<<(std::ostream &output, const Vector3 &v){
	output << '[' << v.x << ", " << v.y << ", " << v.z << ']';
	return output;
}

//************************************************************

int main(){
	Vector3 a{2, 3, 4}, b{1, 1, 4}, c;

	cout << "a:\t" << a << '\n'
	     << "b:\t" << b << '\n'
	     << "c:\t" << c << '\n'
	     << "----------\n"
	     << "a+b:\t" << a+b << '\n'
	     << "a-b:\t" << a-b << '\n'
	     << "5a:\t" << 5*a << " == " << a*5 << '\n'
		 << "a/2:\t" << a / 2 << '\n'
		 << "a dot b: " << Vector3::dot(a, b) << '\n'
		 << "a x b:\t" << Vector3::cross(a, b) << '\n';
}

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
