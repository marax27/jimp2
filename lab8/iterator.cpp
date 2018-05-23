#include <iostream>
#include "singlylinkedlist.h"

using std::cout;

class ListInt : public SinglyLinkedList<int>{
public:
	class Iterator{
		SinglyLinkedList<int>::Node *current;
		SinglyLinkedList<int> &li;
	public:
		int& operator++(){
			current = li.next(current);
			return current->value;
		}
		int operator++(int){
			const SinglyLinkedList<int>::Node &old = *current;
			current = li.next(current);
			return old.value;
		}
		bool operator==(const Iterator &r) const;
	};
};

int main(){
	ListInt sl;
}