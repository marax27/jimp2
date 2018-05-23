#include "set.h"
#include <iostream>

using std::cout;
using std::cin;

int main(){
	Set<int> set;

	// Inserting.
	for(int i=0; i!=14; ++i)
		set.insert(i*i);

	// set = {0, 1, 4, 9, ..., 144, 169}

	// Searching.
	cout << "T F T F T\n"
	     << set.in(9) << ' ' << set.in(39) << ' ' << set.in(64)
		 << ' ' << set.in(66) << ' ' << set.in(169) << "\n\n";
	
	set.erase(64);  //is in set - 64 removed
	set.erase(32);  //not in set - nothing happens

	cout << "T F F F T\n"
	     << set.in(9) << ' ' << set.in(39) << ' ' << set.in(64)
		 << ' ' << set.in(66) << ' ' << set.in(169) << "\n\n";
	
	// Range-based for loop.
	cout << "Set: ";
	for(auto &i : set)
		cout << '[' << i << ']';
	cout << '\n';

	// Copy constructor and assignment.
	Set<int> p(set), q;
	q = p;

	// Set comparison.
	cout << "Expected: 1, 1. Result: " << (set == p) << ", " << (set == q) << '\n';

	p.insert(-1);
	q.erase(144);
	cout << "Expected: 0, 0. Result: " << (set == p) << ", " << (set == q) << "\n\n";
	
	// Iterators.
	int sum = 0;
	for(Set<int>::iterator it = set.begin(); it != set.end(); ++it)
		sum += *it;
	cout << "Sum = " << sum << " (expected: " << (1+4+9+16+25+36+49+81+100+121+144+169) << ")\n";
}