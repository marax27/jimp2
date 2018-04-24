//: C14:Instrument.cpp
// Inheritance & upcasting
#include <iostream>
using std::cout;

enum note { middleC, Csharp, Cflat }; // Etc.

class Instrument {
public:
	virtual void play(note) const { cout << "Playing music lalalala\n"; }
	void prepare() {}
};

// Wind objects are Instruments
// because they have the same interface:
class Wind : public Instrument {
public:
	virtual void play(note) const { cout << "Shhh\n"; }
};
void tune(Instrument& i) {
	// ...
	i.prepare();
	i.play(middleC); 
}

int main() {
	Wind flute;
	tune(flute); // Upcasting
} ///:~ 