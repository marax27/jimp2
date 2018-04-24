#include <iostream>
using namespace std;

class C{
public:
	static void xPrinter(){ cout << "X\n"; }
	static void yPrinter(){ cout << "Y\n"; }
};

class D : public C{
public:
	static void xPrinter(){ cout << "XXX\n"; }
};

int main(){
	C::xPrinter();
	C::yPrinter();

	D::xPrinter();
	D::yPrinter();
}