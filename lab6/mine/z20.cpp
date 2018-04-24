#include <iostream>
using namespace std;

class SpaceShip{

public:
	void fly(){ cout << "Flying.\n"; }
};

class Shuttle : public SpaceShip{
public:
	void land(){ cout << "Poof! Landed.\n"; }
};

int main(){
	Shuttle sh;
	sh.fly();
	sh.land();

	SpaceShip ss;
	Shuttle *ptr = dynamic_cast<Shuttle*>(&ss);
	ptr->fly();
	ptr->land();
}