#include <iostream>
#include <fstream>
#include <sstream>
#include "drawer.h"
#include "interpreter.h"

template<typename T>
	bool convertString(const std::string &s, T &var){
		std::stringstream ss{s};
		ss >> var;
		return !ss.fail();
	}

int main(int argc, char *argv[]){
	if(argc != 5){
		std::cerr << "Usage: " << argv[0]
		          << " instruction-file output-bmp width height\n";
		return 1;
	}

	uint16_t w, h;
	if( !convertString(argv[3], w) || !convertString(argv[4], h)){
		std::cerr << "Invalid bitmap size provided.\n";
		return 2;
	}

	JiMP2::BMP bitmap(w, h);
	DrawerImplementation di(bitmap);
	Interpreter interpreter;
	
	interpreter.analyze(argv[1], di);

	std::ofstream writer(argv[2], std::ios::binary);
	writer << bitmap;
	writer.close();
	return 0;
}