#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include <string>
#include <vector>
#include <sstream>
#include "drawer.h"

class Interpreter{
public:
	Interpreter() = default;

	// Load and interpret a file.
	void analyze(const std::string &filename, Drawer &drawer);

	//------------------------------
	class InterpreterException : public std::exception {};

	class FileNotOpen : public InterpreterException{
	public:
		FileNotOpen(const std::string &filename)
			: fn(filename) {}
		virtual const char* what() const noexcept{
			std::ostringstream ss;
			ss << "Failed to open '" << fn << "'.";
			return ss.str().c_str();
		}
	private:
		std::string fn;
	};

	// Line-specific errors.
	class InstructionException : public InterpreterException{
	public:
		InstructionException(
			const std::string &instruction, int line_number)
			: instr(instruction), line_no(line_number) {}
	protected:
		std::string instr;
		int line_no;
	};

	class UnknownInstruction : public InstructionException{
	public:
		UnknownInstruction(
			const std::string &instruction, int line_number)
			: InstructionException(instruction, line_number) {}
		
		virtual const char* what() const noexcept{
			std::ostringstream ss;
			ss << "Unknown instruction on line " << line_no << ": '" << instr << "'.";
			return ss.str().c_str();
		}
	};

	class InvalidArgCount : public InstructionException{
	public:
		InvalidArgCount(const std::string &instruction, int line_number,
			int provided, int expected)
			: InstructionException(instruction, line_number), prov(provided), exp(expected) {}
		
		virtual const char* what() const noexcept{
			std::ostringstream ss;
			ss << "Instruction '" << instr 
			   << "' on line " << line_no
			   << ": invalid number of arguments ("
			   << exp << " expected, "
			   << prov << " provided).";
			return ss.str().c_str();
		}
	private:
		int prov, exp;
	};

	class InvalidArgType : public InstructionException{
	public:
		InvalidArgType(const std::string &instruction, int line_number)
			: InstructionException(instruction, line_number) {}

		virtual const char* what() const noexcept{
			std::ostringstream ss;
			ss << "Instruction '" << instr 
			   << "' on line " << line_no
			   << ": invalid argument type.";
			return ss.str().c_str();
		}
	private:
		int prov, exp;
	};
	//------------------------------

private:
	void processLine(const std::vector<fp_t> &args);
	void processCircle(const std::vector<fp_t> &args);
	void processDisk(const std::vector<fp_t> &args);
	void processArc(const std::vector<fp_t> &args);
	void processCircularSector(const std::vector<fp_t> &args);

	void processEllipse(const std::vector<fp_t> &args);
	void processRectangle(const std::vector<fp_t> &args);
	void processRegularPolygon(const std::vector<fp_t> &args);

	Drawer *internal_drawer;

	int line_no;
	std::string current_instruction;

	// Read a value of type T from a stringstream. If there's not enough
	// values in stream, return EOL. If value types don't match,
	// return FAIL. GOOD if everything is fine.
	enum e_ProcessingStatus{ GOOD = 0, EOL, FAIL };
	template<typename T>
		e_ProcessingStatus readFromStream(std::istringstream &ss, T &x){
			ss >> x;
			if(ss.fail())
				return FAIL;
			else if(ss.eof())
				return EOL;
			return GOOD;
		}
};

#endif  //_INTERPRETER_H_