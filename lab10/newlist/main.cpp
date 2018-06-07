#include <iostream>

#include "doublylinkedlist.h"

using std::cout;
using std::cerr;

int main(){
	DoublyLinkedList<bool> list;

	list.DEBUG_iterate();

	for(int i=0; i!=13; ++i)
		list.push_front(i%7);
	
	for(int i=0; i!=6; ++i)
		list.push_back(true);

	list.DEBUG_iterate();

	for(int i=0; i!=8; ++i){
		cerr << '<' << list.pop_back() << ' ';
		list.DEBUG_iterate();
	}
	
	// cout << "iterator test\n";
	// for(auto i = list.begin(); i != list.end(); ++i)
	// 	cerr << *i << ' ';
	// cout << '\n';

	cout << "Node test\n";
	auto n = list.front();
	while(n != list.back()){
		cerr << n.value() << ' ';
		n = list.next(n);
	}
	cout << '\n';
}