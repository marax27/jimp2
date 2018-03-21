#include <stdlib.h>
#include <stdio.h>

typedef struct{
	double x, y, z;
} Vector3;

void printVector3(Vector3 *vec){
	printf("[%f, %f, %f]\n", vec->x, vec->y, vec->z);
}

//************************************************************

typedef struct{
	void **data;
	unsigned top;
	unsigned capacity;
} Stack;

void initialize(Stack *stack, unsigned max_size);
void destroy(Stack *stack);
void push(Stack *stack, void *elem);
void* pop(Stack *stack);
void* peek(Stack *stack);

//************************************************************

/*int main(void){
	Stack s;
	initialize(&s, 2);

	Vector3 a = {2, 3, 4},
	        b = {1, 1, 1};

	push(&s, &a);
	push(&s, &b);
	push(&s, &a);  //stack is full
	push(&s, &b);  //stack is full
	
	// Since there are 2 elements on the stack,
	// the loop will fail to pop twice.
	for(int i=0; i!=4; ++i){
		Vector3 *x = (Vector3*)pop(&s);
		if(x)
			printVector3(x);
		else
			puts("{NULL}");
	}

	destroy(&s);
}*/

//************************************************************

// Stack initialization.
void initialize(Stack *stack, unsigned max_size){
	stack->capacity = max_size;
	stack->top = -1;
	if(max_size > 0){
		stack->data = (void**)malloc(sizeof(void*) * max_size);
		if(!stack->data){
			fputs("Error: failed to allocate memory.\n", stderr);
			stack->capacity = 0;
		}
	}
	else
		stack->data = NULL;
}

// Free a stack.
void destroy(Stack *stack){
	free(stack->data);
	stack->capacity = 0;
	stack->top = -1;
}

// Append an element.
void push(Stack *stack, void *elem){
	if((unsigned)(stack->top + 1) == stack->capacity)
		fputs("Error: stack at max capacity.\n", stderr);
	else
		stack->data[++stack->top] = elem;
}

// Pop an element.
void* pop(Stack *stack){
	void *result = peek(stack);
	--stack->top;
	return result;
}

// View a top element.
void* peek(Stack *stack){
	if(stack->top < 0){
		fputs("Error: no elements on the stack.\n", stderr);
		return NULL;
	}
	else
		return stack->data[stack->top];
}
