#include <iostream>
#include <fstream>
#include "drawer.h"
#include "interpreter.h"

int main(int argc, char *argv[]){
	uint16_t w = 500,
	         h = 200;
	JiMP2::BMP bitmap(w, h);
	DrawerImplementation di(bitmap);

	/*for(int x = 0; x < w; x += 5)
		di.drawPoint({x, h/2}, {128, 0, 0});

	di.drawLine({0, h}, {w, 0}, {0, 0, 128});

	di.drawCircle({w/2, h/2}, 150, {0, 128, 0});
	di.drawDisk({w/2, h/2}, 100, {0, 200, 200});
	di.drawArc({w/2, h/2}, 175, 180, 270, {100, 100, 100});
	di.drawCircularSector({w/2, h/2}, 80, 30, 90, {0xff, 0, 0});

	di.drawRectangle({25, 25}, {w-25, h-25}, {160, 160, 160});
	di.drawEllipse({w/2, h/2}, 200, 75, {192, 192, 0});
	di.drawRegularPolygon({50, 50}, 8, 15, {0, 120, 0xff});*/

	if(argc != 3){
		std::cerr << "Usage: " << argv[0] << " instruction-file output-bmp\n";
		return 1;
	}

	Interpreter interpreter;
	
	interpreter.analyze(argv[1], di);

	std::ofstream writer(argv[2], std::ios::binary);
	writer << bitmap;
	writer.close();
	return 0;
}