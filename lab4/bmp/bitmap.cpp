#include <iostream>
#include <fstream>
#include <cmath>
#include "bmp.h"

//************************************************************

// Floating-point type used in algorithms.
// In future, double may be necessary.
typedef float fp_t;

// Return squared argument.
fp_t squared(fp_t x){
	return x*x;
}

// Convert degrees to radians.
fp_t deg2Rad(fp_t degrees){
	return degrees * M_PI / 180.0;
}

// Representation of a 2D point.
struct Point{
	int x = {0},
	    y = {0};
	
	static fp_t distance(const Point &a, const Point &b){
		return sqrt( squared((int)a.x - b.x) + squared((int)a.y - b.y) );
	}
};

// Representation of RGB colour.
struct Colour{
	unsigned char r = {0},
	              g = {0},
	              b = {0};
};

//************************************************************

void drawLine(JiMP2::BMP &bitmap, Point A, Point B, Colour colour);
void drawCircle(JiMP2::BMP &bitmap, Point S, uint16_t r, Colour colour);
void drawDisk(JiMP2::BMP &bitmap, Point S, uint16_t r, Colour colour);
void drawArc(JiMP2::BMP &bitmap, Point S, uint16_t r,
	fp_t alfa1, fp_t alfa2, Colour colour);


//************************************************************

void run(){
	const uint16_t imgWidth = 800;
	const uint16_t imgHeight = 600;

	JiMP2::BMP bmp(imgWidth, imgHeight);

	// Line drawing.
	drawLine(bmp, {5, 5}, {795, 595}, {255, 0, 0});
	drawLine(bmp, {50, 200}, {50, 400}, {0, 120, 0});
	drawLine(bmp, {300, 55}, {25, 55}, {0, 0, 255});
	drawLine(bmp, {200, 200}, {650, 500}, {80, 80, 80});

	// Circle drawing.
	drawCircle(bmp, {400, 400}, 200, {128, 0, 255});

	// Disk drawing.
	drawDisk(bmp, {400, 400}, 80, {60, 60, 60});

	// Arc drawing.
	drawArc(bmp, {550, 120}, 75, 30, 330, {60, 60, 60});

	std::ofstream outfile("test.bmp", std::ofstream::binary);
	outfile << bmp;
}

//************************************************************

// Linia.
// Implementation of Bresenham's algorithm.
// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void drawLine(JiMP2::BMP &bitmap, Point A, Point B, Colour colour){
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
			bitmap.setPixel(x, y, colour.r, colour.g, colour.b);
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
			bitmap.setPixel(A.x, y, colour.r, colour.g, colour.b);
	}
}

//************************************************************

// Okrag.
// Implementation of midpoint circle algorithm.
// https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
void drawCircle(JiMP2::BMP &bitmap, Point S, uint16_t r, Colour colour){
	int dx = 1,
	    dy = 1;
	int x = r-1,
	    y = 0,
	    err = dx - (r << 1);
	
	while(x >= y){
		bitmap.setPixel(S.x+x, S.y+y, colour.r, colour.g, colour.b);
		bitmap.setPixel(S.x+y, S.y+x, colour.r, colour.g, colour.b);
		bitmap.setPixel(S.x-y, S.y+x, colour.r, colour.g, colour.b);
		bitmap.setPixel(S.x-x, S.y+y, colour.r, colour.g, colour.b);
		bitmap.setPixel(S.x-x, S.y-y, colour.r, colour.g, colour.b);
		bitmap.setPixel(S.x-y, S.y-x, colour.r, colour.g, colour.b);
		bitmap.setPixel(S.x+y, S.y-x, colour.r, colour.g, colour.b);
		bitmap.setPixel(S.x+x, S.y-y, colour.r, colour.g, colour.b);

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
// Variation of midpoint circle algorithm.
// https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
void drawDisk(JiMP2::BMP &bitmap, Point S, uint16_t r, Colour colour){
	int dx = 1,
	    dy = 1;
	int x = r-1,
	    y = 0,
	    err = dx - (r << 1);
	
	while(x >= y){
		drawLine(bitmap, {S.x-y, S.y-x}, {S.x+y, S.y-x}, colour);
		drawLine(bitmap, {S.x-y, S.y+x}, {S.x+y, S.y+x}, colour);
		drawLine(bitmap, {S.x-x, S.y-y}, {S.x+x, S.y-y}, colour);
		drawLine(bitmap, {S.x-x, S.y+y}, {S.x+x, S.y+y}, colour);

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
// Naiwna implementacja.
void drawArc(JiMP2::BMP &bitmap, Point S, uint16_t r,
	fp_t alfa1, fp_t alfa2, Colour colour){
	
	alfa1 = deg2Rad(alfa1);
	alfa2 = deg2Rad(alfa2);

	fp_t step = acos(1 - 0.5*squared(0.99/r));
	for(fp_t angle = alfa1; angle < alfa2; angle += step)
		bitmap.setPixel(S.x + r*cos(angle), S.y + r*sin(angle), colour.r, colour.g, colour.b);
}

