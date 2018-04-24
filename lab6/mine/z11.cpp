#include <iostream>
using namespace std;

#include "../C13/PStash.h"

class Asteroid{
public:

};

class AsteroidStash : PStash{
	int add(Asteroid *asteroid){
		PStash::add(asteroid);
	}

	Asteroid* operator[](int index) const{
		return reinterpret_cast<Asteroid*>(PStash::operator[](index));
	}
};