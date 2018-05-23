#include "singlylinkedlist.h"
#include "doublylinkedlist.h"
#include "dynamicstack.h"
#include "vector.h"

using std::cout;

int main(){
	DoublyLinkedList<float> sl;

	for(int i=0; i!=5; ++i)
		sl.push_back(3.14 + 5*i);

	std::cout << "Front: ";
	for(const auto &i : sl)
		std::cout << "--> [" << i << "] ";
	std::cout << '\n';



	Vector<int> ds;
	for(int i=0; i!=5; ++i)
		ds.push_back(i*i*i);
	for(auto i : ds)
		std::cout << i << '-';
	std::cout << '\n';
}