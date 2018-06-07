#ifndef _BIT_H_
#define _BIT_H_

#include <cstdint>

typedef uint8_t byte_t;

//************************************************************

class BitRef{
	byte_t *byte;
	uint8_t bit_index;
public:
	BitRef(byte_t *_byte, byte_t _bit_index)
		: byte(_byte), bit_index(_bit_index) {}
		
	BitRef& operator=(bool value){
		if(value)
			*byte |= 1 << bit_index;
		else
			*byte &= ~(1 << bit_index);
		return *this;
	}

	operator bool() const {
		return *byte & (1 << bit_index);
	}
};

//************************************************************

class ConstBitRef{
	const byte_t *byte;
	uint8_t bit_index;
public:
	ConstBitRef(const byte_t *_byte, uint8_t _bit_index)
		: byte(_byte), bit_index(_bit_index) {}
		
	operator bool() const {
		return *byte & (1 << bit_index);
	}
};

//************************************************************


#endif  //_BIT_H_
