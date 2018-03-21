/* String operations
 * Implementation of some popular string functions.
 */

#include <iostream>

#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

//************************************************************

namespace jimp{

	bool strcmp(const char *s1, const char *s2){
		unsigned int i = 0;
		while(s1[i] == s2[i]){
			if(!s1[i++])
				return true;
		}
		return false;
	}

	unsigned strlen(const char *s){
		unsigned i = 0;
		while(s[i++]);
		return i-1;
	}

	char* strcpy(char *target, const char *source){
		// Assume allocated memory is large enough.
		unsigned i = 0;
		do{
			target[i] = source[i];
		}while(source[i++]);
		return target;
	}

	char* strcat(char *target, const char *source){
		// Assume allocated memory is large enough.
		unsigned from_where = jimp::strlen(target);
		unsigned i = 0;
		do{
			target[from_where + i] = source[i];
		}while(source[i++]);
		return target;
	}

	void* memcpy(void *target, const void *source, unsigned n){
		// Assume allocated memory is large enough.
		unsigned i = 0;
		while(i < n){
			reinterpret_cast<uint8_t*>(target)[i] = reinterpret_cast<const uint8_t*>(source)[i];
			++i;
		}
		return target;
	}

}//namespace jimp

//************************************************************

TEST_CASE("String comparison", "[strcmp]"){
	REQUIRE( jimp::strcmp("ASDFghjk", "ASDFghjk") );
	REQUIRE( jimp::strcmp("_{}+00", "_{}+00") );
	REQUIRE( jimp::strcmp("", "") );

	REQUIRE( !jimp::strcmp("qwerty", "qwertz") );
	REQUIRE( !jimp::strcmp("ASDF", "Asdf") );
	REQUIRE( !jimp::strcmp("xxx", "") );
}

TEST_CASE("Length of string", "[strlen]"){
	REQUIRE( jimp::strlen("abcdEFGH") == 8 );
	REQUIRE( jimp::strlen("X") == 1 );
	REQUIRE( jimp::strlen("") == 0 );
}

TEST_CASE("String copying", "[strcpy]"){
	char *a = new char[32];
	a[0] = '\0';
	const char *b = "abcd";
	REQUIRE( jimp::strcmp(jimp::strcpy(a, b), "abcd") );
	REQUIRE( jimp::strcmp(jimp::strcpy(a, b), "abcd") );
	REQUIRE( jimp::strcmp(jimp::strcpy(a, b), "abcd") );

	char *c = new char[16];
	c[0] = '\0';
	REQUIRE( jimp::strcmp(jimp::strcpy(c, ""), "") );
	REQUIRE( jimp::strcmp(jimp::strcpy(c, "aaaaaaaa"), "aaaaaaaa") );
	REQUIRE( jimp::strcmp(jimp::strcpy(c, "1111"), "1111") );

	delete[] a;
	delete[] c;
}

TEST_CASE("String concanectation", "[strcat]"){
	char *a = new char[32];
	a[0] = '\0';
	const char *b = "abcd";
	REQUIRE( jimp::strcmp(jimp::strcat(a, b), "abcd") );
	REQUIRE( jimp::strcmp(jimp::strcat(a, b), "abcdabcd") );
	REQUIRE( jimp::strcmp(jimp::strcat(a, b), "abcdabcdabcd") );

	char *c = new char[16];
	c[0] = '\0';
	REQUIRE( jimp::strcmp(jimp::strcat(c, ""), "") );
	REQUIRE( jimp::strcmp(jimp::strcat(c, "xyz"), "xyz") );

	delete[] a;
	delete[] c;
}

TEST_CASE("Memory copying", "[memcpy]"){
	SECTION("C-string copy"){
		char *d1 = new char[128];
		const char *s1 = "Test string.";
		jimp::memcpy(d1, s1, 13);

		REQUIRE( jimp::strcmp(d1, s1) );

		delete[] d1;
	}

	SECTION("Int array copy"){
		int *d2 = new int[4];
		int s2[4] = {432, 7, -123, 1720123123};
		jimp::memcpy(d2, s2, 4*sizeof(int));

		REQUIRE( s2[0] == d2[0] );
		REQUIRE( s2[1] == d2[1] );
		REQUIRE( s2[2] == d2[2] );
		REQUIRE( s2[3] == d2[3] );
	}
}

