#include "bmp.h"
#include <iostream>
#include <cassert>
#include <fstream>
#include <cstring>

using namespace std;

//*** Auxilliary functions ***********************************

int negativeToZero(int x){
	return x < 0 ? 0 : x;
}



namespace JiMP2 {

//************************************************************

void BMP::projectObjXY(const Obj &object,
	unsigned char r, unsigned char g, unsigned char b){

	// Adjust image scaling.
	const AABB &aabb = object.getAABB();
	double scale_x = getWidth() / (double)(aabb.x_max - aabb.x_min);
	double scale_y = getHeight() / (double)(aabb.y_max - aabb.y_min);
	double scale = std::min(scale_x, scale_y); 

	// Translate the camera.
	float dx = -aabb.x_min,
	      dy = -aabb.y_min;

	// Draw each triangle.
	for(index_t i = 0; i != object.numberOfFaces(); ++i){
		Point3 points[3]{
			object.getVertexOfFace(i, 0),
			object.getVertexOfFace(i, 1),
			object.getVertexOfFace(i, 2)
		};

		// Draw a triangle.
		for(int i = 0; i != 3; ++i){
			int x0 = scale*(points[i%3].getX() + dx),
			    y0 = scale*(points[i%3].getY() + dy),
				x1 = scale*(points[(i+1)%3].getX() + dx),
				y1 = scale*(points[(i+1)%3].getY() + dy);

			drawLine(
				(uint16_t)negativeToZero(x0),
				(uint16_t)negativeToZero(y0),
				(uint16_t)negativeToZero(x1),
				(uint16_t)negativeToZero(y1),
				r, g, b
			);
		}
	}
}

//************************************************************

void BMP::projectObjXZ(const Obj &object,
	unsigned char r, unsigned char g, unsigned char b){
	
	// Adjust image scaling.
	const AABB &aabb = object.getAABB();
	double scale_x = getWidth() / (double)(aabb.x_max - aabb.x_min);
	double scale_z = getHeight() / (double)(aabb.z_max - aabb.z_min);
	double scale = std::min(scale_x, scale_z); 

	// Translate a camera.
	float dx = -aabb.x_min,
	      dz = -aabb.z_min;

	// Draw each triangle.
	for(index_t i = 0; i != object.numberOfFaces(); ++i){
		Point3 points[3]{
			object.getVertexOfFace(i, 0),
			object.getVertexOfFace(i, 1),
			object.getVertexOfFace(i, 2)
		};

		// Draw a triangle.
		for(int i = 0; i != 3; ++i){
			int x0 = scale*(points[i%3].getX() + dx),
			    z0 = scale*(points[i%3].getZ() + dz),
			    x1 = scale*(points[(i+1)%3].getX() + dx),
			    z1 = scale*(points[(i+1)%3].getZ() + dz);
			
			drawLine(
				(uint16_t)negativeToZero(x0),
				(uint16_t)negativeToZero(z0),
				(uint16_t)negativeToZero(x1),
				(uint16_t)negativeToZero(z1),
				r, g, b
			);

		}
	}
}

//************************************************************

void BMP::projectObjYZ(const Obj &object,
	unsigned char r, unsigned char g, unsigned char b){
	
	// Adjust image scaling.
	const AABB &aabb = object.getAABB();
	double scale_y = getWidth() / (double)(aabb.y_max - aabb.y_min);
	double scale_z = getHeight() / (double)(aabb.z_max - aabb.z_min);
	double scale = std::min(scale_y, scale_z); 

	// Translate a camera.
	float dy = -aabb.y_min,
	      dz = -aabb.z_min;

	// Draw each triangle.
	for(index_t i = 0; i != object.numberOfFaces(); ++i){
		Point3 points[3]{
			object.getVertexOfFace(i, 0),
			object.getVertexOfFace(i, 1),
			object.getVertexOfFace(i, 2)
		};

		// Draw a triangle.
		for(int i = 0; i != 3; ++i){
			int y0 = scale*(points[i%3].getY() + dy),
			    z0 = scale*(points[i%3].getZ() + dz),
			    y1 = scale*(points[(i+1)%3].getY() + dy),
			    z1 = scale*(points[(i+1)%3].getZ() + dz);
			
			drawLine(
				(uint16_t)negativeToZero(y0),
				(uint16_t)negativeToZero(z0),
				(uint16_t)negativeToZero(y1),
				(uint16_t)negativeToZero(z1),
				r, g, b
			);
		}
	}
}

//************************************************************

BMP::BMP(uint16_t width, uint16_t height) :
		bitmapCoreHeader(width, height) {

	assert(IS_LITTLE_ENDIAN);
	assert(width > 0);
	assert(height > 0);

	const unsigned int rowSize = ((bitmapCoreHeader.bitsPerPixel * width + 31)
			/ 32) * 4;
	const unsigned int imgSize = rowSize * height;

	bmpFileHeader.size = 14 + bitmapCoreHeader.size + imgSize;
	bmpFileHeader.dataOffset = 14 + bitmapCoreHeader.size;

	pixelData = new unsigned char[imgSize];
	std::memset(pixelData, 255, imgSize);
}

BMP::~BMP() {
	delete[] pixelData;
}

void BMP::setPixel(uint16_t x, uint16_t y, unsigned char r, unsigned char g,
		unsigned char b) {
	assert(bitmapCoreHeader.bitsPerPixel == 24);

	// Check if a pixel isn't outside a bitmap.
	if(x >= getWidth() || y >= getHeight())
		return;

	const size_t rowSize = ((bitmapCoreHeader.bitsPerPixel
			* bitmapCoreHeader.bmpWidth + 31) / 32) * 4;
	const size_t offset = rowSize * (bitmapCoreHeader.bmpHeight - y)
			+ x * (bitmapCoreHeader.bitsPerPixel / 8);

	pixelData[offset + 0] = b;
	pixelData[offset + 1] = g;
	pixelData[offset + 2] = r;
}

void BMP::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
	unsigned char r, unsigned char g, unsigned char b){
	
	int dx = abs(x1 - x0),
	    sx = x0 < x1 ? 1 : -1,
		dy = -abs(y1 - y0),
		sy = y0 < y1 ? 1 : -1;
	int err = dx + dy,
	    e2;
	
	while(true){
		setPixel(x0, y0, r, g, b);
		if(x0 == x1 && y0 == y1)
			break;
		e2 = 2*err;
		if(e2 >= dy){
			err += dy;
			x0 += sx;
		}
		if(e2 <= dx){
			err += dx;
			y0 += sy;
		}
	}
}

std::ostream& operator<<(std::ostream& os, const BMP& bmp) {
	os.write(bmp.bmpFileHeader.id, sizeof(bmp.bmpFileHeader.id));
	os.write((const char*) &bmp.bmpFileHeader.size,
			sizeof(bmp.bmpFileHeader.size));
	os.write(bmp.bmpFileHeader.reserved, sizeof(bmp.bmpFileHeader.reserved));
	os.write((const char*) &bmp.bmpFileHeader.dataOffset,
			sizeof(bmp.bmpFileHeader.dataOffset));

	os.write((const char*) &bmp.bitmapCoreHeader.size,
			sizeof(bmp.bitmapCoreHeader.size));
	os.write((const char*) &bmp.bitmapCoreHeader.bmpWidth,
			sizeof(bmp.bitmapCoreHeader.bmpWidth));
	os.write((const char*) &bmp.bitmapCoreHeader.bmpHeight,
			sizeof(bmp.bitmapCoreHeader.bmpHeight));
	os.write((const char*) &bmp.bitmapCoreHeader.colorPlanes,
			sizeof(bmp.bitmapCoreHeader.colorPlanes));
	os.write((const char*) &bmp.bitmapCoreHeader.bitsPerPixel,
			sizeof(bmp.bitmapCoreHeader.bitsPerPixel));

	const unsigned int rowSize = ((bmp.bitmapCoreHeader.bitsPerPixel
			* bmp.bitmapCoreHeader.bmpWidth + 31) / 32) * 4;
	const unsigned int imgSize = rowSize * bmp.bitmapCoreHeader.bmpHeight;

	os.write((const char*) bmp.pixelData, imgSize);

	return os;
}
}
