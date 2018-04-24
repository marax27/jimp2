#include <cstring>
#include <sstream>
#include <iostream>
#include <string>
#include <cstddef>  //std::size_t
#include <exception>  //std::exception

class BigInt{
public:
	typedef char Digit;

	BigInt(long value);
	BigInt(const BigInt &model);

	// std::string is needed because of ambiguity of BigInt(0);
	// it maches BigInt(long) AND BigInt(const char*).
	explicit BigInt(const std::string &value);
	~BigInt();

	// Assignment.
	BigInt& operator=(const BigInt &model);

	// Comparing 2 BigInts.
	bool operator==(const BigInt &right) const;
	bool operator<(const BigInt &right) const;

	bool operator!=(const BigInt &right) const{
		return !(*this == right);
	}
	bool operator>(const BigInt &right) const{
		return !(*this < right) && (*this != right);
	}
	bool operator<=(const BigInt &right) const{
		return (*this < right) || (*this == right);
	}
	bool operator>=(const BigInt &right) const{
		return !(*this < right);
	}

	// Basic operators.
	BigInt operator+(const BigInt &right) const;
	BigInt operator-(const BigInt &right) const;
	BigInt operator*(const BigInt &right) const;
	BigInt operator/(const BigInt &right) const;
	BigInt operator%(const BigInt &right) const;

	BigInt operator-() const;
	BigInt operator+() const{
		return *this;
	}

	// Pre-
	BigInt& operator++(){
		*this = *this + 1;
		return *this;
	}
	BigInt& operator--(){
		*this = *this - 1;
		return *this;
	}

	// Post-
	BigInt operator++(int){
		BigInt copy(*this);
		++(*this);
		return copy;
	}
	BigInt operator--(int){
		BigInt copy(*this);
		--(*this);
		return copy;
	}

	// Assignment+ operators.
	BigInt& operator+=(const BigInt &right){
		*this = *this + right;
		return *this;
	}
	BigInt& operator-=(const BigInt &right){
		*this = *this - right;
		return *this;
	}
	BigInt& operator*=(const BigInt &right){
		*this = *this * right;
		return *this;
	}
	BigInt& operator/=(const BigInt &right){
		*this = *this / right;
		return *this;
	}
	BigInt& operator%=(const BigInt &right){
		*this = *this % right;
		return *this;
	}

	// Data access.
	Digit& operator[](std::size_t index){
		if(index <= length)
			return data[index];
		else
			throw OutOfRangeException();
	}
	const Digit& operator[](std::size_t index) const {
		if(index <= length)
			return data[index];
		else
			throw OutOfRangeException();
	}

	std::string asString() const;

	BigInt abs() const;

	//----------------------------------------
	class BigIntException : public std::exception {};

	class AllocationException : public BigIntException {};
	class ConstructionException : public BigIntException {};
	class OutOfRangeException : public BigIntException {};
	class ZeroDivisionException : public BigIntException {};
	//----------------------------------------

private:
	std::size_t length;  //number of digits
	bool sign;           //true if number is negative
	Digit *data;

	void createFromString(const std::string &s);
	void allocateDigits(std::size_t digits);
	void trimZeroes();  // remove most significant 0-digits.
	void free();
	void ensurePositiveZero();  //handle -0

	// Integer division - algorithm guesses the result.
	// Limited to 1-digit results.
	// Algorithm assumes treats numbers as non-negative.
	static Digit divint(BigInt a, BigInt b);

	// Private constructor; takes ownership over *data.
	BigInt(Digit *data, bool sign, std::size_t length)
		: length(length), sign(sign), data(data){
			trimZeroes();
		}
};

BigInt operator+(long left, const BigInt &right);
BigInt operator-(long left, const BigInt &right);
BigInt operator*(long left, const BigInt &right);
BigInt operator/(long left, const BigInt &right);

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

BigInt BigInt::operator*(const BigInt &right) const{
	if(*this == 0 || right == 0)
		return BigInt(0);
	
	const BigInt *greater = (abs() > right.abs() ? this : &right);
	const BigInt *less = (abs() <= right.abs() ? this : &right);
	std::size_t len = length + right.length;

	char *result_data = new char[len]();
	char *mid_data = new char[len * less->length]();
	char sh = 0;

	for(std::size_t i = 0; i != less->length; ++i){
		for(std::size_t j = 0; j != greater->length; ++j){
			std::size_t idx = j+i + i*len;
			mid_data[idx] = greater->data[j] * less->data[i] + sh;
			if(mid_data[idx] > 9){
				char v = mid_data[idx] % 10;
				sh = (mid_data[idx] - v) / 10;
				mid_data[idx] = v;
			}
			else //if(j+1 != greater->length)
				sh = 0;
		}
		mid_data[greater->length+i + i*len] = sh;
		sh = 0;
	}

	for(std::size_t i = 0; i != len; ++i){
		result_data[i] = sh;
		sh = 0;
		for(std::size_t j = 0; j != less->length; ++j)
			result_data[i] += mid_data[i + j*len];

		if(result_data[i] > 9){
			char v = result_data[i] % 10;
			sh = (result_data[i] - v) / 10;
			result_data[i] = v;
		}
	}

	BigInt result(result_data, sign ^ right.sign, length + right.length);
	result.trimZeroes();
	delete[] mid_data;
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

BigInt operator+(long left, const BigInt &right){
	return right + left;
}

BigInt operator-(long left, const BigInt &right){
	return -(right - left);
}

BigInt operator*(long left, const BigInt &right){
	return right * left;
}

BigInt operator/(long left, const BigInt &right){
	return BigInt(left) / right;
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

	ensurePositiveZero();
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

//************************************************************

int main(){
	std::cout << "Interactive calculator\n"
	          << "Usage: 'NUMBER [+-*/%] NUMBER'\n\n";

	while(true){
		BigInt a(0), b(0);
		char operation;
		std::cout << ">>> ";
		// Read in format "a op b". Spaces are required.
		std::cin >> a >> operation >> b;

		BigInt p(0);

		switch(operation){
		case '+':
			p = a+b;
			break;
		case '-':
			p = a-b;
			break;
		case '*':
			p = a*b;
			break;
		case '/':
			try{ p = a/b; }
			catch(BigInt::BigIntException&){
				std::cerr << "[Failed]\n";
				continue;
			}
			break;
		case '%':
			try{ p = a%b; }
			catch(BigInt::BigIntException&){
				std::cerr << "[Failed]\n";
				continue;
			}
			break;
		default:
			std::cerr << "[Unknown operation]\n";
		}
		std::cout << a << ' ' << operation << ' ' << b << " = " << p << '\n';
	}
}
