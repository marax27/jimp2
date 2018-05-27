#include <iostream>

using std::cout;

template<typename T1, typename T2>
class MyPair{
	T1 a;
	T2 b;
};

template<typename T>
class MyPair<T,T>{
	T x[2];
};

template<typename T1, typename T2>
class MyPair<T1*, T2*>{
	;
};

template<> class MyPair<int, float>{
	;
};

int main(){
	MyPair<int, float> h;
	MyPair<char, char> j;
}
