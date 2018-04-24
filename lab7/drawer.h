#ifndef _DRAWER_H_
#define _DRAWER_H_

#include "bmp.h"
#include "misc.h"

//************************************************************

// Interfejs dla metod rysujacych.
class Drawer{
public:
	Drawer() : bitmap(nullptr) {}
	Drawer(JiMP2::BMP &bmp) : bitmap(&bmp) {}
	virtual ~Drawer(){ bitmap = nullptr; }

	virtual void drawPoint(Point P, Colour clr) = 0;
	virtual void drawLine(Point A, Point B, Colour clr) = 0;
	virtual void drawCircle(Point S, uint16_t r, Colour clr) = 0;
	virtual void drawDisk(Point S, uint16_t r, Colour clr) = 0;
	virtual void drawArc(Point S, uint16_t r,
		fp_t alfa1, fp_t alfa2, Colour clr) = 0;
	virtual void drawCircularSector(Point S, uint16_t r,
		fp_t alfa1, fp_t alfa2, Colour clr) = 0;
	virtual void drawEllipse(Point S, uint16_t a, uint16_t b, Colour clr) = 0;
	virtual void drawRectangle(Point A, Point B, Colour clr) = 0;
	virtual void drawRegularPolygon(Point S, int n, fp_t side_length, Colour clr) = 0;

	void setBitmap(JiMP2::BMP &bmp){ bitmap = &bmp; }

	class DrawingException : public std::exception {};

protected:
	JiMP2::BMP *bitmap;

};

//************************************************************

class DrawerImplementation : public Drawer{
public:
	DrawerImplementation() : Drawer() {}
	DrawerImplementation(JiMP2::BMP &bmp) : Drawer(bmp) {}

	virtual void drawPoint(Point P, Colour clr);
	virtual void drawLine(Point A, Point B, Colour clr);
	virtual void drawCircle(Point S, uint16_t r, Colour clr);
	virtual void drawDisk(Point S, uint16_t r, Colour clr);
	virtual void drawArc(Point S, uint16_t r,
		fp_t alfa1, fp_t alfa2, Colour clr);
	virtual void drawCircularSector(Point S, uint16_t r,
		fp_t alfa1, fp_t alfa2, Colour clr);
	virtual void drawEllipse(Point S, uint16_t a, uint16_t b, Colour clr);
	virtual void drawRectangle(Point A, Point B, Colour clr);
	virtual void drawRegularPolygon(Point S, int n, fp_t side_length, Colour clr);
};

//************************************************************

#endif  //_DRAWER_H_