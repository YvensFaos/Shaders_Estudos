#pragma once

#include "edfile.h"
#include "glconfig.h"
#include <time.h>

#define LOG_EXTENSION ".txt"

class EDLogger
{
private:
	EDFileWriter* writer;
	time_t ltime;
	struct tm *Tm;

public:
	EDLogger(void);
	EDLogger(char* filename);
	~EDLogger(void);

	void logLine(char* line);
	void logLineTimestamp(char* line);
	void closeLog(void);
};

class EDPrinter
{
public:
	EDPrinter(void);
	~EDPrinter(void);

	void printScreen(GLConfig* config, char* printName);
};