#include <iostream>

#include "doublylinkedlist.h"

using std::cerr;

int main(){
	DoublyLinkedList<bool> list;

	// cerr << "After creation: ";
	// list.printList();

	for(int i=0; i!=15; ++i)
		list.push_front(i%2);
	
	for(int i=0; i!=6; ++i)
		list.push_back(true);

	// cerr << "Filled list: ";
	// list.printList();

	// cerr << "Popping back:\n";
	for(int i=0; i!=8; ++i){
		cerr << "<Pop: " << list.pop_back() << "> ";
		// list.printList();
	}
	
	
	cerr << "\n\nRange-based for loop\n";
	for(auto i : list)
		cerr << i << ' ';
	cerr << '\n';

	cerr << "iterator\n";
	for(DoublyLinkedList<bool>::iterator i = list.begin(); i != list.end(); ++i)
		cerr << *i << ' ';
	cerr << '\n';

	cerr << "Nodes - twice\n";
	for(int i=0; i!=2; ++i){
		DoublyLinkedList<bool>::Node n = list.front();
		while(n != list.back()){
			cerr << n.value << ' ';
			n.value = !n.value;  //all bits negated
			n = list.next(n);
		}
		cerr << '\n';
	}
}