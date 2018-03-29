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

// Sprawdza, czy x nalezy do przedzialu [min, max].
template<typename T> bool in(T min, T x, T max){
	return min <= x && x <= max;
}

// Ogranicza wartosc x do wartosci z przedzialu [min, max].
template<typename T> void limitTo(int min, T &x, int max){
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

void safeDrawPoint(JiMP2::BMP &bitmap, Point P, Colour clr);
void drawLine(JiMP2::BMP &bitmap, Point A, Point B, Colour clr);
void drawCircle(JiMP2::BMP &bitmap, Point S, uint16_t r, Colour clr);
void drawDisk(JiMP2::BMP &bitmap, Point S, uint16_t r, Colour clr);
void drawArc(JiMP2::BMP &bitmap, Point S, uint16_t r,
	fp_t alfa1, fp_t alfa2, Colour clr);
void drawCircularSector(JiMP2::BMP &bitmap, Point S, uint16_t r,
	fp_t alfa1, fp_t alfa2, Colour clr);
void drawEllipse(JiMP2::BMP &bitmap, Point S, uint16_t a, uint16_t b, Colour clr);

//************************************************************

const uint16_t imgWidth = 800;
const uint16_t imgHeight = 600;

void lineTest(){
	JiMP2::BMP bmp(imgWidth, imgHeight);

	const int A = 325;
	const int D = 25;
	const int S = 375;
	const Colour clr{0, 0, 128};
	const Colour clr1{192, 32, 32};

	drawLine(bmp, {A, A}, {A, A+S}, clr);
	drawLine(bmp, {A, A}, {A+D, A+S}, clr);
	drawLine(bmp, {A, A}, {A+3*D, A+S}, clr);
	drawLine(bmp, {A, A}, {A+5*D, A+S}, clr);
	drawLine(bmp, {A, A}, {A+11*D, A+S}, clr);
	drawLine(bmp, {A, A}, {A+15*D, A+S/2}, clr);
	drawLine(bmp, {A, A}, {A+15*D, A+S/8}, clr);
	drawLine(bmp, {A, A}, {A+15*D, A}, clr);

	drawLine(bmp, {A, A}, {A+15*D, A-S/4}, clr);
	drawLine(bmp, {A, A}, {A+15*D, A-S}, clr);
	drawLine(bmp, {A, A}, {A+11*D, A-S}, clr);
	drawLine(bmp, {A, A}, {A+5*D, A-S}, clr);
	drawLine(bmp, {A, A}, {A+3*D, A-S}, clr);
	drawLine(bmp, {A, A}, {A, A-S}, clr);

	drawLine(bmp, {A, A}, {A-3*D, A-S}, clr);
	drawLine(bmp, {A, A}, {A-S, A-S}, clr);
	drawLine(bmp, {A, A}, {A-S, A}, clr);
	
	drawLine(bmp, {A, A}, {A-S, A+S}, clr);
	drawLine(bmp, {A, A}, {A-S, A+S/2}, clr);
	drawLine(bmp, {A, A}, {A-S, A+S/4}, clr);
	drawLine(bmp, {A, A}, {A-3*D, A+S}, clr);
	drawLine(bmp, {A, A}, {A-D, A+S}, clr);

	drawLine(bmp, {-50, 50}, {bmp.getWidth()+50, 50}, clr1);
	drawLine(bmp, {50, -50}, {50, bmp.getHeight()+50}, clr1);
	drawLine(bmp, {-30, -50}, {bmp.getWidth()-30, bmp.getHeight()+50}, clr1);

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

	std::ofstream writer("circledisk.bmp", std::ofstream::binary);
	writer << bmp;
}

void arcTest(){
	const uint16_t w = 800, h = 600;
	JiMP2::BMP bmp(w, h);
	Colour black{0, 0, 0};

	int r = 50;
	for(int i=0; i<6; ++i)
		drawArc(bmp, {50+100*i, 50}, r, 30*i, 360-30*i, black);

	for(int i=0; i<4; ++i)
		drawArc(bmp, {50, 150}, r, 30+90*i, 60+90*i, black);

	for(int i=0; i<6; ++i)
		drawArc(bmp, {150+100*i, 150}, r, 0, 30*(1+i), black);

	std::ofstream writer("arc.bmp", std::ofstream::binary);
	writer << bmp;
}

void run(){
	// Linie.
	lineTest();

	// Okrag i kolo.
	circleDiskTest();

	// Luk.
	arcTest();

	// Wycinek kola.
	//drawCircularSector(bmp, {450, 300}, 75, 30, 330, {255, 60, 60});

	// Elipsa.
	//drawEllipse(bmp, {400, 200}, 150, 120, {50, 50, 50});
}

//************************************************************

void safeDrawPoint(JiMP2::BMP &bitmap, Point P, Colour clr){
	if(in(0, P.x, bitmap.getWidth()-1) && in(0, P.y, bitmap.getHeight()-1))
		bitmap.setPixel(P.x, P.y, clr.r, clr.g, clr.b);
}

//************************************************************

// Linia.
// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void drawLine(JiMP2::BMP &bitmap, Point A, Point B, Colour clr){
	if(A.x != B.x){
		// Non-vertical line.
		fp_t delta_x = B.x - A.x,
		     delta_y = B.y - A.y;
		fp_t delta_err = fabs(delta_y / delta_x);
		fp_t err = 0.0;
		int y = A.y;
		int left_x = std::min(A.x, B.x),
		    right_x = std::max(A.x, B.x);

		for(int x = left_x; x <= right_x; ++x){
			safeDrawPoint(bitmap, {x, y}, clr);
			err += delta_err;
			while(err >= 0.5){
				y += delta_y ? (delta_y > 0.0 ? 1 : -1) : 0;
				err -= 1.0;
			}
		}
	}
	else{
		// Linia pionowa.
		int top_y = std::min(A.y, B.y),
		    bottom_y = std::max(A.y, B.y);
		
		// Sprawdz, czy zakres y nie wykracza poza bitmape.
		limitTo(0, top_y, bitmap.getHeight());
		limitTo(0, bottom_y, bitmap.getHeight());

		for(uint16_t y = top_y; y <= bottom_y; ++y)
			bitmap.setPixel(A.x, y, clr.r, clr.g, clr.b);
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
	
	// I cwiartka.
	if(alfa1 < 90.0){
		uint16_t y_max = S.y - r*sin(a1),
		         y_min = (alfa2 < 90.0) ? (S.y - r*sin(a2)) : (S.y - r);

		while(x >= y){
			if(in(y_min, uint16_t(S.y-x), y_max))
				bitmap.setPixel(S.x+y, S.y-x, clr.r, clr.g, clr.b);
			if(in(y_min, uint16_t(S.y-y), y_max))
				bitmap.setPixel(S.x+x, S.y-y, clr.r, clr.g, clr.b);
			
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
}

// Naiwna implementacja.
/*void drawArc(JiMP2::BMP &bitmap, Point S, uint16_t r,
	fp_t alfa1, fp_t alfa2, Colour clr){
	
	alfa1 = deg2Rad(alfa1);
	alfa2 = deg2Rad(alfa2);

	fp_t step = acos(1 - 0.5*squared(0.99/r));
	for(fp_t angle = alfa1; angle < alfa2; angle += step)
		bitmap.setPixel(S.x + r*cos(angle), S.y + r*sin(angle), clr.r, clr.g, clr.b);
}*/

//************************************************************

// Wycinek kola.
// Naiwna implementacja.
void drawCircularSector(JiMP2::BMP &bitmap, Point S, uint16_t r,
	fp_t alfa1, fp_t alfa2, Colour clr){
	
	alfa1 = std::min(alfa1, alfa2);
	alfa2 = std::max(alfa1, alfa2);
	fp_t a1 = deg2Rad(alfa1);
	fp_t a2 = deg2Rad(alfa2);

	Point P1(S.x + r*cos(a1), S.y - r*sin(a1));
	Point P2(S.x + r*cos(a2), S.y - r*sin(a2));

	drawLine(bitmap, S, P1, clr);
	drawLine(bitmap, S, P2, clr);
	drawArc(bitmap, S, r, alfa1, alfa2, clr);
}

//************************************************************

void drawEllipse(JiMP2::BMP &bitmap, Point S, uint16_t a, uint16_t b, Colour clr){
	int x=0,
	    y=b;
	int p = squared(b) - squared(a)*b + squared(a)/4;

	while(x*squared(b) < y*squared(a)){
		bitmap.setPixel(S.x+x, S.y-y, clr.r, clr.g, clr.b);
		bitmap.setPixel(S.x-x, S.y+y, clr.r, clr.g, clr.b);
		bitmap.setPixel(S.x+x, S.y+y, clr.r, clr.g, clr.b);
		bitmap.setPixel(S.x-x, S.y-y, clr.r, clr.g, clr.b);

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
		bitmap.setPixel(S.x+x, S.y-y, clr.r, clr.g, clr.b);
		bitmap.setPixel(S.x-x, S.y+y, clr.r, clr.g, clr.b);
		bitmap.setPixel(S.x+x, S.y+y, clr.r, clr.g, clr.b);
		bitmap.setPixel(S.x-x, S.y-y, clr.r, clr.g, clr.b);

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
