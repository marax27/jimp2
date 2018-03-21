#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{double x, y, z;} Vector3;

//************************************************************

/* DynamicStack pozwala przechowywac dowolna liczbe elementow
 * jednego typu. Dodawane elementy sa kopiowane przez stos.
 *
 * Jesli obiekty sa tak duze, ze ich kopiowanie jest
 * nieoplacalne, mozna rozwazyc utworzenie stosu wskaznikow
 * na takie obiekty.
 */

#define ALLOC_CHUNK 4

typedef unsigned int dsUint;
enum StackState{FAIL, GOOD};

typedef struct{
	dsUint capacity,   //current capacity, in elements
	       size,       //number of elements currently stored
	       elem_size;  //size of a single element, in bytes
	enum StackState state;   //set to FAIL if something goes wrong
	void *data;
} DynamicStack;

void initialize(DynamicStack *stack, dsUint size_of_element);
void destroy(DynamicStack *stack);
void reserve(DynamicStack *stack, dsUint new_capacity);
int verifyIntegrity(DynamicStack *stack, dsUint n);
void push(DynamicStack *stack, const void *elem, dsUint size_of_element);
void pop(DynamicStack *stack, void *where, dsUint size_of_element);
void peek(DynamicStack *stack, void *where, dsUint size_of_element);

void charTest();

//************************************************************

int main(void){
	// Etap 1: kilka podstawowych operacji na stosie na typ Vector3.
	// Etap 2: interaktywny stos na typ char.

	DynamicStack s;
	initialize(&s, sizeof(Vector3));
	for(int i=0; i!=15; ++i){
		Vector3 a = {i, i, i};
		push(&s, &a, sizeof(Vector3));
	}
	
	printf("S: %d/%d (state=%d)\n", s.size, s.capacity, s.state);
	Vector3 x;
	for(int i=0; i!=15; ++i){
		pop(&s, &x, sizeof(Vector3));
		printf("%d) [%f, %f, %f]\n", i, x.x, x.y, x.z);
	}
	pop(&s, &x, sizeof(Vector3));  //blad

	puts("Press [Enter]");
	getc(stdin);
	
	puts("--------------------");
	charTest();
}

//************************************************************

// Stack initialization. A stack must not be initialized more than once.
void initialize(DynamicStack *stack, dsUint size_of_element){
	stack->capacity = ALLOC_CHUNK;
	stack->size = 0;
	stack->elem_size = size_of_element;
	stack->state = GOOD;
	stack->data = malloc(stack->elem_size * stack->capacity);
	if(!stack->data){
		fputs("Error: failed to allocate memory.\n", stderr);
		stack->state = FAIL;
		stack->capacity = 0;
	}
}

// Free a stack.
void destroy(DynamicStack *stack){
	free(stack->data);
	stack->data = NULL;
	stack->capacity = stack->size = stack->elem_size = 0;
}

// Change stack's capacity.
void reserve(DynamicStack *stack, dsUint new_capacity){
	if(new_capacity <= stack->size){
		fputs("Error: cannot resize, too many elements on the stack.\n", stderr);
		stack->state = FAIL;
	}
	else{
		// Allocate new memory.
		void *ptr = malloc(stack->elem_size*new_capacity);
		if(!ptr){
			fputs("Error: failed to reallocate memory.", stderr);
			stack->state = FAIL;
			return;
		}
		else{
			// Move old data to new memory.
			stack->capacity = new_capacity;
			memcpy(ptr, stack->data, stack->size*stack->elem_size);
			free(stack->data);
			stack->data = ptr;
		}
	}
}

// Check if n is equal to stack's element size. If not, act accordingly.
int verifyIntegrity(DynamicStack *stack, dsUint n){
	if(stack->elem_size != n){
		fputs("Error: invalid element size.\n", stderr);
		stack->state = FAIL;
		return 0;
	}
	return 1;
}

// Append an element.
void push(DynamicStack *stack, const void *elem, dsUint size_of_element){
	// Check sizeof integrity.
	if(!verifyIntegrity(stack, size_of_element))
		return;

	// Resize stack, if needed.
	if(stack->size == stack->capacity){
		reserve(stack, stack->capacity + ALLOC_CHUNK);
		if(stack->state == FAIL){
			fputs("Failed to push an object.\n", stderr);
			return;
		}
	}

	// Appending an object.
	memcpy((char*)stack->data + stack->elem_size*stack->size, elem, stack->elem_size);
	++stack->size;
}

// Pop an object.
void pop(DynamicStack *stack, void *where, dsUint size_of_element){
	peek(stack, where, size_of_element);
	if(stack->state)
		--stack->size;
}

// View a top element.
void peek(DynamicStack *stack, void *where, dsUint size_of_element){
	if(!verifyIntegrity(stack, size_of_element))
		return;
	
	if(!stack->size){
		fputs("Error: cannot pop; stack is empty.\n", stderr);
		stack->state = FAIL;
		return;
	}

	char *last_elem = (char*)stack->data + stack->elem_size*(stack->size - 1);
	memcpy(where, last_elem, stack->elem_size);
}

//************************************************************

void charTest(){
	DynamicStack s;
	initialize(&s, sizeof(char));

	puts("DynamicStack interactive");
	puts("reset   peek   view   push   pop   size   capacity   exit   quit\n");

	while(1){
		size_t n = 128;		
		char *buffer = (char*)malloc(n);

		printf("[%s] >> ", s.state ? "GOOD" : "FAIL");
		if( getline(&buffer, &n, stdin) == -1){
			fputs("Failure to read from stdin.\n", stderr);
			break;
		}

		// Trim newline.
		size_t c = strlen(buffer) - 1;
		buffer[c] = '\0';  //newline

		//printf("[%s](%lu)\n", buffer, n);
		if(!strcmp(buffer, "reset")){
			s.state = GOOD;
		}
		else if(!strcmp(buffer, "peek")){
			char x;
			peek(&s, &x, sizeof(char));
			printf("Peek: %c (%x)\n", x, (unsigned)x);
		}
		else if(!strcmp(buffer, "view")){
			size_t N = s.size*s.elem_size + 1;
			if(N <= 1)
				puts("View: stack empty.");
			else{
				char *view = (char*)malloc(N);
				view[N] = '\0';
				memcpy(view, s.data, N-1);
				printf("View: {%s}\n", view);
				free(view);
			}
		}
		else if(!strcmp(buffer, "push")){
			char c = rand() % 26 + 0x41;  //get random capital letter
			printf("Adding '%c' (%d/%d -> ", c, s.size, s.capacity);
			push(&s, &c, sizeof(char));
			printf("%d/%d)\n", s.size, s.capacity);
		}
		else if(!strcmp(buffer, "pop")){
			char x;
			pop(&s, &x, sizeof(char));
			if(s.state)
				printf("Pop: %c\n", x);
		}
		else if(!strcmp(buffer, "size")){
			printf("Size: %d objects\n", s.size);
		}
		else if(!strcmp(buffer, "capacity")){
			printf("Capacity: %d objects\n", s.capacity);
		}
		else if(!strcmp(buffer, "exit") || !strcmp(buffer, "quit"))
			break;
		else
			puts("?");

		free(buffer);
	}

	destroy(&s);
}