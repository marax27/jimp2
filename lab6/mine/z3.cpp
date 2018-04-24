#include <iostream>
using std::cout;

class A{
public:
	A(){ cout << "I am constructor of A.\n"; }
	~A(){ cout << "I am destructor of A.\n"; }
};

class B : public A{
public:
	B(){ cout << "\tI am constructor of B.\n"; }
	~B(){ cout << "\tI am destructor of B.\n"; }
};

class C : public B{
public:
	C(){ cout << "\t\tI am constructor of C.\n"; }
	~C(){ cout << "\t\tI am destructor of C.\n"; }
};


int main(){
	C cx;
}