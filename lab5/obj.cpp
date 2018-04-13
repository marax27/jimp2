#include <exception>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <vector>
#include <unordered_map>

//************************************************************

struct Point3{
	Point3() = default;
	Point3(double x, double y, double z) : x(x), y(y), z(z) {}

	double x = {0.0}, y = {0.0}, z = {0.0};
};

struct Triangle{
	Triangle() = default;
	Triangle(Point3 a, Point3 b, Point3 c) : p1(a), p2(b), p3(c) {}

	Point3 p1, p2, p3;
};

//************************************************************

class Obj{
public:
	Obj(const std::vector<Triangle> &face_list) : faces(face_list) {}

private:
	// std::vector<Point3> points;
	std::vector<Triangle> faces;
};

//************************************************************

class ObjReader{
public:
	Obj readObjFile(const std::string &fname);

	class ObjParseException : std::exception {};

private:
	bool readVertex(const char *line);
	bool readFace(const char *line);
	bool readNotImplemented(const char*);

	std::vector<Point3> vertices;
	std::vector<Triangle> faces;
};

Obj ObjReader::readObjFile(const std::string &fname){
	vertices.resize(0);

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
	
	//for(; reader.good(); ++line_number){
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

	std::cerr << "Processing done\n\t" << vertices.size()
	          << " vertices read\n\t" << faces.size() << " faces read\n";

	reader.close();
	return Obj(faces);
}

//************************************************************

bool ObjReader::readVertex(const char *line){
	// Format: 'v' x y z
	Point3 p;
	if(sscanf(line, "v %lf %lf %lf", &p.x, &p.y, &p.z) != 3){
		std::cerr << "[Error] Unsupported vertex format: '" << line << "'\n";
		return false;
	}
	vertices.push_back(p);
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

	Triangle t(
		vertices[vertex_indices[0]],
	    vertices[vertex_indices[1]],
		vertices[vertex_indices[2]]
	);
	faces.push_back(std::move(t));

	// Handle quads.
	if(vertex_indices.size() == 4){
		Triangle u(
			vertices[vertex_indices[2]],
			vertices[vertex_indices[3]],
			vertices[vertex_indices[0]]
		);
		faces.push_back(std::move(u));
	}
	return true;
}

//************************************************************

bool ObjReader::readNotImplemented(const char*){
	return true;
}

//************************************************************

int main(){
	ObjReader reader;
	Obj model = reader.readObjFile("Sword/Scottish Sword.obj");



	return 0;
}
