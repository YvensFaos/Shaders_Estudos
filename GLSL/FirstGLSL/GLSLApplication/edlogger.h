#ifndef __EDLOGGER__
#define __EDLOGGER__

#include "edfile.h"
#include "glconfig.h"

class EDLogger
{
private:
	EDFileWriter* writer;

public:
	EDLogger(char* filename);
	~EDLogger();

	void logLine(char* line);
	void closeLog(void);
};

class EDPrinter
{
public:
	EDPrinter(void);
	~EDPrinter(void);

	void printScreen(GLConfig* config, char* printName);
};

#endif