#include <iostream>
#include <fstream>
#include <sstream>
#include "drawer.h"
#include "interpreter.h"

const char *OUTPUT_FILE = "out.bmp";

//************************************************************

void reportError(const char *filename, int line_number, const char *err);

template<typename T>
	bool convertString(const std::string &s, T &var){
		std::stringstream ss{s};
		ss >> var;
		return !ss.fail();
	}

//************************************************************

int main(int argc, char *argv[]){
	if(argc != 4){
		std::cerr << "Usage: " << argv[0]
		          << " instruction-file width height\n";
		return 1;
	}

	uint16_t w, h;
	if( !convertString(argv[2], w) || !convertString(argv[3], h)){
		std::cerr << "Invalid bitmap size provided.\n";
		return 2;
	}

	JiMP2::BMP bitmap(w, h);
	DrawerImplementation di(bitmap);
	Interpreter interpreter;
	
	try{
		interpreter.analyze(argv[1], di);
	}
	catch(Interpreter::InterpreterException &exc){
		// Handle any Interpreter exception.
		std::cerr << exc.what() << '\n';
		return 3;
	}

	std::ofstream writer(OUTPUT_FILE, std::ios::binary);
	writer << bitmap;
	writer.close();
	return 0;
}

//************************************************************
