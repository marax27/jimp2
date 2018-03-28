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

template<typename T> bool in(T min, T x, T max){
	return min <= x && x <= max;
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

void run(){
	const uint16_t imgWidth = 800;
	const uint16_t imgHeight = 600;

	JiMP2::BMP bmp(imgWidth, imgHeight);

	// Linie.
	drawLine(bmp, {5, 5}, {795, 595}, {255, 0, 0});
	drawLine(bmp, {50, 200}, {50, 400}, {0, 120, 0});
	drawLine(bmp, {300, 55}, {25, 55}, {0, 0, 255});
	drawLine(bmp, {200, 200}, {650, 500}, {80, 80, 80});

	// Okrag.
	drawCircle(bmp, {400, 400}, 200, {128, 0, 255});

	// Kolo.
	drawDisk(bmp, {400, 400}, 80, {60, 60, 60});

	// Luk.
	drawArc(bmp, {550, 120}, 75, 30, 330, {60, 60, 60});

	// Wycinek kola.
	drawCircularSector(bmp, {450, 300}, 75, 30, 330, {255, 60, 60});

	// Elipsa.
	drawEllipse(bmp, {400, 400}, 140, 70, {50, 50, 50});

	std::ofstream outfile("test.bmp", std::ofstream::binary);
	outfile << bmp;
}

void testWycinkow(){
	const uint16_t w = 800, h = 600;
	JiMP2::BMP bmp(w, h);
	const Colour black{0, 0, 0};

	int r = 50;
	drawArc(bmp, {50, 50}, r, 0, 360, black);
	drawArc(bmp, {150, 50}, r, 30, 330, black);
	drawArc(bmp, {250, 50}, r, 60, 300, black);
	drawArc(bmp, {350, 50}, r, 90, 270, black);
	drawArc(bmp, {450, 50}, r, 120, 240, black);
	drawArc(bmp, {550, 50}, r, 150, 210, black);

	drawArc(bmp, {50, 150}, r, 30, 60, black);
	drawArc(bmp, {50, 150}, r, 120, 150, black);
	drawArc(bmp, {50, 150}, r, 210, 240, black);
	drawArc(bmp, {50, 150}, r, 300, 330, black);

	drawArc(bmp, {150, 150}, r, 0, 30, black);
	drawArc(bmp, {250, 150}, r, 0, 60, black);
	drawArc(bmp, {350, 150}, r, 0, 90, black);
	drawArc(bmp, {450, 150}, r, 0, 120, black);
	drawArc(bmp, {550, 150}, r, 0, 150, black);
	drawArc(bmp, {650, 150}, r, 0, 180, black);

	std::ofstream writer("wycinki.bmp", std::ofstream::binary);
	writer << bmp;
}

//************************************************************

void safeDrawPoint(JiMP2::BMP &bitmap, Point P, Colour clr){
	if(in(0, P.x, bitmap.getWidth()-1) && in(0, P.y, bitmap.getHeight()-1))
		bitmap.setPixel(P.x, P.y, clr.r, clr.g, clr.b)
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
		uint16_t y = A.y;
		uint16_t left_x = A.x < B.x ? A.x : B.x,
		         right_x = A.x > B.x ? A.x : B.x;

		for(uint16_t x = left_x; x <= right_x; ++x){
			bitmap.setPixel(x, y, clr.r, clr.g, clr.b);
			err += delta_err;
			while(err >= 0.5){
				y += delta_y ? (delta_y > 0.0 ? 1 : -1) : 0;
				err -= 1.0;
			}
		}
	}
	else{
		// Vertical line.
		uint16_t top_y = A.y < B.y ? A.y : B.y,
		         bottom_y = A.y > B.y ? A.y : B.y;

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
		bitmap.setPixel(S.x+x, S.y+y, clr.r, clr.g, clr.b);
		bitmap.setPixel(S.x+y, S.y+x, clr.r, clr.g, clr.b);
		bitmap.setPixel(S.x-y, S.y+x, clr.r, clr.g, clr.b);
		bitmap.setPixel(S.x-x, S.y+y, clr.r, clr.g, clr.b);
		bitmap.setPixel(S.x-x, S.y-y, clr.r, clr.g, clr.b);
		bitmap.setPixel(S.x-y, S.y-x, clr.r, clr.g, clr.b);
		bitmap.setPixel(S.x+y, S.y-x, clr.r, clr.g, clr.b);
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
