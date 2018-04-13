#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <vector>
#include <unordered_map>

#include "misc.h"
#include "bmp.h"

//************************************************************

std::ostream& operator<<(std::ostream &out, const Obj &model){
	out << "# Wavefront OBJ file\n";

	// Write vertices.
	for(index_t i = 0; i != model.numberOfVertices(); ++i){
		const Point3 &p = model.getVertex(i);
		out << "v " 
		    << p.getX() << ' '
			<< p.getY() << ' '
		    << p.getZ() << '\n';
	}

	// Write faces.
	for(index_t i = 0; i != model.numberOfFaces(); ++i){
		const Triangle &t = model.getFace(i);
		out << "f "
		    << t.getVertexIndex(0) << ' '
		    << t.getVertexIndex(1) << ' '
		    << t.getVertexIndex(2) << '\n';
	}

	return out;
}

//************************************************************

class ObjReader{
public:
	bool readObjFile(Obj &object, const std::string &fname);

	class ObjParseException : std::exception {};

private:
	bool readVertex(const char *line);
	bool readFace(const char *line);
	bool readNotImplemented(const char*);

	Obj *instance;
};

//************************************************************

bool ObjReader::readObjFile(Obj &object, const std::string &fname){
	instance = &object;
	instance->clear();

	std::size_t line_number = 0;
	std::ifstream reader(fname, std::ios::binary);
	if(!reader)
		throw ObjParseException();
	
	std::unordered_map<
		std::string, bool(ObjReader::*)(const char*)> handlers({
		{ "v", &ObjReader::readVertex },
		{ "f", &ObjReader::readFace },
		{ "vn", &ObjReader::readNotImplemented },
		{ "vt", &ObjReader::readNotImplemented },
		{ "mtllib", &ObjReader::readNotImplemented },
		{ "usemtl", &ObjReader::readNotImplemented },      
		{ "s", &ObjReader::readNotImplemented },
		{ "g", &ObjReader::readNotImplemented },
		{ "o", &ObjReader::readNotImplemented }
	});
	
	while(reader.good()){
		++line_number;
		const int BUFSIZE = 256;
		char line[BUFSIZE];
		reader.getline(line, BUFSIZE, '\n');

		std::size_t len = strlen(line);

		// Handle CRLF format.
		if(line[len-1] == '\r')
			line[(len--)-1] = '\0';
		
		char token[16]{};
		if(sscanf(line, "%15s", token) != 1)
			continue;  //skip empty line

		if(token[0] == '#')
			continue;  //skip comments

		auto handler_itr = handlers.find(std::string(token));
		if(handler_itr == handlers.end()){
			std::cerr << "[Warning]: Unknown Obj feature: " << token << ".\n";
			continue;
		}

		if(!(this->*handler_itr->second)(line))
			throw ObjParseException();
		
		if(!reader.good())
			break;
	}

	reader.close();
	return true;
}

//************************************************************

bool ObjReader::readVertex(const char *line){
	// Format: 'v' x y z
	double x, y, z;
	if(sscanf(line, "v %lf %lf %lf", &x, &y, &z) != 3){
		std::cerr << "[Error] Unsupported vertex format: '" << line << "'\n";
		return false;
	}
	instance->appendVertex(Point3{x, y, z});
	return true;
}

//************************************************************

bool ObjReader::readFace(const char *line){
	// Possible formats:
	// f v ...
	// f v/vt ...
	// f v//vn ...
	// f v/vt/vn ...

	// Values 'vt' and 'vn' will be ignored.
	int v = 0, vt = 0, vn = 0;
	std::stringstream ss(line);
	std::string token;
	ss >> token;  // line identifier

	std::vector<int> vertex_indices;

	do{
		ss >> token;
		if(ss.eof())
			break;
		
		if(sscanf(token.c_str(), "%d/%d/%d", &v, &vt, &vn) != 3 &&
		   sscanf(token.c_str(), "%d//%d", &v, &vn) != 2 &&
		   sscanf(token.c_str(), "%d/%d", &v, &vt) != 2 &&
		   sscanf(token.c_str(), "%d", &v) != 1){
			   std::cerr << "[Error] Unsupported face format: '" << line << "'\n";
			   return false;
		   }
		
		// Indices in Obj file are 1-based. Correct them.
		vertex_indices.push_back(v - 1);
	}while(ss.good());

	// Only triangles and quads are allowed.
	if(vertex_indices.size() != 3 && vertex_indices.size() != 4){
		std::cerr << "[Warning] Unsupported number of face's vertices: "
		          << vertex_indices.size() << ", omitting...\n";
		return true;  //omit face without stopping a program
	}

	// std::cerr << "Try: " << vertex_indices[0] << ", " << vertex_indices[1] << ", " << vertex_indices[2] << "; " << vertices.size() << '\n';

	instance->appendTriangleFromPoints(vertex_indices[0], vertex_indices[1], vertex_indices[2]);

	// Handle quads.
	if(vertex_indices.size() == 4)
		instance->appendTriangleFromPoints(vertex_indices[2], vertex_indices[3], vertex_indices[0]);

	return true;
}

//************************************************************

bool ObjReader::readNotImplemented(const char*){
	return true;
}

//************************************************************

int main(){
	Obj model;
	ObjReader().readObjFile(model, "Sword/Scottish Sword.obj");

	std::cout << "Processing done\n\t"
	          << model.numberOfVertices() << " vertices read\n\t"
	          << model.numberOfFaces() << " faces read\n\n";
	
	const AABB &box = model.getAABB();
	std::cout << "AABB: [" << box.x_min << ", " << box.x_max << "]["
	                       << box.y_min << ", " << box.y_max << "]["
					       << box.z_min << ", " << box.z_max << "]\n";

	{
	JiMP2::BMP bitmap(600, 400);
	bitmap.projectObjXYForward(model, 0xaa, 0, 0);
	std::ofstream writer("project_xy_forw.bmp", std::ios::binary);
	writer << bitmap;
	writer.close();
	}{
	JiMP2::BMP bitmap(600, 400);
	bitmap.projectObjXZForward(model, 0xaa, 0, 0);
	std::ofstream writer("project_xz_forw.bmp", std::ios::binary);
	writer << bitmap;
	writer.close();
	}{
	JiMP2::BMP bitmap(600, 400);
	bitmap.projectObjYZForward(model, 0xaa, 0, 0);
	std::ofstream writer("project_yz_forw.bmp", std::ios::binary);
	writer << bitmap;
	writer.close();
	}

	return 0;
}
