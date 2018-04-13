#include <iostream>
using std::cout;

class A{
    static unsigned no;
	static void decNo(){--no; cout << no << '\n';}
	static void incNo(){++no; cout << no << '\n';}

	A(){incNo();}
	A(const A&){
		incNo();
		// ...
	}
	~A(){decNo();}

	static A *instance;

public:
	// Lazy programming - initialize only when needed.
	static A& getInstance(){
		if(!instance)
			instance = new A();
		return *instance;
	}
};

unsigned A::no = 0;
A *A::instance = 0;

int main(){
	/*A::incNo();
	A a1, a2;
	// a1.incNo();
	A a3 = a1;
	A *tab[10];*/

	A &first = A::getInstance();
	A &second = A::getInstance();

	printf("[%x] [%x]\n", &first, &second);
}