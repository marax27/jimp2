#include <iostream>
using std::cout;

class A{
public:
	A(){ cout << "I am constructor of A.\n"; }
};

class B{
public:
	B(){ cout << "I am constructor of B.\n"; }
};

class C{
	A obj_a;
	B obj_b;
};

int main(){
	C cx;
}