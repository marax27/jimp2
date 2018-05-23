#include <iostream>
#include <cstring>
using std::cout;

template<typename T>
	const T& min(T a, T b){
		return a<b ? a : b;
	}

// template<> const char* min<const char*>(const char *a, const char *b){
// 	cout << "{char test}";
// 	return strcmp(a,b) < 0 ? a : b;
// }

int main(){
	cout << ::min(2, -7) << ','
	     << ::min(3.0, 1.97) << ','
	     << ::min<int>(8, 99.0f) << ','
	     << ::min("x", "y") << '\n';
}