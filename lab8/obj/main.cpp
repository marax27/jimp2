#include <iostream>
#include <sstream>
#include <fstream>

#include "bmp.h"
#include "objreader.h"

int stringToInt(const std::string &str){
	int x;
	std::istringstream ss(str);
	ss >> x;
	return x;
}

int main(int argc, char *argv[]){
	if(argc != 5){
		std::cout << "Usage: " << argv[0] << " obj-filename bmp-filename width height\n";
		return 1;
	}

	Obj model;
	std::string in_filename = argv[1],
	            out_filename = argv[2];
	
	int w = stringToInt(argv[3]), h = stringToInt(argv[4]);
	
	// Read .OBJ file and build Obj object.
	try{
		ObjReader().readObjFile(model, in_filename);
	}catch(ObjReader::ObjParseException &exc){
		std::cerr << "Failed to read '" << in_filename << "':\n\t"
		          << exc.what() << "\n";
		return 2;
	}

	std::cout << "Processing done\n\t"
	          << model.numberOfVertices() << " vertices read\n\t"
	          << model.numberOfFaces() << " faces read\n\n";

	JiMP2::BMP bitmap(w, h);
	for(uint16_t y = 0; y != h; ++y){
		for(uint16_t x = 0; x != w; ++x){
			unsigned char g = x / (float)w * 0xff;
			bitmap.setPixel(x, y, g, g, g);
		}
	}
	std::ofstream writer("test-bitmap.bmp", std::ios::binary);
	writer << bitmap;
	writer.close();
}
