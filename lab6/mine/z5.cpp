//: C14:Combined.cpp
// Inheritance & composition
#include <iostream>
using std::cout;

class A {
	int i;
public:
	A(int ii) : i(ii) { cout << "Constructor of A\n"; }
	~A() { cout << "Destructor of A\n"; }
	void f() const {}
};

class B {
	int i;
public:
	B(int ii) : i(ii) { cout << "Constructor of B\n"; }
	~B() { cout << "Destructor of B\n"; }
	void f() const {}
};

class C : public B {
	A a;
public:
	C(int ii) : B(ii), a(ii) { cout << "Constructor of C\n"; }
	~C() { cout << "Destructor of C\n"; } // Calls ~A() and ~B()
	void f() const { // Redefinition
	a.f();
	B::f();
	}
};

class D : public B {
	C c_member;
public:
	D(int x) : B(x), c_member(x) { cout << "Constructor of D\n"; }
	~D() { cout << "Destructor of D\n"; }
};

int main() {
	// C c(47);
	D d(321);
} ///:~ 