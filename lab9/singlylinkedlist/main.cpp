#include "singlylinkedlist.h"
#include <iostream>

using std::cout;

int main(){
	SinglyLinkedList<float> sl;

	for(int i=0; i!=5; ++i)
		sl.push_back(3.14 + 5*i);

	std::cout << "Front: ";
	for(auto i : sl)
		std::cout << "--> [" << i << "] ";
	std::cout << '\n';
}