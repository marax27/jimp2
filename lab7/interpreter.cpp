#include "interpreter.h"
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>

//************************************************************

void Interpreter::analyze(const std::string &filename, Drawer &drawer){
	std::ifstream reader(filename, std::ios::binary);
	if(!reader)
		throw FileNotOpen(filename);

	internal_drawer = &drawer;
	
	std::unordered_map<std::string, void(Interpreter::*)(const char*)> handlers({
		// Basic shapes.
		{"L", &Interpreter::readLine},
		{"C", &Interpreter::readCircle},
		{"D", &Interpreter::readDisk},
		{"A", &Interpreter::readArc},
		{"S", &Interpreter::readCircularSector},
		
		// Additional shapes.
		{"E", &Interpreter::readEllipse},
		{"R", &Interpreter::readRectangle},
		{"P", &Interpreter::readRegularPolygon}
	});
	
	this->line_no = 0;
	while(reader.good()){
		++line_no;
		const int BUFSIZE = 256;
		char line[BUFSIZE];
		reader.getline(line, BUFSIZE, '\n');

		std::size_t len = strlen(line);

		// Handle CRLF format.
		if(line[len-1] == '\r')
			line[(len--)-1] = '\0';
		
		;

		char token[16]{};
		if(sscanf(line, "%15s", token) != 1)
			continue;  //skip empty line

		if(token[0] == '#')
			continue;  //skip comments

		this->current_instruction = token;

		auto handler_itr = handlers.find(token);
		if(handler_itr == handlers.end())
			throw UnknownInstruction(token, line_no);
		
		(this->*handler_itr->second)(line);

		if(!reader.good())
			break;
	}

	reader.close();
	internal_drawer = nullptr;
}

//************************************************************

void Interpreter::readLine(const char *line){
	const int ARGCOUNT = 7;
	std::istringstream ss{line};
	std::string token;
	Point a, b;
	Colour c;

	e_ProcessingStatus err;
	fp_t args[ARGCOUNT];

	// Read function arguments.
	for(int i = 0; i != ARGCOUNT; ++i){
		err = readFromStream(ss, a.x);
		if(err == EOL)
			throw InvalidArgCount(current_instruction, line_no, i, ARGCOUNT);
		else if(err == FAIL)
			throw InvalidArgType(current_instruction, line_no);
	}

	// ss.good() indicates there's still content in stream.
	// This might mean someone passed too many arguments.
	if(ss.good()){
		int additional = 0;
		std::string x;
		while(ss.good()){
			ss >> x;
			++additional;
		}
		throw InvalidArgCount(
			current_instruction, line_no, ARGCOUNT+additional, ARGCOUNT);
	}

	a = {args[0], args[1]};
	b = {args[2], args[3]};
	for(int i = 4; i != ARGCOUNT; ++i){
		if(!in(0, args[i], 256))
			throw InvalidArgType(current_instruction, line_no);
	}
	c = {args[4], args[5], args[6]};

	// Draw an object.

}

//************************************************************

void Interpreter::readCircle(const char *line){
	Point s;
	uint16_t r;
	Colour c;
}

//************************************************************

void Interpreter::readDisk(const char *line){
	Point s;
	uint16_t r;
	Colour c;
}

//************************************************************

void Interpreter::readArc(const char *line){
	Point s;
	uint16_t r;
	fp_t a1, a2;
	Colour c;
}

//************************************************************

void Interpreter::readCircularSector(const char *line){
	Point s;
	uint16_t r;
	fp_t a1, a2;
	Colour c;
}

//************************************************************

void Interpreter::readEllipse(const char *line){
	Point s;
	uint16_t a, b;
	Colour c;
}

//************************************************************

void Interpreter::readRectangle(const char *line){
	Point a, b;
	Colour c;
}

//************************************************************

void Interpreter::readRegularPolygon(const char *line){
	Point s;
	int n;
	fp_t side;
	Colour c;
}

//************************************************************
