#ifndef _MISC_H_
#define _MISC_H_

// Miscellaneous smaller classes:
// * Point3
// * Triangle
// * Obj

#include <iostream>
#include <vector>

typedef std::size_t index_t;

//************************************************************

class Point3{
public:
	Point3() = default;
	Point3(double x, double y, double z) : x(x), y(y), z(z) {}

	void setX(double v){ x = v; }
	void setY(double v){ y = v; }
	void setZ(double v){ z = v; }

	double getX() const { return x; }
	double getY() const { return y; }
	double getZ() const { return z; }

private:
	double x = {0.0},
	       y = {0.0},
	       z = {0.0};
};

//************************************************************

class Triangle{
public:
	Triangle() = default;
	Triangle(index_t a, index_t b, index_t c) : vertices{a, b, c} {}

	// Return index of n-th vertex in vertex array.
	index_t getVertexIndex(index_t i) const{
		// index_t is unsigned, no negative values are passed.
		if(i > 2)
			throw std::out_of_range("Invalid index while accessing triangle");
		return vertices[i];
	}

private:
	index_t vertices[3];
};

//************************************************************

class Obj{
public:
	Obj() = default;
	
	// Remove any existing faces and vertices.
	void clear(){
		vertices.clear();
		faces.clear();
	}

	// Add elements.
	void appendVertex(Point3 p){
		vertices.push_back(p);
	}

	void appendTriangleFromPoints(index_t p1, index_t p2, index_t p3){
		faces.push_back(Triangle(p1, p2, p3));
	}

	// Access elements.
	index_t numberOfVertices() const{
		return vertices.size();
	}
	index_t numberOfFaces() const{
		return faces.size();
	}
	const Point3& getVertex(index_t i) const{
		return vertices.at(i);
	}
	const Triangle& getFace(index_t i) const{
		return faces.at(i);
	}
	const Point3& getVertexOfFace(index_t triangle_idx, index_t vertex_idx){
		return vertices.at(faces.at(triangle_idx).getVertexIndex(vertex_idx));
	}

private:
	std::vector<Point3> vertices;
	std::vector<Triangle> faces;
};

//************************************************************

#endif  //_MISC_H_