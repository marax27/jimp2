//: C14:Car.cpp
// Public composition
class Engine {
public:
	void start() const {}
	void rev() const {}
	void stop() const {}
};
class Wheel {
public:
	void inflate(int psi) const {}
};
class Window {
public:
	void rollup() const {}
	void rolldown() const {}
};
class Door {
public:
	Window window;
	void open() const {}
	void close() const {}
};

class Vehicle{
public:
	Vehicle(int clr) : colour(clr) {}
	int colour;	
};

class Car : public Vehicle {
public:
	Car(int clr) : Vehicle(clr) {}

	Engine engine;
	Wheel wheel[4];
	Door left, right; // 2-door 
};

int main() {
	Car car(321);
	car.left.window.rollup();
	car.wheel[0].inflate(72);
} ///:~ 