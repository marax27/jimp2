#include <iostream>
using namespace std;

class Traveler{
protected:
	string trav_str;
public:
	Traveler(string s) : trav_str(s) {}
	Traveler(const Traveler &right) : trav_str(right.trav_str) {
		cout << "Copied a Traveler.\n";
	}

	Traveler& operator=(const Traveler &right){
		trav_str = right.trav_str;
		cout << "Traveler assignment.\n";
	}
};

class Pager{
	string pager_str;
public:
	Pager(string s) : pager_str(s) {}
	Pager(const Pager &right) : pager_str(right.pager_str) {
		cout << "Copied a Pager.\n";
	}

	Pager& operator=(const Pager &right){
		pager_str = right.pager_str;
		cout << "Pager assignment.\n";
	}

	string getStr()const{return pager_str;}
};

class BusinessTraveler : public Traveler{
	Pager pager;
public:
	BusinessTraveler() : Traveler("John Doe"), pager("VIP pager") {}
	BusinessTraveler(string s) : Traveler(s), pager(s) {}
	BusinessTraveler(const BusinessTraveler &right)
		: Traveler(right.trav_str), pager(right.pager) {}
	
	BusinessTraveler& operator=(const BusinessTraveler &right){
		Traveler::operator=(right);
		pager = right.pager;
		return *this;
	}

	void print() const{ cout << "["<<trav_str<<"]["<<pager.getStr()<<"]\n"; }
};

int main(){
	BusinessTraveler a("xxx");
	BusinessTraveler b;

	a.print();
	a = b;
	a.print();
}