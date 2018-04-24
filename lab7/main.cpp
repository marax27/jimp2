#include <iostream>
#include <fstream>
#include "drawer.h"

int main(){
	uint16_t w = 512,
	         h = 256;
	JiMP2::BMP bitmap(w, h);
	DrawerImplementation di(bitmap);

	for(int x = 0; x < w; x += 5)
		di.drawPoint({x, h/2}, {128, 0, 0});

	di.drawLine({0, h}, {w, 0}, {0, 0, 128});

	di.drawCircle({w/2, h/2}, 150, {0, 128, 0});
	di.drawDisk({w/2, h/2}, 100, {0, 200, 200});
	di.drawArc({w/2, h/2}, 175, 180, 270, {100, 100, 100});
	di.drawCircularSector({w/2, h/2}, 80, 30, 90, {0xff, 0, 0});

	di.drawRectangle({25, 25}, {w-25, h-25}, {160, 160, 160});
	di.drawEllipse({w/2, h/2}, 200, 75, {192, 192, 0});
	di.drawRegularPolygon({50, 50}, 8, 15, {0, 120, 0xff});

	std::ofstream writer("test1.bmp", std::ios::binary);
	writer << bitmap;
	writer.close();
	return 0;
}