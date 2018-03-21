#include <iostream>
#include <cmath>

using std::cout;

//************************************************************

//#define EXACT_COMPARE

class Vector3{

public:
	// Constructors.
	//Vector3() : x(0.0), y(0.0), z(0.0) {}
	Vector3() = default;
	Vector3(double x, double y, double z) : x(x), y(y), z(z) {}

	// Length of a vector.
	double length() const {
		return sqrt(x*x + y*y + z*z);
	}

	// Binary operators.
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

	// Comparison.
	bool operator==(const Vector3 &right) const;
	
	bool operator!=(const Vector3 &right) const {
		return !(*this == right);
	}

	// Unary operators.
	Vector3 operator-() const {
		return *this * -1.0;
	}

	Vector3 operator+() const {
		return *this;
	}

	// Static functions.
	static Vector3 cross(const Vector3 &left, const Vector3 &right);
	static double dot(const Vector3 &left, const Vector3 &right);

	// Normalization.
	Vector3 normalized() const {
		return *this / length();
	}

	void normalize(){
		*this = normalized();
	};

	// Getters.
	double X() const { return x; }
	double Y() const { return y; }
	double Z() const { return z; }

protected:
	double x = {0.0}, y = {0.0}, z = {0.0};
};

Vector3 inline operator*(double left, const Vector3 &right){
	return right * left;
}

// Wypisywanie wektora na ekran.
std::ostream& operator<<(std::ostream &output, const Vector3 &v);

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
	     << "a x b:\t" << Vector3::cross(a, b) << '\n'
	     << "----------\n"
	     << "norm(a): " << a.normalized() << " (length: " << a.normalized().length() << ")\n";
}

//************************************************************

bool Vector3::operator==(const Vector3 &right) const {
	#ifdef EXACT_COMPARE
		return left.x == right.x &&
		       left.y == right.y &&
		       left.z == right.z;
	#else
		const double EPSILON = 10e-9;
		return abs(x - right.x) < EPSILON &&
		       abs(y - right.y) < EPSILON &&
		       abs(z - right.z) < EPSILON;
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
	output << '[' << v.X() << ", " << v.Y() << ", " << v.Z() << ']';
	return output;
}

//************************************************************
