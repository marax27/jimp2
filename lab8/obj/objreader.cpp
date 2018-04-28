#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <vector>
#include <unordered_map>

#include "misc.h"
#include "bmp.h"
#include "objreader.h"

//************************************************************

std::ostream& operator<<(std::ostream &out, const Obj &model){
	out << "# Wavefront OBJ file\n";

	// Write vertices.
	for(index_t i = 0; i != model.numberOfVertices(); ++i){
		const Point3 &p = model.getVertex(i);
		out << "v " 
		    << p.getX() << ' '
			<< p.getY() << ' '
		    << p.getZ() << " \n";
	}

	// Write faces.
	for(index_t i = 0; i != model.numberOfFaces(); ++i){
		const Triangle &t = model.getFace(i);
		out << "f "
		    << t.getVertexIndex(0)+1 << ' '
		    << t.getVertexIndex(1)+1 << ' '
		    << t.getVertexIndex(2)+1 << " \n";
	}

	return out;
}

//************************************************************

void ObjReader::readObjFile(Obj &object, const std::string &fname){
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

		std::istringstream ss{line};
		std::string token;

		ss >> token;
		if(token.empty())
			continue;  //skip empty line

		if(token[0] == '#')
			continue;  //skip comments

		auto handler_itr = handlers.find(token);
		if(handler_itr == handlers.end())
			throw ObjParseException();

		if(!(this->*handler_itr->second)(line))
			throw ObjParseException();
		
		if(!reader.good())
			break;
	}

	reader.close();
}

//************************************************************

bool ObjReader::readVertex(const char *line){
	// Format: 'v' x y z
	double x, y, z;
	std::istringstream ss(line);
	std::string token;

	ss >> token >> x >> y >> z;
	if(ss.good()){
		token.clear();
		ss >> token;
		if(!token.empty())
			return false;
	}
	else if(ss.fail())
		return false;

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
	int v = 0;
	std::istringstream ss(line);
	std::string token;
	ss >> token;  // line identifier

	std::vector<int> vertex_indices;

	do{
		ss >> token;
		std::istringstream token_stream(token);

		if(ss.eof())
			break;
		
		token_stream >> v;
		// Ignore the rest of a token.

		if(token_stream.fail())
			return false;
		
		// Indices in Obj file are 1-based. Correct them.
		vertex_indices.push_back(v - 1);
	}while(ss.good());

	// Only triangles and quads are allowed.
	if(vertex_indices.size() != 3 && vertex_indices.size() != 4){
		std::cerr << "[Warning] Unsupported number of face's vertices: "
		          << vertex_indices.size() << ", omitting...\n";
		return true;  //omit face without stopping a program
	}

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

	try{
		ObjReader().readObjFile(model, "model.obj");
	}catch(ObjReader::ObjParseException&){
		std::cerr << "Failed to read a file.\n";
		return 1;
	}

	std::cout << "Processing done\n\t"
	          << model.numberOfVertices() << " vertices read\n\t"
	          << model.numberOfFaces() << " faces read\n\n";
	
	const AABB &box = model.getAABB();
	std::cout << "AABB: [" << box.x_min << ", " << box.x_max << "]["
	                       << box.y_min << ", " << box.y_max << "]["
					       << box.z_min << ", " << box.z_max << "]\n";
	
	JiMP2::BMP bitmapxy(720, 480);
	bitmapxy.projectObjXY(model, 0xaa, 0, 0);
	std::ofstream writerxy("project_xy_forw.bmp", std::ios::binary);
	writerxy << bitmapxy;
	writerxy.close();

	JiMP2::BMP bitmapxz(720, 480);
	bitmapxz.projectObjXZ(model, 0xaa, 0, 0);
	std::ofstream writerxz("project_xz_forw.bmp", std::ios::binary);
	writerxz << bitmapxz;
	writerxz.close();

	JiMP2::BMP bitmapyz(720, 480);
	bitmapyz.projectObjYZ(model, 0xaa, 0, 0);
	std::ofstream writeryz("project_yz_forw.bmp", std::ios::binary);
	writeryz << bitmapyz;
	writeryz.close();

	return 0;
}
