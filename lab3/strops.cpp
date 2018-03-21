/* String operations
 * Implementation of some popular string functions.
 */

#include <iostream>

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
