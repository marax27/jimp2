#include "vector.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using std::cout;

int main(){
	/*Vector<int> ds;
	for(int i=0; i!=5; ++i)
		ds.push_back(i*i*i);
	for(auto i : ds)
		std::cout << i << '-';
	std::cout << '\n';*/

	//------------------------------

	Vector<bool> bf;
	for(int i=0; i!=5; ++i)
		bf.push_back(i >> 1);
	for(auto i : bf)
		cout << i << ' ';
	cout << '\n';

	for(int i=0; i!=bf.size(); ++i){
		auto b = bf[i];
		cout << b << ' ';
		// b = true;  //assign true to all vector elements
	}
	std::cout << '\n';

	for(auto i : bf)
		cout << i << ' ';
	cout << '\n';

	bf.erase(0);
	bf.erase(0);
	for(auto i : bf)
		cout << i << ' ';
	cout << '\n';

}