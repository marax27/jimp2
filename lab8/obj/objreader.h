#ifndef _OBJREADER_H_
#define _OBJREADER_H_

#include <sstream>
#include "misc.h"

class ObjReader{
public:
	void readObjFile(Obj &object, const std::string &fname);

	// Interface for more specific exceptions.
	class ObjParseException : public std::exception {
	public:
		virtual const char* what() const noexcept{
			return message.c_str();
		}
	protected:
		void setMessage(const std::string &msg){ message = msg; }
	private:
		std::string message;
	};

	// Not open - file cannot be opened.
	class ObjNotOpen : public ObjParseException{
	public:
		ObjNotOpen(const std::string &filename){
			std::stringstream ss;
			ss << "Failed to open OBJ file: '" << filename << "'.";
			setMessage(ss.str());
		}
	};

	// Invalid line - when instruction receives invalid set of arguments.
	class ObjInvalidLine : public ObjParseException{
	public:
		ObjInvalidLine(uint32_t line_number){
			std::stringstream ss;
			ss << "Failed to process line " << line_number;
			setMessage(ss.str());
		}
	};

	// Unknown instruction - non-standard OBJ instruction.
	class ObjUnknownInstruction : public ObjParseException{
	public:
		ObjUnknownInstruction(uint32_t line_number, const std::string &name){
			std::stringstream ss;
			ss << "Unknown instruction on line " << line_number << ": '" << name << "'.";
			setMessage(ss.str());
		}
	};

private:
	bool readVertex(const char *line);
	bool readFace(const char *line);
	bool readNotImplemented(const char*);

	Obj *instance;
};

std::ostream& operator<<(std::ostream &out, const Obj &model);

#endif  //_OBJREADER_H_