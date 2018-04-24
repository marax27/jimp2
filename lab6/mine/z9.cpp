#include <vector>
#include <string>
#include <iostream>

using namespace std;

class StringVector : vector<void*>{
public:
	void push_back(string *s){
		vector<void*>::push_back(reinterpret_cast<void*>(s));
	}
	void push_back(void *s){ vector<void*>::push_back(s); }

	string* operator[](size_t idx){
		return reinterpret_cast<string*>( vector<void*>::operator[](idx) );
	}
};

int main(){
	string s1 = "abc";
	void *m = malloc(1024);

	StringVector sv;
	sv.push_back(&s1);
	sv.push_back(m);

	free(m);
}