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

typedef struct{
	unsigned int capacity, size;
	Vector3 *data;
} Stack;

// Stack initialization. A stack must not be initialized more than once.
void initialize(Stack *stack, unsigned int max_size){
	stack->capacity = max_size;
	stack->size = 0;
	if(max_size > 0)
		stack->data = (Vector3*)malloc(sizeof(Vector3) * max_size);
	else
		stack->data = NULL;
}

// Append an element.
void push(Stack *stack, const Vector3 *elem){
	if(stack->size == stack->capacity)
		fputs("Error: stack at max capacity.\n", stderr);
	else
		stack->data[stack->size++] = *elem;
}

// Pop an element.
Vector3 pop(Stack *stack){
	if(!stack->size){
		fputs("Error: no elements on the stack.\n", stderr);
		return NOT_VECTOR;
	}
	else
		return stack->data[--stack->size];
}

// View a top element.
Vector3 peek(Stack *stack){
	if(!stack->size){
		fputs("Error: no elements on the stack.\n", stderr);
		return NOT_VECTOR;
	}
	else
		return stack->data[stack->size - 1];
}

int main(void){
	Stack s;
	initialize(&s, 2);

	Vector3 a = {2, 3, 4};

	push(&s, &a);
	push(&s, &a);
	push(&s, &a);
	push(&s, &a);
	
	for(int i=0; i!=4; ++i){
		Vector3 x = pop(&s);
		printVector3(&x);
	}
}