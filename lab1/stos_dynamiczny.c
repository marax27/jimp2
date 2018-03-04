#include <stdlib.h>
#include <stdio.h>
#include <math.h>  //NAN

typedef struct{
	double x, y, z;
} Vector3;

const Vector3 NOT_VECTOR = {NAN, NAN, NAN};

void printVector3(Vector3 *vec){
	printf("[%f, %f, %f]\n", vec->x, vec->y, vec->z);
}

//************************************************************

#define ALLOC_CHUNK 8

typedef struct{
	unsigned int capacity, size;
	Vector3 *data;
} DynamicStack;

// Stack initialization. A stack must not be initialized more than once.
void initialize(DynamicStack *stack){
	stack->capacity = ALLOC_CHUNK;
	stack->size = 0;
	stack->data = (Vector3*)malloc(sizeof(Vector3) * ALLOC_CHUNK);
	if(!stack->data){
		fputs("Error: failed to allocate memory.\n", stderr);
		stack->capacity = 0;
	}
}

// Free a stack.
void destroy(DynamicStack *stack){
	free(stack->data);
	stack->capacity = stack->size = 0;
}

// Append an element.
void push(DynamicStack *stack, const Vector3 *elem){
	if(stack->size == stack->capacity){
		Vector3 *ptr = realloc(stack->data, stack->capacity + ALLOC_CHUNK);
		if(!ptr){
			fputs("Error: not enough memory.", stderr);
			return;
		}
		else{
			stack->capacity += ALLOC_CHUNK;
			stack->data = ptr;
		}
	}
	
	// Append an element.
	stack->data[stack->size++] = *elem;
}

// Pop an element.
Vector3 pop(DynamicStack *stack){
	if(!stack->size){
		fputs("Error: no elements on the stack.\n", stderr);
		return NOT_VECTOR;
	}
	else
		return stack->data[--stack->size];
}

// View a top element.
Vector3 peek(DynamicStack *stack){
	if(!stack->size){
		fputs("Error: no elements on the stack.\n", stderr);
		return NOT_VECTOR;
	}
	else
		return stack->data[stack->size - 1];
}

int main(void){
	DynamicStack s;
	initialize(&s);

	Vector3 a = {2, 3, 4};

	for(int i=0; i!=17; ++i){
		push(&s, &a);
		printf("Status: %d/%d\n", s.size, s.capacity);
	}
	
	printf("Size before pop(): %d\n", s.size);
	for(int i=0; i!=6; ++i){
		Vector3 x = pop(&s);
		printVector3(&x);
	}
	printf("Size after pop(): %d\n", s.size);

	destroy(&s);
}