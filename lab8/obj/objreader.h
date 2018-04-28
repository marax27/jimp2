#ifndef _OBJREADER_H_
#define _OBJREADER_H_

#include "misc.h"

class ObjReader{
public:
	void readObjFile(Obj &object, const std::string &fname);

	class ObjParseException : std::exception {};

private:
	bool readVertex(const char *line);
	bool readFace(const char *line);
	bool readNotImplemented(const char*);

	Obj *instance;
};

#endif  //_OBJREADER_H_