#include "drawer.h"

//************************************************************

void DrawerImplementation::drawPoint(Point P, Colour clr){
	// If a point lies outside a bitmap, draw nothing.
	if(P.x < 0 || P.y < 0 ||
	   P.x >= bitmap->getWidth() || P.y >= bitmap->getHeight())
		return;

	bitmap->setPixel(P.x, P.y, clr.r, clr.g, clr.b);
}

//************************************************************

// Linia.
// W oparciu o algorytm Bresenhama.
// https://gist.github.com/bert/1085538
void DrawerImplementation::drawLine(Point A, Point B, Colour clr){
	int dx =  abs (B.x - A.x), sx = A.x < B.x ? 1 : -1,
	    dy = -abs (B.y - A.y), sy = A.y < B.y ? 1 : -1; 
	int err = dx + dy,
	    e2;
	
	while(true){
		drawPoint(A, clr);
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
void DrawerImplementation::drawCircle(Point S, uint16_t r, Colour clr){
	int dx = 1,
	    dy = 1;
	int x = r-1,
	    y = 0,
	    err = dx - (r << 1);
	
	while(x >= y){
		drawPoint({S.x+x, S.y+y}, clr);
		drawPoint({S.x+y, S.y+x}, clr);
		drawPoint({S.x-y, S.y+x}, clr);
		drawPoint({S.x-x, S.y+y}, clr);
		drawPoint({S.x-x, S.y-y}, clr);
		drawPoint({S.x-y, S.y-x}, clr);
		drawPoint({S.x+y, S.y-x}, clr);
		drawPoint({S.x+x, S.y-y}, clr);

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
void DrawerImplementation::drawDisk(Point S, uint16_t r, Colour clr){
	int dx = 1,
	    dy = 1;
	int x = r-1,
	    y = 0,
	    err = dx - (r << 1);
	
	while(x >= y){
		drawLine({S.x-y, S.y-x}, {S.x+y, S.y-x}, clr);
		drawLine({S.x-y, S.y+x}, {S.x+y, S.y+x}, clr);
		drawLine({S.x-x, S.y-y}, {S.x+x, S.y-y}, clr);
		drawLine({S.x-x, S.y+y}, {S.x+x, S.y+y}, clr);

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
void DrawerImplementation::drawArc(Point S, uint16_t r,
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
			drawPoint({S.x+y, S.y-x}, clr);
		if(in(q1.y_min, uint16_t(S.y-y), q1.y_max))
			drawPoint({S.x+x, S.y-y}, clr);

		// II
		if(in(q2.y_min, uint16_t(S.y-y), q2.y_max))
			drawPoint({S.x-x, S.y-y}, clr);
		if(in(q2.y_min, uint16_t(S.y-x), q2.y_max))
			drawPoint({S.x-y, S.y-x}, clr);

		// III
		if(in(q3.y_min, uint16_t(S.y+x), q3.y_max))
			drawPoint({S.x-y, S.y+x}, clr);
		if(in(q3.y_min, uint16_t(S.y+y), q3.y_max))
			drawPoint({S.x-x, S.y+y}, clr);

		// IV
		if(in(q4.y_min, uint16_t(S.y+y), q4.y_max))
			drawPoint({S.x+x, S.y+y}, clr);
		if(in(q4.y_min, uint16_t(S.y+x), q4.y_max))
			drawPoint({S.x+y, S.y+x}, clr);

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
void DrawerImplementation::drawCircularSector(Point S, uint16_t r,
	fp_t alfa1, fp_t alfa2, Colour clr){

	alfa1 = std::min(alfa1, alfa2);
	alfa2 = std::max(alfa1, alfa2);
	fp_t a1 = deg2Rad(alfa1);
	fp_t a2 = deg2Rad(alfa2);
	int y;

	if(isInFirstQuarter(alfa1, alfa2)){
		Point U(S.x + r*cos(a1), S.y - r*sin(a1));
		Point V((alfa2 < 90.0f) ? (S.x + r*cos(a2)) : S.x,
		        (alfa2 < 90.0f) ? (S.y - r*sin(a2)) : (S.y - r));
		// V.x < U.x; V.y < U.y
		
		for(y = V.y; y <= U.y; ++y){
			Point P(linearInterpolation(S, V, y), y);
			Point Q(S.x + sqrt(r*r - squared(y-S.y)), y);
			drawLine(P, Q, clr);
		}

		for(; y <= S.y; ++y){
			Point P(linearInterpolation(S, V, y), y);
			Point R(linearInterpolation(S, U, y), y);
			drawLine(P, R, clr);
		}
	}
	if(isInSecondQuarter(alfa1, alfa2)){
		Point U(
			(alfa1 >= 90.0f) ? (S.x + r*cos(a1)) : S.x,
			(alfa1 >= 90.0f) ? (S.y - r*sin(a1)) : (S.y-r)
		);
		Point V(
			(alfa2 < 180.0f) ? (S.x + r*cos(a2)) : (S.x-r),
			(alfa2 < 180.0f) ? (S.y - r*sin(a2)) : S.y
		);
		// V.x < U.x; V.y > U.y

		for(y = U.y; y <= V.y; ++y){
			Point P(linearInterpolation(S, U, y), y);
			Point Q(S.x - sqrt(r*r - squared(y-S.y)), y);
			drawLine(P, Q, clr);
		}

		for(; y <= S.y; ++y){
			Point P(linearInterpolation(S, U, y), y);
			Point R(linearInterpolation(S, V, y), y);
			drawLine(P, R, clr);
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
			drawLine(P, Q, clr);
		}

		for(; y >= S.y; --y){
			Point P(linearInterpolation(V, S, y), y);
			Point R(linearInterpolation(U, S, y), y);
			drawLine(P, R, clr);
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
			drawLine(P, Q, clr);
		}

		for(; y >= S.y; --y){
			Point P(linearInterpolation(U, S, y), y);
			Point R(linearInterpolation(V, S, y), y);
			drawLine(P, R, clr);
		}
	}
}

//************************************************************

void DrawerImplementation::drawEllipse(Point S, uint16_t a, uint16_t b, Colour clr){
	int x=0,
	    y=b;
	int p = squared(b) - squared(a)*b + squared(a)/4;

	while(x*squared(b) < y*squared(a)){
		drawPoint({S.x+x, S.y-y}, clr);
		drawPoint({S.x-x, S.y+y}, clr);
		drawPoint({S.x+x, S.y+y}, clr);
		drawPoint({S.x-x, S.y-y}, clr);

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
		drawPoint({S.x+x, S.y-y}, clr);
		drawPoint({S.x-x, S.y+y}, clr);
		drawPoint({S.x+x, S.y+y}, clr);
		drawPoint({S.x-x, S.y-y}, clr);

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
void DrawerImplementation::drawRectangle(Point A, Point B, Colour clr){
	drawLine(A, {B.x, A.y}, clr);
	drawLine(A, {A.x, B.y}, clr);
	drawLine({B.x, A.y}, B, clr);
	drawLine({A.x, B.y}, B, clr);
}

//************************************************************

// Wielokat foremny.
// Naiwna implementacja.
void DrawerImplementation::drawRegularPolygon(Point S, int n, fp_t side_length, Colour clr){
	if(n < 3 || side_length <= 0.0)
		throw DrawingException();
	
	fp_t phi = 2*M_PI / n;
	fp_t radius = side_length / sqrt(2 * (1 - cos(phi)));  //z twierdzenia cosinusow

	Point p1(S.x, S.y - radius);
	for(int i = 1; i <= n; ++i){
		Point p2(
			S.x - radius*sin(i*phi), //cos(i*phi),
			S.y - radius*cos(i*phi) //sin(i*phi)
		);
		drawLine(p1, p2, clr);
		p1 = p2;
	}

}

//************************************************************
