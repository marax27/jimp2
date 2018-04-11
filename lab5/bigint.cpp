#include "bigint.hpp"
#include <cstring>
#include <sstream>
#include <iostream>
#include <ctime>
#include <cstdlib>

// #define DEBUG

//*** Input & output *****************************************

std::ostream& operator<<(std::ostream &out, const BigInt &right){
	out << right.asString();
	return out;
}

std::istream& operator>>(std::istream &in, BigInt &right){
	std::string s;
	in >> s;
	right = BigInt(s);
	return in;
}

//*** Constructors & destructor ******************************

BigInt::BigInt(long value){
	std::stringstream ss;
	ss << value;
	createFromString(ss.str());
}

BigInt::BigInt(const BigInt &model) : length(model.length), sign(model.sign){
	allocateDigits(length);
	memcpy(data, model.data, sizeof(Digit)*length);
}

BigInt::BigInt(const std::string &value){
	createFromString(value);
	trimZeroes();
}

BigInt::~BigInt(){
	free();
}

//*** Operators **********************************************

BigInt& BigInt::operator=(const BigInt &model){
	if(data)
		free();
	length = model.length;
	sign = model.sign;
	allocateDigits(length);
	memcpy(data, model.data, sizeof(Digit)*length);
	return *this;
}

bool BigInt::operator==(const BigInt &right) const{
	if(sign != right.sign || length != right.length)
		return false;
	
	for(std::size_t i = 0; i != length; ++i){
		if(data[i] != right.data[i])
			return false;
	}
	return true;
}

bool BigInt::operator<(const BigInt &right) const{
	if(sign > right.sign)
		return true;
	else if(sign < right.sign)
		return false;

	if(length > right.length)
		return sign ? true : false;
	else if(length < right.length)
		return sign ? false : true;

	// What is known:
	// a,b > 0 || a,b < 0
	// length(a) == length(b)
	for(std::size_t i = 0; i != length; ++i){
		auto d1 = data[length-i-1];
		auto d2 = right.data[length-i-1];
		if(sign){
			if(d1 < d2)
				return false;
			else if(d1 > d2)
				return true;
		}
		else{
			if(d1 > d2)
				return false;
			else if(d1 < d2)
				return true;
		}
	}
	return false;  //they're equal
}

//************************************************************

BigInt BigInt::operator+(const BigInt &right) const{
	if(sign ^ right.sign)
		return sign ? (right - (-*this)) : (*this - (-right));
	
	// Compute a + b, where a*b > 0.

	const BigInt *max_n, *min_n;
	if(this->abs() >= right.abs()){
		max_n = this;
		min_n = &right;
	}
	else{
		max_n = &right;
		min_n = this;
	}
	// max_n >= min_n => max_length >= min_length
	Digit *tab = new Digit[max_n->length+1];
	int8_t sh = 0;
	std::size_t i = 0;

	while(i < max_n->length){
		tab[i] = sh + max_n->data[i];
		if(i < min_n->length)
			tab[i] += min_n->data[i];

		if(tab[i] > 9){
			tab[i] -= 10;
			sh = 1;
		}
		else
			sh = 0;
		++i;
	}
	if(sh)
		tab[i++] = 1;

	BigInt result(tab, sign, i);
	return result;
}

//************************************************************

BigInt BigInt::operator-(const BigInt &right) const{
	if(sign ^ right.sign)
		return sign ? (*this + (-right)) : (*this + (-right));

	// Compute a - b, where a*b > 0.
	const BigInt *max_n, *min_n;
	if(this->abs() >= right.abs()){
		max_n = this;
		min_n = &right;
	}
	else{
		max_n = &right;
		min_n = this;
	}

	// max_n >= min_n => max_length >= min_length
	Digit *tab = new Digit[max_n->length];
	std::size_t i = 0;

	int8_t *above = new int8_t[max_n->length];
	for(std::size_t k = 0; k != max_n->length; ++k)
		above[k] = max_n->data[k];

	while(i < min_n->length){
		if(above[i] >= min_n->data[i])
			tab[i] = above[i] - min_n->data[i];
		else{
			above[i] += 10;
			std::size_t k = i+1;
			while(!above[k])
				above[k++] += 9;
			--above[k];

			tab[i] = above[i] - min_n->data[i];
		}
		++i;
	}
	while(i < max_n->length){
		tab[i] = above[i];
		++i;
	}

	delete[] above;
	bool s;
	if(sign)
		s = (*this <= right) ? true : false;
	else
		s = (*this >= right) ? false : true;
	BigInt result(tab, s, i);
	return result;
}

//************************************************************

// Naive implementation.
BigInt BigInt::operator*(const BigInt &right) const{
	BigInt result(0);
	if(*this == 0 || right == 0)
		return result;
		
	BigInt rabs = right.abs();
	for(BigInt I = 0; I != rabs; I += 1)
		result += *this;
	if(result != 0)
		result.sign = (sign ^ right.sign);
	return result;
}

//************************************************************

BigInt BigInt::operator/(const BigInt &right) const{
	BigInt result = *this;
	BigInt x = 0;

	for(std::size_t i = this->length; i > 0; --i){
		x = 10*x + this->data[i-1];
		Digit d = divint(x, right);
		result[i-1] = d;
		x -= right.abs()*d;
	}

	result.trimZeroes();
	result.sign = (sign ^ right.sign);
	result.ensurePositiveZero();
	return result;
}

//************************************************************

BigInt BigInt::operator%(const BigInt &right) const{
	// Note: a % b == -[(-a) % (-b)]
	if(right == 0)
		throw ZeroDivisionException();
	
	BigInt a = *this;
	BigInt b = right.abs();
	bool flag = false;

	if(right.sign){
		// a % (-b) == -[(-a) % b]
		a.sign = !a.sign;
		b.sign = false;
		flag = true;
	}
	
	if(a.sign){
		while(a < 0)
			a += b;
	}
	else{
		while(a >= b)
			a -= b;
	}

	a.sign = flag;
	return a;
}

//************************************************************

BigInt BigInt::operator-() const{
	BigInt result(*this);
	if(result != 0)
		result.sign = !sign;
	return result;
}

//************************************************************

BigInt operator*(long left, const BigInt &right){
	return right * left;
}

//*** Public methods *****************************************

std::string BigInt::asString() const{
	const char OFFSET = '0' - 0;
	std::stringstream result;
	if(sign)
		result << '-';
	for(std::size_t i = length; i > 0; --i)
		result << (char)(data[i-1] + OFFSET);
	return result.str();
}

BigInt BigInt::abs() const{
	BigInt result(*this);
	result.sign = 0;
	return result;
}

//*** Private methods ****************************************

void BigInt::createFromString(const std::string &s){
	// Set length, sign and allocate memory.
	if(s[0] == '-'){
		length = s.length() - 1;
		sign = true;
	}
	else{
		length = s.length();
		sign = false;
	}
	allocateDigits(length);

	// Copy digits IN REVERSED ORDER.
	Digit *p = data + length - 1;
	for(auto i : ((s[0] == '-') ? s.substr(1) : s)){
		Digit d = i - 48;
		if(d >= 0 && d <= 9)
			*(p--) = d;
		else
			throw ConstructionException();
	}
}

void BigInt::allocateDigits(std::size_t digits){
	try{
		data = new Digit[digits];
		length = digits;
	}catch(std::bad_alloc){
		throw AllocationException();
	}
}

void BigInt::trimZeroes(){
	Digit *p = data + length - 1;
	while(*p == 0 && p != data){
		--length;
		--p;
	}
}

void BigInt::free(){
	delete[] data;
	length = 0;
}

void BigInt::ensurePositiveZero(){
	if(length == 1 && sign && data[0] == 0)
		sign = false;
}

BigInt::Digit BigInt::divint(BigInt a, BigInt b){
	if(b == 0)
		throw ZeroDivisionException();

	a = a.abs();
	b = b.abs();
	if(a < b)
		return 0;
	else if(a == b)
		return 1;
	Digit result = 0;
	do{
		a -= b;
		++result;
	}while(a >= b);
	return result;
}

//*** Unit testing *******************************************

#ifdef DEBUG

#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

TEST_CASE("Comparing BigInt objects", "BigInt::operator=="){
	// REQUIRE( BigInt() == 0 );

	REQUIRE( BigInt(12345) == 12345 );
	REQUIRE( BigInt(-1) == -1 );
	REQUIRE( BigInt(0) == 0 );

	REQUIRE( BigInt(321) != 999 );
	REQUIRE( BigInt(-123) != 123 );
	REQUIRE( BigInt(-50000) != -49999 );
}

TEST_CASE("Test copy constructor", "BigInt(const BigInt&)"){
	BigInt a("-1234567890123456789012345678901234567890");
	BigInt b(a);
	REQUIRE( a == b );
}

TEST_CASE("Test assignment operator", "operator="){
	BigInt a("-1234567890123456789012345678901234567890");
	BigInt b(666);
	b = a;
	REQUIRE( a == b );
}

TEST_CASE("Relation a > b", "operator>"){
	REQUIRE( BigInt(1234) > BigInt(99) );
	REQUIRE( BigInt(1234) > BigInt(1099) );
	REQUIRE( BigInt(100) > BigInt(-5421) );
	REQUIRE( BigInt(-100) > BigInt(-125) );
}

TEST_CASE("Relation a < b", "operator<"){
	REQUIRE( BigInt(99) < BigInt(1234) );
	REQUIRE( BigInt(1099) < BigInt(1234) );
	REQUIRE( BigInt(-100) < BigInt(12) );
	REQUIRE( BigInt(-888) < BigInt(-4) );
}

TEST_CASE("Addition", "operator+"){
	REQUIRE( BigInt(30) + BigInt(1) == 31 );
	REQUIRE( BigInt(-30) + BigInt(1) == -29 );
	REQUIRE( BigInt(30) + BigInt(-1) == 29 );
	REQUIRE( BigInt(-30) + BigInt(-1) == -31 );
}

TEST_CASE("Substraction", "operator-"){
	REQUIRE( BigInt(99) - BigInt(9) == BigInt(90) );
	REQUIRE( BigInt(-99) - BigInt(9) == BigInt(-108) );
	REQUIRE( BigInt(99) - BigInt(-9) == BigInt(108) );
	REQUIRE( BigInt(-99) - BigInt(-9) == BigInt(-90) );
}

TEST_CASE("Multiplication", "operator*"){
	BigInt a(12345), b(7099), c(12345*7099);
	REQUIRE( a*b == c );
	REQUIRE( b*(-a) == -c );
	REQUIRE( (-a)*(-b) == c );
	REQUIRE( (-a)*b == -c );
}

TEST_CASE("Division", "operator/"){
	REQUIRE( BigInt(256) / BigInt(4) == 64 );
	REQUIRE( BigInt(-256) / BigInt(4) == -64 );
	REQUIRE( BigInt(256) / BigInt(-4) == -64 );
	REQUIRE( BigInt(-256) / BigInt(-4) == 64 );
}

TEST_CASE("Modulus", "operator%"){
	REQUIRE( BigInt(17) % BigInt(8) == 1 );
	REQUIRE( BigInt(-17) % BigInt(8) == 7 );
	REQUIRE( BigInt(17) % BigInt(-8) == -7 );
	REQUIRE( BigInt(-17) % BigInt(-8) == -1 );
}


/*TEST_CASE("divint", "divint()"){
	REQUIRE( BigInt::divint(36, 6) == 6 );
	REQUIRE( BigInt::divint(37, 6) == 6 );
	REQUIRE( BigInt::divint(41, 6) == 6 );
	REQUIRE( BigInt::divint(42, 6) == 7 );

	REQUIRE( BigInt::divint(13, 13) == 1 );
	REQUIRE( BigInt::divint(8, 13) == 0 );
	REQUIRE( BigInt::divint(0, 5) == 0 );
	
	REQUIRE_THROWS_AS( BigInt::divint(1, 0), BigInt::ZeroDivisionException );
	REQUIRE_THROWS_AS( BigInt::divint(0, 0), BigInt::ZeroDivisionException );
}*/

#else

int main(){
	srand(time(NULL));

	/*while(true){
		BigInt a(0), b(0);
		char operation;
		std::cout << ">>> ";
		std::cin >> a >> operation >> b;

		switch(operation){
		case '+':
			std::cout << a << " + " << b << " = " << a+b;
			break;
		case '-':
			std::cout << a << " - " << b << " = " << a-b;
			break;
		case '*':
			std::cout << a << " * " << b << " = " << a*b;
			break;
		case '/':
			std::cout << a << " / " << b << " = " << a/b << "(mod " << a%b << ')';
			break;
		case '%':
			std::cout << a << " % " << b << " = " << a%b;
			break;
		default:
			std::cout << "[Error]";
		}
		std::cout << std::endl;
	}*/
	
	const int RANGE = 5000;
	const int ITERATIONS = 1000000;
	for(int i=0; i<ITERATIONS; ++i){
		int a = rand() % (2*RANGE) - RANGE;
		int b = rand() % (2*RANGE) - RANGE;
		
		/*BigInt p = BigInt(a)+BigInt(b);
		BigInt m = BigInt(a)-BigInt(b);
		BigInt d = b ? BigInt(a)/BigInt(b) : 0;

		if(p != a+b){
			std::cerr << "Err: "<<a<<" + "<<b<<" != "<< p << '\n';
			break;
		}
		if(m != a-b){
			std::cerr << "Err: "<<a<<" - "<<b<<" != "<< m << '\n';
			break;
		}
		if(b && d != a/b){
			std::cerr << "Err: "<<a<<" / "<<b<<" != "<< d << '\n';
			break;
		}*/

		BigInt m = BigInt(a)*BigInt(b);
		if(m != a*b){
			std::cerr << "Err: "<<a<<" * "<<b<<" != "<< m << '\n';
			break;
		}

		if(i % 100 == 0){
			std::cout << "\rProgress: " << (i*100.0f/ITERATIONS) << "%         ";
		}
	}
	std::cout << "\nDone.\n";
}

#endif

