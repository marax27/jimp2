#include "interpreter.h"
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>

//************************************************************

bool isRGBValid(fp_t r, fp_t g, fp_t b){
	return in(0, r, 256) &&
	       in(0, g, 256) &&
	       in(0, b, 256);
}

//************************************************************

void Interpreter::analyze(const std::string &filename, Drawer &drawer){
	std::ifstream reader(filename, std::ios::binary);
	if(!reader)
		throw FileNotOpen(filename);

	internal_drawer = &drawer;
	
	std::unordered_map<
		std::string,
		void(Interpreter::*)(const std::vector<fp_t>&)> handlers({

		// Basic shapes.
		{"L", &Interpreter::processLine},
		{"C", &Interpreter::processCircle},
		{"D", &Interpreter::processDisk},
		{"A", &Interpreter::processArc},
		{"S", &Interpreter::processCircularSector},
		
		// Additional shapes.
		{"E", &Interpreter::processEllipse},
		{"R", &Interpreter::processRectangle},
		{"P", &Interpreter::processRegularPolygon}
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
		
		std::istringstream ss{line};
		std::string token;

		ss >> token;
		if(token.empty())
			continue;  //skip empty line

		if(token[0] == '#')
			continue;  //skip comments

		this->current_instruction = token;

		auto handler_itr = handlers.find(token);
		if(handler_itr == handlers.end())
			throw UnknownInstruction(token, line_no);
		
		// Read all arguments.
		std::vector<fp_t> args;
		fp_t f;
		e_ProcessingStatus err = GOOD;
		while(!err){
			err = readFromStream(ss, f);
			if(err != FAIL)
				args.push_back(f);
			else
				throw InvalidArgType(this->current_instruction, line_no);
		}

		// Execute instruction - draw an object.
		(this->*handler_itr->second)(args);

		if(!reader.good())
			break;
	}

	reader.close();
	internal_drawer = nullptr;
}

//************************************************************

void Interpreter::processLine(const std::vector<fp_t> &args){
	const int ARGCOUNT = 7;
	Point a, b;
	Colour c;

	if(args.size() != ARGCOUNT)
		throw InvalidArgCount(
			this->current_instruction,
			this->line_no,
			ARGCOUNT,
			args.size()
		);
	
	a = Point(args[0], args[1]);
	b = Point(args[2], args[3]);
	
	if(!isRGBValid(args[4], args[5], args[6]))
		throw InvalidArgType(this->current_instruction, this->line_no);
	c = Colour(args[4], args[5], args[6]);

	internal_drawer->drawLine(a, b, c);
}

//************************************************************

void Interpreter::processCircle(const std::vector<fp_t> &args){
	const int ARGCOUNT = 6;
	Point s;
	uint16_t r;
	Colour c;

	if(args.size() != ARGCOUNT)
		throw InvalidArgCount(
			this->current_instruction,
			this->line_no,
			ARGCOUNT,
			args.size()
		);
	
	s = Point(args[0], args[1]);
	
	if(args[2] < 0.0f)
		throw InvalidArgType(this->current_instruction, this->line_no);
	r = args[2];

	if(!isRGBValid(args[3], args[4], args[5]))
		throw InvalidArgType(this->current_instruction, this->line_no);
	c = Colour(args[3], args[4], args[5]);

	internal_drawer->drawCircle(s, r, c);
}

//************************************************************

void Interpreter::processDisk(const std::vector<fp_t> &args){
	const int ARGCOUNT = 6;
	Point s;
	uint16_t r;
	Colour c;

	if(args.size() != ARGCOUNT)
		throw InvalidArgCount(
			this->current_instruction,
			this->line_no,
			ARGCOUNT,
			args.size()
		);
	
	s = Point(args[0], args[1]);
	
	if(args[2] < 0.0f)
		throw InvalidArgType(this->current_instruction, this->line_no);
	r = args[2];

	if(!isRGBValid(args[3], args[4], args[5]))
		throw InvalidArgType(this->current_instruction, this->line_no);
	c = Colour(args[3], args[4], args[5]);

	internal_drawer->drawDisk(s, r, c);
}

//************************************************************

void Interpreter::processArc(const std::vector<fp_t> &args){
	const int ARGCOUNT = 8;
	Point s;
	uint16_t r;
	fp_t a1, a2;
	Colour c;

	if(args.size() != ARGCOUNT)
		throw InvalidArgCount(
			this->current_instruction,
			this->line_no,
			ARGCOUNT,
			args.size()
		);
	
	s = Point(args[0], args[1]);
	
	if(args[2] < 0.0f)
		throw InvalidArgType(this->current_instruction, this->line_no);
	r = args[2];

	a1 = args[3];
	a2 = args[4];

	if(!isRGBValid(args[5], args[6], args[7]))
		throw InvalidArgType(this->current_instruction, this->line_no);
	c = Colour(args[5], args[6], args[7]);

	internal_drawer->drawArc(s, r, a1, a2, c);
}

//************************************************************

void Interpreter::processCircularSector(const std::vector<fp_t> &args){
	const int ARGCOUNT = 8;
	Point s;
	uint16_t r;
	fp_t a1, a2;
	Colour c;

	if(args.size() != ARGCOUNT)
		throw InvalidArgCount(
			this->current_instruction,
			this->line_no,
			ARGCOUNT,
			args.size()
		);
	
	s = Point(args[0], args[1]);
	
	if(args[2] < 0.0f)
		throw InvalidArgType(this->current_instruction, this->line_no);
	r = args[2];

	a1 = args[3];
	a2 = args[4];

	if(!isRGBValid(args[5], args[6], args[7]))
		throw InvalidArgType(this->current_instruction, this->line_no);
	c = Colour(args[5], args[6], args[7]);

	internal_drawer->drawCircularSector(s, r, a1, a2, c);
}

//************************************************************

void Interpreter::processEllipse(const std::vector<fp_t> &args){
	const int ARGCOUNT = 7;
	Point s;
	uint16_t a, b;
	Colour c;

	if(args.size() != ARGCOUNT)
		throw InvalidArgCount(
			this->current_instruction,
			this->line_no,
			ARGCOUNT,
			args.size()
		);
	
	s = Point(args[0], args[1]);

	if(args[2] < 0.0f || args[3] < 0.0f)
		throw InvalidArgType(this->current_instruction, this->line_no);
	a = args[2];
	b = args[3];

	if(!isRGBValid(args[4], args[5], args[6]))
		throw InvalidArgType(this->current_instruction, this->line_no);
	c = Colour(args[4], args[5], args[6]);

	internal_drawer->drawEllipse(s, a, b, c);
}

//************************************************************

void Interpreter::processRectangle(const std::vector<fp_t> &args){
	const int ARGCOUNT = 7;
	Point a, b;
	Colour c;

	if(args.size() != ARGCOUNT)
		throw InvalidArgCount(
			this->current_instruction,
			this->line_no,
			ARGCOUNT,
			args.size()
		);
	
	a = Point(args[0], args[1]);
	b = Point(args[2], args[3]);

	if(!isRGBValid(args[4], args[5], args[6]))
		throw InvalidArgType(this->current_instruction, this->line_no);
	c = Colour(args[4], args[5], args[6]);

	internal_drawer->drawRectangle(a, b, c);
}

//************************************************************

void Interpreter::processRegularPolygon(const std::vector<fp_t> &args){
	const int ARGCOUNT = 7;
	Point s;
	int n;
	fp_t side;
	Colour c;

	if(args.size() != ARGCOUNT)
		throw InvalidArgCount(
			this->current_instruction,
			this->line_no,
			ARGCOUNT,
			args.size()
		);
	
	s = Point(args[0], args[1]);
	n = args[2];
	side = args[3];

	if(!isRGBValid(args[4], args[5], args[6]))
		throw InvalidArgType(this->current_instruction, this->line_no);
	c = Colour(args[4], args[5], args[6]);

	internal_drawer->drawRegularPolygon(s, n, side, c);
}

//************************************************************
