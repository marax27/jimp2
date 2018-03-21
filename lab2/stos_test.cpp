#include <iostream>
#include <cmath>

using std::cout;

extern struct Stack;
extern struct Vector3;

void initialize(Stack *stack, unsigned max_size);
void destroy(Stack *stack);
void push(Stack *stack, void *elem);
void* pop(Stack *stack);
void* peek(Stack *stack);

#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

//************************************************************

TEST_CASE("Stack initialization", "[initialize]"){
	SECTION("Valid parameters"){
		Stack s;
		initialize(&s, 123);
		REQUIRE( s.data );
		REQUIRE( !s.top );
		REQUIRE( s.capacity == 123 );
	}
	SECTION("Zero size"){
		Stack p;
		initialize(&p, 0);
		REQUIRE( !s.data );
		REQUIRE( s.top < 0 );
		REQUIRE( !s.capacity );
	}
}

TEST_CASE("Stack destruction", "[destroy]"){
	Stack s;
	initialize(&s, 16);
	destroy(&s);
	REQUIRE( !s.data );
	REQUIRE( s.top < 0 );
	REQUIRE( !s.capacity );
}



