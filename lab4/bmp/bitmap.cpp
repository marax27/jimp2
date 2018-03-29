#include <iostream>
#include <fstream>
#include <cmath>
#include "bmp.h"

//************************************************************

// Typ zmiennoprzecinkowy uzywany przy obliczeniach.
// Niewykluczona zmiana na double w przyszlosci.
typedef float fp_t;

// Zwraca kwadrat x.
fp_t squared(fp_t x){
	return x*x;
}

// Konwersja stopni na radiany.
fp_t deg2Rad(fp_t degrees){
	return degrees * M_PI / 180.0;
}

// Funkcja znaku.
bool sign(int x){
	return x ? (x > 0 ? 1 : -1) : 0;
}

// Sprawdzenie, czy czesc wycinka/luku ograniczonego
// katami alfa1, alfa2 lezy w danej cwiartce.
bool isInFirstQuarter(fp_t alfa1, fp_t){
	return alfa1 < 90.0f;
}
bool isInSecondQuarter(fp_t alfa1, fp_t alfa2){
	return alfa1 < 180.0f && (alfa1 >= 90.0f || alfa2 >= 90.0f);
}
bool isInThirdQuarter(fp_t alfa1, fp_t alfa2){
	return alfa1 < 270.0f && (alfa1 >= 180.0f || alfa2 >= 180.0f);
}
bool isInFourthQuarter(fp_t, fp_t alfa2){
	return alfa2 >= 270.0f;
}

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

// Reprezentacja punktu na bitmapie.
struct Point{
	Point(int x, int y) : x(x), y(y) {}

	int x = {0},
	    y = {0};
	
	static fp_t distance(const Point &a, const Point &b){
		return sqrt( squared((int)a.x - b.x) + squared((int)a.y - b.y) );
	}
};

// Reprezentacja koloru RGB.
struct Colour{
	Colour(unsigned char r, unsigned char g, unsigned char b)
		: r(r), g(g), b(b) {}
	unsigned char r = {0},
	              g = {0},
	              b = {0};
};

//************************************************************

// Oblicz wsp. x punktu (x, y) lezacego na odcinku AB.
int linearInterpolation(Point A, Point B, int y);

void safeDrawPoint(JiMP2::BMP &bitmap, Point P, Colour clr);
void drawLine(JiMP2::BMP &bitmap, Point A, Point B, Colour clr);
void drawCircle(JiMP2::BMP &bitmap, Point S, uint16_t r, Colour clr);
void drawDisk(JiMP2::BMP &bitmap, Point S, uint16_t r, Colour clr);
void drawArc(JiMP2::BMP &bitmap, Point S, uint16_t r,
	fp_t alfa1, fp_t alfa2, Colour clr);
void drawCircularSector(JiMP2::BMP &bitmap, Point S, uint16_t r,
	fp_t alfa1, fp_t alfa2, Colour clr);
void drawEllipse(JiMP2::BMP &bitmap, Point S, uint16_t a, uint16_t b, Colour clr);
void drawRectangle(JiMP2::BMP &bitmap, Point A, Point B, Colour clr);

//************************************************************

const uint16_t imgWidth = 800;
const uint16_t imgHeight = 600;

void lineTest(){
	JiMP2::BMP bmp(imgWidth, imgHeight);

	const Colour clr{0, 0, 128};
	const Colour clr1{192, 32, 32};
	const int S = 500;   //duza wartosc, by linie wykraczaly za bitmape
	const int mx = imgWidth/2;
	const int my = imgHeight/2;
	const Point center = {mx, my};

	drawLine(bmp, center, {mx+S, my}, clr);
	drawLine(bmp, center, {mx, my-S}, clr);
	drawLine(bmp, center, {mx-S, my}, clr);
	drawLine(bmp, center, {mx, my+S}, clr);

	for(float a = 25.0f; a < 360.0f; a += 25.0f)
		drawLine(bmp, center, Point(mx + S*cos(deg2Rad(a)), my + S*sin(deg2Rad(a))), clr);

	std::ofstream writer("line.bmp", std::ofstream::binary);
	writer << bmp;
}

void circleDiskTest(){
	JiMP2::BMP bmp(imgWidth, imgHeight);

	const int r = 100;
	Colour c1{128, 0, 255};

	drawCircle(bmp, {r, r}, r, c1);
	drawCircle(bmp, {0, imgHeight}, r, c1);
	drawCircle(bmp, {-5, imgHeight+5}, r, c1);
	for(int i=0; i!=5; ++i)
		drawCircle(bmp, {3*r, -25*i}, r, c1);

	drawDisk(bmp, {r, 3*r}, r, c1);
	drawDisk(bmp, {r, 7*r}, 3*r, {0, 0, 0});

	std::ofstream writer("circledisk.bmp", std::ofstream::binary);
	writer << bmp;
}

void arcSectorTest(){
	const uint16_t w = 800, h = 600;
	JiMP2::BMP bmp(w, h);
	Colour black{0, 0, 0}, blue{64, 64, 128};

	int r = 50;
	for(int i=0; i<6; ++i)
		drawArc(bmp, {50+100*i, 50}, r, 30*i, 360-30*i, black);

	for(int i=0; i<4; ++i)
		drawArc(bmp, {50, 150}, r, 30+90*i, 60+90*i, black);

	for(int i=0; i<6; ++i)
		drawArc(bmp, {150+100*i, 150}, r, 0, 50*(1+i), black);

	for(int i=0; i<8; ++i)
		drawCircularSector(bmp, {50+2*r*i, 350}, r, 20*i, 360-20*i, blue);

	for(int i=0; i<8; ++i)
		drawCircularSector(bmp, {50+2*r*i, 350+3*r}, r, 40*i, 40*(i+1), blue);	

	std::ofstream writer("arcsector.bmp", std::ofstream::binary);
	writer << bmp;
}

void ellipseTest(){
	const uint16_t w = 800, h = 600;
	JiMP2::BMP bmp(w, h);

	const Point mid{w/2, h/2};

	drawEllipse(bmp, mid, w/2, h/2, {0, 0, 0});
	drawEllipse(bmp, mid, w/3, h/3, {50, 50, 50});
	drawEllipse(bmp, mid, w/4, h/4, {100, 100, 100});
	drawEllipse(bmp, mid, w/6, h/6, {150, 150, 150});
	drawEllipse(bmp, mid, w/9, h/9, {200, 200, 200});

	drawEllipse(bmp, mid, 100, 100, {192, 0, 0});
	drawEllipse(bmp, mid, 100, 200, {192, 0, 0});
	drawEllipse(bmp, mid, 100, 400, {192, 0, 0});

	std::ofstream writer("ellipse.bmp", std::ofstream::binary);
	writer << bmp;
}

void rectangleTest(){
	const uint16_t w = 800, h = 600;
	JiMP2::BMP bmp(w, h);

	drawRectangle(bmp, {220, 220}, {120, 420}, {0, 0, 0});
	drawRectangle(bmp, {-100, -100}, {300, 200}, {0, 0, 192});	
	drawRectangle(bmp, {600, 500}, {300, 200}, {0, 168, 0});	

	std::ofstream writer("rectangle.bmp", std::ofstream::binary);
	writer << bmp;
}

void run(){
	lineTest();
	circleDiskTest();
	arcSectorTest();
	ellipseTest();
	rectangleTest();
}

//************************************************************

int linearInterpolation(Point A, Point B, int y){
	//if(A.x != B.x)
	if(A.y != B.y)
		return B.x + (y-B.y)*(A.x-B.x)/(A.y-B.y);
	else
		return B.x;
}

//************************************************************

void safeDrawPoint(JiMP2::BMP &bitmap, Point P, Colour clr){
	if(in(0, P.x, (int)bitmap.getWidth()+1) && in(0, P.y, (int)bitmap.getHeight()+1))
		bitmap.setPixel(P.x, P.y, clr.r, clr.g, clr.b);
}

//************************************************************

// Linia.
// W oparciu o algorytm Bresenhama.
// https://gist.github.com/bert/1085538
void drawLine (JiMP2::BMP &bitmap, Point A, Point B, Colour clr)
{
	int dx =  abs (B.x - A.x), sx = A.x < B.x ? 1 : -1,
	    dy = -abs (B.y - A.y), sy = A.y < B.y ? 1 : -1; 
	int err = dx + dy,
	    e2;
 
	while(true){
		safeDrawPoint(bitmap, A, clr);
		if(A.x == B.x && A.y == B.y)
			break;
		e2 = 2*err;
		if(e2 >= dy){
			err += dy;
			A.x += sx;
		}
		if(e2 <= dx){
			err += dx;
			A.y += sy;
		}
	}
}

//************************************************************

// Okrag.
// https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
void drawCircle(JiMP2::BMP &bitmap, Point S, uint16_t r, Colour clr){
	int dx = 1,
	    dy = 1;
	int x = r-1,
	    y = 0,
	    err = dx - (r << 1);
	
	while(x >= y){
		safeDrawPoint(bitmap, {S.x+x, S.y+y}, clr);
		safeDrawPoint(bitmap, {S.x+y, S.y+x}, clr);
		safeDrawPoint(bitmap, {S.x-y, S.y+x}, clr);
		safeDrawPoint(bitmap, {S.x-x, S.y+y}, clr);
		safeDrawPoint(bitmap, {S.x-x, S.y-y}, clr);
		safeDrawPoint(bitmap, {S.x-y, S.y-x}, clr);
		safeDrawPoint(bitmap, {S.x+y, S.y-x}, clr);
		safeDrawPoint(bitmap, {S.x+x, S.y-y}, clr);

		if(err <= 0){
			++y;
			err += dy;
			dy += 2;
		}
		if(err > 0){
			--x;
			dx += 2;
			err += dx - (r << 1);
		}
	}
}

//************************************************************

// Kolo.
// https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
void drawDisk(JiMP2::BMP &bitmap, Point S, uint16_t r, Colour clr){
	int dx = 1,
	    dy = 1;
	int x = r-1,
	    y = 0,
	    err = dx - (r << 1);
	
	while(x >= y){
		drawLine(bitmap, {S.x-y, S.y-x}, {S.x+y, S.y-x}, clr);
		drawLine(bitmap, {S.x-y, S.y+x}, {S.x+y, S.y+x}, clr);
		drawLine(bitmap, {S.x-x, S.y-y}, {S.x+x, S.y-y}, clr);
		drawLine(bitmap, {S.x-x, S.y+y}, {S.x+x, S.y+y}, clr);

		if(err <= 0){
			++y;
			err += dy;
			dy += 2;
		}
		if(err > 0){
			--x;
			dx += 2;
			err += dx - (r << 1);
		}
	}
}

//************************************************************

// Wycinek okregu - luk.
void drawArc(JiMP2::BMP &bitmap, Point S, uint16_t r,
	fp_t alfa1, fp_t alfa2, Colour clr){
	
	alfa1 = std::min(alfa1, alfa2);
	alfa2 = std::max(alfa1, alfa2);
	fp_t a1 = deg2Rad(alfa1),
	     a2 = deg2Rad(alfa2);

	int dx=1, dy=1;
	int x = r-1,
	    y = 0,
		err = dx - (r << 1);

	struct{
		int y_min = {0}, y_max = {0};
	} q1, q2, q3, q4;
	
	// I cwiartka.
	if(isInFirstQuarter(alfa1, alfa2)){
		q1.y_max = S.y - r*sin(a1);
		q1.y_min = (alfa2 < 90.0f) ? (S.y - r*sin(a2)) : (S.y - r);
	}
	// II cwiartka.
	if(isInSecondQuarter(alfa1, alfa2)){
		q2.y_max = (alfa2 < 180.0f) ? (S.y - r*sin(a2)) : S.y;
		q2.y_min = (alfa1 > 90.0f) ? (S.y - r*sin(a1)) : (S.y - r);
	}
	// III cwiartka.
	if(isInThirdQuarter(alfa1, alfa2)){
		q3.y_max = (alfa2 < 270.0f) ? (S.y - r*sin(a2)) : (S.y + r);
		q3.y_min = (alfa1 > 180.0f) ? (S.y - r*sin(a1)) : S.y;
	}
	// IV cwiartka.
	if(isInFourthQuarter(alfa1, alfa2)){
		q4.y_max = (alfa1 > 270.0f) ? (S.y - r*sin(a1)) : (S.y + r);
		q4.y_min = S.y - r*sin(a2);
	}

	while(x >= y){
		// I
		if(in(q1.y_min, uint16_t(S.y-x), q1.y_max))
			safeDrawPoint(bitmap, {S.x+y, S.y-x}, clr);
		if(in(q1.y_min, uint16_t(S.y-y), q1.y_max))
			safeDrawPoint(bitmap, {S.x+x, S.y-y}, clr);

		// II
		if(in(q2.y_min, uint16_t(S.y-y), q2.y_max))
			safeDrawPoint(bitmap, {S.x-x, S.y-y}, clr);
		if(in(q2.y_min, uint16_t(S.y-x), q2.y_max))
			safeDrawPoint(bitmap, {S.x-y, S.y-x}, clr);

		// III
		if(in(q3.y_min, uint16_t(S.y+x), q3.y_max))
			safeDrawPoint(bitmap, {S.x-y, S.y+x}, clr);
		if(in(q3.y_min, uint16_t(S.y+y), q3.y_max))
			safeDrawPoint(bitmap, {S.x-x, S.y+y}, clr);

		// IV
		if(in(q4.y_min, uint16_t(S.y+y), q4.y_max))
			safeDrawPoint(bitmap, {S.x+x, S.y+y}, clr);
		if(in(q4.y_min, uint16_t(S.y+x), q4.y_max))
			safeDrawPoint(bitmap, {S.x+y, S.y+x}, clr);

		if(err <= 0){
			++y;
			err += dy;
			dy += 2;
		}
		if(err > 0){
			--x;
			dx += 2;
			err += dx - (r << 1);
		}
	}
}

//************************************************************

// Wycinek kola.
void drawCircularSector(JiMP2::BMP &bitmap, Point S, uint16_t r,
	fp_t alfa1, fp_t alfa2, Colour clr){
	
	alfa1 = std::min(alfa1, alfa2);
	alfa2 = std::max(alfa1, alfa2);
	fp_t a1 = deg2Rad(alfa1);
	fp_t a2 = deg2Rad(alfa2);

	#ifdef DEBUG
	safeDrawPoint(bitmap, S, {0xff, 0, 0});
	drawCircle(bitmap, S, r, {0xaa, 0xaa, 0xaa});
	#endif

	int y;

	if(isInFirstQuarter(alfa1, alfa2)){
		Point U(S.x + r*cos(a1), S.y - r*sin(a1));
		Point V((alfa2 < 90.0f) ? (S.x + r*cos(a2)) : S.x,
		        (alfa2 < 90.0f) ? (S.y - r*sin(a2)) : (S.y - r));
		// V.x < U.x; V.y < U.y
		
		for(y = V.y; y <= U.y; ++y){
			Point P(linearInterpolation(S, V, y), y);
			Point Q(S.x + sqrt(r*r - squared(y-S.y)), y);
			drawLine(bitmap, P, Q, clr);
		}

		for(; y <= S.y; ++y){
			Point P(linearInterpolation(S, V, y), y);
			Point R(linearInterpolation(S, U, y), y);
			drawLine(bitmap, P, R, clr);
		}
	}
	if(isInSecondQuarter(alfa1, alfa2)){
		Point U(
			(alfa1 >= 90.0f) ? (S.x + r*cos(a1)) : S.x,
			(alfa1 >= 90.0f) ? (S.y - r*sin(a1)) : (S.y-r)
		);
		Point V(
			(alfa2 < 180.0f) ? (S.x + r*cos(a2)) : (S.x-r),
			(alfa2 < 180.0f) ? (S.x - r*sin(a2)) : S.y
		);
		// V.x < U.x; V.y > U.y

		for(y = U.y; y <= V.y; ++y){
			Point P(linearInterpolation(U, S, y), y);
			Point Q(S.x - sqrt(r*r - squared(y-S.y)), y);
			drawLine(bitmap, P, Q, clr);
		}

		for(; y <= S.y; ++y){
			Point P(linearInterpolation(S, V, y), y);
			Point R(linearInterpolation(S, U, y), y);
			drawLine(bitmap, P, R, clr);
		}
	}
	if(isInThirdQuarter(alfa1, alfa2)){
		Point U(
			(alfa1 >= 180.0f) ? (S.x + r*cos(a1)) : (S.x-r),
			(alfa1 >= 180.0f) ? (S.y - r*sin(a1)) : S.y
		);
		Point V(
			(alfa2 < 270.0f) ? (S.x + r*cos(a2)) : S.x,
			(alfa2 < 270.0f) ? (S.y - r*sin(a2)) : (S.y+r)
		);
		// V.x > U.x; V.y > U.y

		for(y = V.y; y >= U.y; --y){
			Point P(linearInterpolation(V, S, y), y);
			Point Q(S.x - sqrt(r*r - squared(y-S.y)), y);
			drawLine(bitmap, P, Q, clr);
		}

		for(; y >= S.y; --y){
			Point P(linearInterpolation(V, S, y), y);
			Point R(linearInterpolation(U, S, y), y);
			drawLine(bitmap, P, R, clr);
		}
	}
	if(isInFourthQuarter(alfa1, alfa2)){
		Point U(
			(alfa1 >= 270.0f) ? (S.x + r*cos(a1)) : S.x,
			(alfa1 >= 270.0f) ? (S.y - r*sin(a1)) : (S.y+r)
		);
		Point V(
			S.x + r*cos(a2),
			S.y - r*sin(a2)
		);
		// V.x > U.x; U.y > V.y

		for(y = U.y; y >= V.y; --y){
			Point P(linearInterpolation(U, S, y), y);
			Point Q(S.x + sqrt(r*r - squared(y-S.y)), y);
			drawLine(bitmap, P, Q, clr);
		}

		for(; y >= S.y; --y){
			Point P(linearInterpolation(U, S, y), y);
			Point R(linearInterpolation(V, S, y), y);
			drawLine(bitmap, P, R, clr);
		}
	}
}

//************************************************************

void drawEllipse(JiMP2::BMP &bitmap, Point S, uint16_t a, uint16_t b, Colour clr){
	int x=0,
	    y=b;
	int p = squared(b) - squared(a)*b + squared(a)/4;

	while(x*squared(b) < y*squared(a)){
		safeDrawPoint(bitmap, {S.x+x, S.y-y}, clr);
		safeDrawPoint(bitmap, {S.x-x, S.y+y}, clr);
		safeDrawPoint(bitmap, {S.x+x, S.y+y}, clr);
		safeDrawPoint(bitmap, {S.x-x, S.y-y}, clr);

		++x;
		if(p<0)
			p += squared(b)*(2*x + 1);
		else{
			--y;
			p += squared(b)*(2*x + 1) - 2*y*squared(a);
		}
	}
	p = squared((x + 0.5) * b) + squared((y - 1) * a) - squared(a*b);
	while(y >= 0){
		safeDrawPoint(bitmap, {S.x+x, S.y-y}, clr);
		safeDrawPoint(bitmap, {S.x-x, S.y+y}, clr);
		safeDrawPoint(bitmap, {S.x+x, S.y+y}, clr);
		safeDrawPoint(bitmap, {S.x-x, S.y-y}, clr);

		--y;
		if(p > 0)
			p -= squared(a) * (2*y + 1);
		else{
			++x;
			p += 2*x*squared(b) - squared(a)*(2*y + 1);
		}
	}
}

//************************************************************

// Prostokat.
// Naiwna implementacja.
void drawRectangle(JiMP2::BMP &bitmap, Point A, Point B, Colour clr){
	drawLine(bitmap, A, {B.x, A.y}, clr);
	drawLine(bitmap, A, {A.x, B.y}, clr);
	drawLine(bitmap, {B.x, A.y}, B, clr);
	drawLine(bitmap, {A.x, B.y}, B, clr);
}

//************************************************************
