#include "vector.h"
#include <iostream>

using std::cout;

int main(){
	Vector<int> ds;
	for(int i=0; i!=5; ++i)
		ds.push_back(i*i*i);
	for(auto i : ds)
		std::cout << i << '-';
	std::cout << '\n';
}