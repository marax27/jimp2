#ifndef _MISC_H_
#define _MISC_H_

#include <cmath>
#include <cstdint>
#include <exception>

//************************************************************

// Typ zmiennoprzecinkowy uzywany przy obliczeniach.
// Niewykluczona zmiana na double w przyszlosci.
typedef float fp_t;

// Sprawdza, czy x nalezy do przedzialu [min, max].
// Wlasnosc: dla dowolnego a,x: in(a, x, a) -> false
template<typename T, typename U> bool in(T min, U x, T max){
	return min <= x && x < max;
}

// Ogranicza wartosc x do wartosci z przedzialu [min, max].
template<typename T, typename U> void limitTo(T min, U &x, T max){
	if(x < min)
		x = min;
	else if(x > max)
		x = max;
}

inline fp_t squared(fp_t x){ return x*x; }

//************************************************************

typedef float fp_t;

// Reprezentacja punktu na bitmapie.
struct Point{
	Point() : x(0), y(0) {}
	Point(int x, int y) : x(x), y(y) {}

	int x = {0},
	    y = {0};
	
	static fp_t distance(const Point &a, const Point &b){
		return sqrt( squared((int)a.x - b.x) + squared((int)a.y - b.y) );
	}
};

// Reprezentacja koloru RGB.
struct Colour{
	Colour() : r(0), g(0), b(0) {}
	Colour(unsigned char r, unsigned char g, unsigned char b)
		: r(r), g(g), b(b) {}
	unsigned char r = {0},
	              g = {0},
	              b = {0};
};

//************************************************************

fp_t deg2Rad(fp_t degrees);
bool sign(int x);
bool isInFirstQuarter(fp_t alfa1, fp_t);
bool isInSecondQuarter(fp_t alfa1, fp_t alfa2);
bool isInThirdQuarter(fp_t alfa1, fp_t alfa2);
bool isInFourthQuarter(fp_t, fp_t alfa2);

// Oblicz wsp. x punktu (x, y) lezacego na odcinku AB.
int linearInterpolation(Point A, Point B, int y);

#endif  //_MISC_H_
