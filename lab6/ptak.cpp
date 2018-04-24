#include <iostream>
using std::cout;

class Ptak{
public:
	virtual void lec(){ cout << "Jestem ptak i lece.\n"; }
	// virtual ~Ptak(){}
};

class Pingwin : public Ptak{
public:
	virtual void lec(){ cout << "Jestem pingwin i nie umie latac.\n"; }
};

class MalyPingwin : public Pingwin{
public:
	virtual void lec(){ cout << "Jestem maly pingwin!\n"; }
};

int main(){
	cout << sizeof(Ptak) << '\n';

	Ptak a1;
	Pingwin b1;
	MalyPingwin c1;

	a1 = c1;

	MalyPingwin *mp = (MalyPingwin*)(&a1);

	a1.lec();
	b1.lec();
	c1.lec();
	cout << "--------------------\n";

	Ptak *p = &a1,
	     *q = &b1,
		 *r = &c1;
	
	p->lec();
	q->lec();
	r->lec();
	cout << "--------------------\n";

	mp->lec();
}

