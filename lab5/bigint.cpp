#include "bigint.hpp"
#include <cstring>
#include <sstream>
#include <iostream>
#include <ctime>
#include <cstdlib>

#define DEBUG

//*** Input & output *****************************************

std::ostream& operator<<(std::ostream &out, const BigInt &right){
	out << right.asString();
	return out;
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
	if(length > right.length)
		return false;
	else if(length < right.length)
		return true;

	if(sign > right.sign)
		return true;
	else if(sign < right.sign)
		return false;
	
	// What is known:
	// a,b > 0 || a,b < 0
	// length(a) == length(b)
	bool equality = true;
	for(std::size_t i = 0; i != length; ++i){
		if(sign){
			if(data[i] < right.data[i])
				return false;
			else
				equality &= (data[i] == right.data[i]);
		}
		else{
			if(data[i] > right.data[i])
				return false;
			else
				equality &= (data[i] == right.data[i]);
		}
	}
	return true && !equality;
}

//************************************************************

BigInt BigInt::operator+(const BigInt &right) const{
	if(sign ^ right.sign)
		return sign ? (right - (-*this)) : (*this - (-right));
	
	// Compute a + b, where a*b > 0.

	const BigInt *max_n, *min_n;
	if(*this >= right){
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
	if(*this >= right){
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

	// std::cerr << "op-(" << *this << ',' << right << ')';
	// for(std::size_t i=0; i!=max_n->length; ++i)
		// std::cerr << '[' << (int)tab[i] << ']';
	// std::cerr << '\n';

	delete[] above;
	bool s;
	if(sign)
		s = (right <= *this) ? false : true;
	else
		s = (*this >= right) ? false : true;
	BigInt result(tab, s, i);
	return result;
}

//************************************************************

BigInt BigInt::operator-() const{
	BigInt result(*this);
	if(result != 0)
		result.sign = !sign;
	return result;
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
	Digit *p = data + length;
	while(*(p--) == 0)
		--length;
}

void BigInt::free(){
	delete[] data;
	length = 0;
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

TEST_CASE("Addition", "operator+"){
	REQUIRE( BigInt(30) + BigInt(1) == 31 );
	REQUIRE( BigInt(-30) + BigInt(1) == -29 );
	REQUIRE( BigInt(30) + BigInt(-1) == 29 );
	REQUIRE( BigInt(-30) + BigInt(-1) == -31 );
}

#else

int main(){
	srand(time(NULL));

	BigInt a(42000), a1("42000"),
	       b(-7200), b1("-7200"),
		   c(0),     c1("0");
	
	/*std::cout << "a: " << a << " | " << a1 << '\n'
	          << "b: " << b << " | " << b1 << '\n'
	          << "c: " << c << " | " << c1 << '\n';*/
	
	const int RANGE = 500000;
	const int ITERATIONS = 1000000;
	for(int i=0; i<ITERATIONS; ++i){
		int a = rand() % (2*RANGE) - RANGE;
		int b = rand() % (2*RANGE) - RANGE;
		
		BigInt p = BigInt(a)+BigInt(b);
		BigInt m = BigInt(a)-BigInt(b);
		if(p != a+b){
			std::cerr << "Err: "<<a<<" + "<<b<<" != "<< p << '\n';
			break;
		}
		if(m != a-b){
			std::cerr << "Err: "<<a<<" - "<<b<<" != "<< m << '\n';
			break;
		}

		if(i % 100 == 0){
			std::cout << "\rProgress: " << (i*100.0f/ITERATIONS) << "%         ";
		}
	}
	std::cout << "\nDone.\n";
}

#endif

