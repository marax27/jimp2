#pragma once

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

	BigInt& operator=(const BigInt &model);

	bool operator==(const BigInt &right) const;
	bool operator!=(const BigInt &right) const{
		return !(*this == right);
	}

	bool operator<(const BigInt &right) const;

	bool operator>(const BigInt &right) const{
		return !(*this < right) && (*this != right);
	}
	bool operator<=(const BigInt &right) const{
		return (*this < right) || (*this == right);
	}
	bool operator>=(const BigInt &right) const{
		return !(*this < right);
	}

	BigInt operator+(const BigInt &right) const;
	BigInt operator-(const BigInt &right) const;

	BigInt operator-() const;
	BigInt operator+() const{
		return *this;
	}

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
	//----------------------------------------

private:
	std::size_t length;  //number of digits
	bool sign;           //true if number is negative
	Digit *data;

	void createFromString(const std::string &s);
	void allocateDigits(std::size_t digits);
	void trimZeroes();  // remove most significant 0-digits.
	void free();

	// Private constructor; takes ownership over *data.
	BigInt(Digit *data, bool sign, std::size_t length)
		: length(length), sign(sign), data(data){
			trimZeroes();
		}
};