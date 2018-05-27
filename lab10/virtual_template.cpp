#include <iostream>

using std::cout;

template<typename T>
class Abc{
public:
	virtual ~Abc() {}

	template<typename U>
	virtual void foo(U &x) {
		std::cout << "[" << x << "]\n";
	}
};

int main(){
	Abc<float> myabc;
}
