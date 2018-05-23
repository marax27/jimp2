#include "dynamicstack.h"
#include <iostream>

using std::cout;

int main(){
	DynamicStack<int> ds;
	for(int i=0; i!=5; ++i)
		ds.push(i*i*i);
	for(auto i : ds)
		std::cout << i << '-';
	std::cout << '\n';
}