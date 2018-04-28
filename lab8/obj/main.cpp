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
	
	JiMP2::BMP bitmapxy(w, h);
	bitmapxy.projectObjXY(model, 0xaa, 0, 0);
	std::ofstream writerxy("xy-" + out_filename, std::ios::binary);
	writerxy << bitmapxy;
	writerxy.close();

	JiMP2::BMP bitmapxz(w, h);
	bitmapxz.projectObjXZ(model, 0, 0xaa, 0);
	std::ofstream writerxz("xz-" + out_filename, std::ios::binary);
	writerxz << bitmapxz;
	writerxz.close();

	JiMP2::BMP bitmapyz(w, h);
	bitmapyz.projectObjYZ(model, 0, 0, 0xaa);
	std::ofstream writeryz("yz-" + out_filename, std::ios::binary);
	writeryz << bitmapyz;
	writeryz.close();

	return 0;
}
