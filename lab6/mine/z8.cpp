//: C14:NameHiding.cpp
// Hiding overloaded names during inheritance

#define _SHOW

#include <iostream>
#include <string>
using namespace std;

class Base {
public:
	int f() const {
		cout << "Base::f()\n";
		return 1;
	}
	int f(string) const { return 1; }
	void g() {}

	int h() const { return 0; }
	int h(int) const { return 1; }
	int h(int, int) const { return 2; }
};

class Derived1 : public Base {
public:
	void g() const {}
};

class Derived2 : public Base {
public:
	// Redefinition:
	int f() const {
		cout << "Derived2::f()\n";
		return 2;
	}

	#ifdef _SHOW
		int h() const { return 37; }
	#endif
};

class Derived3 : public Base {
public:
	// Change return type:
	void f() const { cout << "Derived3::f()\n"; }
};
class Derived4 : public Base {
public:
	// Change argument list:
	int f(int) const {
		cout << "Derived4::f()\n";
		return 4;
	}
};

int main() {
	Base b;
	cout << b.h() << ", " << b.h(1) << ", " << b.h(1, 2) << '\n';

	Derived2 d2;
	cout << d2.h() << ", " << d2.h(1) << ", " << d2.h(1, 2) << '\n';	
} ///:~ 