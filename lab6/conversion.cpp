#include <iostream>
using namespace std;

class Point2D{
	int x, y;
public:
	Point2D() : x(0), y(0) {}
	Point2D(int c) : x(c), y(c) { cout << 'x'; }
	Point2D(const Point2D &p) : x(p.x), y(p.y) { cout << 'y'; }
};

class X{
	X(Point2D p) { cout << "!\n"; }
};

int main(){
	X x = 321;
}