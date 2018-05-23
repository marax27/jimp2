#include <iostream>
using std::size_t;

//************************************************************

template<typename T, size_t SIZE> class Stack{
public:
	Stack() : top(0) {}

	void push(const T &elem){
		elements[top++] = elem;
	}

	T pop(){
		return elements[--top];
	}

	bool empty() const{
		return !top;
	}

private:
	T elements[SIZE];
	size_t top;
};

//************************************************************

int main(){
	// int a = 15;
	Stack<int, 100> si;
	// Stack<int, a> sid;
}