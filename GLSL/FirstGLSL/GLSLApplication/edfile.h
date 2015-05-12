#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

class EDFileReader
{
public:
	int linesRead;
private:
	FILE* fileHandle;
	int bufLen;
	int bufPos;
	int bufSize;
	int lineLen;
	char* buffer;
	long fileSize;
	long filePos;
	char line[255];
private:
	void readBuf(void);
	int readLn(void);
public:
	EDFileReader(const char* fileName);
	~EDFileReader(void);
	bool eof(void);
	char* readLnStr(void);
	int readLnInt(void);
	float readLnFloat(void);
	void close(void);
};

class EDFileWriter
{
private:
	FILE* file;
	char* buffer;
	int bufSize;
	int bufLen;
	void flush(void);
	void checkFlush(int bufInc);
	void write(char* s);
public:
	EDFileWriter(const char* fileName);
	~EDFileWriter(void);
	void writeLnStr(char* line);
	void writeLnInt(int i);
	void writeLnFloat(float f);
	void write(const void * ptr, size_t size, size_t count);
	void close(void);
};