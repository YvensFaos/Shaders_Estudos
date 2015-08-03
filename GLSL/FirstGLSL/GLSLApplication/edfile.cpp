#include "edfile.h"

EDFileReader::EDFileReader(const char* fileName)
{
	//Cria as instâncias necessárias para a leitura do arquivo
	//rb -> read as binary file (?)
	fileHandle = fopen(fileName, "rb");

	fseek(fileHandle, 0, SEEK_END);
	fileSize = ftell(fileHandle);
	filePos = 0;
	fseek(fileHandle, filePos, SEEK_SET);
	//2^13
	bufSize = 8192;
	buffer = new char[bufSize];
	bufLen = 0;
	bufPos = 0;

	linesRead = 0;
}

EDFileReader::~EDFileReader()
{
	close();
	delete buffer;
}

void EDFileReader::readBuf()
{
	bufLen = fread(buffer, 1, bufSize, fileHandle);
	bufPos = 0;
	filePos += bufLen;
}

bool EDFileReader::eof()
{
	bool hasBuf = bufPos < bufLen;
	bool hasFile = filePos < fileSize;
	return !(hasBuf || hasFile);
}

void EDFileReader::close()
{
	if (fileHandle)
	{
		fclose(fileHandle);
		fileHandle = 0;
	}
	fileSize = 0;
	filePos = 0;
	bufLen = 0;
	bufPos = 0;
}

int EDFileReader::readLn()
{
	//Método para ler uma linha do arquivo e apontar para a linha seguinte
	lineLen = 0;
	bool cr = false;
	while (true)
	{
		char ch;
	READ_CHAR_FROM_BUFFER:
		if (bufPos < bufLen)
		{
			ch = buffer[bufPos++];
		}
		else if (filePos < fileSize)
		{
			readBuf();
			goto READ_CHAR_FROM_BUFFER; //goto?!
		}
		else{
			break;
		}

		//ch == 0  = NUL	0	0	0	^@	Null character
		//ch == 26 = SUB	32	26	1a	^Z	Substitute
		//ch == 10 = LF	    12	10	a	^J	Line Feed
		if (ch == 0 || ch == 26 || ch == 10){
			break;
		}

		if (cr){
			break;
		}

		//ch == 13  = CR	15	13	d	^M	Carriage Return
		if (ch == 13){
			cr = true;
		}
		else{
			line[lineLen++] = ch;
		}
	}
	line[lineLen] = 0x0;
	linesRead++;
	return lineLen;
}

char* EDFileReader::readLnStr()
{
	readLn();
	return &line[0];
}

int EDFileReader::readLnInt()
{
	readLn();
	return atoi(line);
}

float EDFileReader::readLnFloat()
{
	readLn();
	return (float)atof(line);
}

EDFileWriter::EDFileWriter(const char* fileName)
{
	file = fopen(fileName, "wb");
	bufSize = 16384;
	buffer = new char[bufSize];
	bufLen = 0;
}

EDFileWriter::~EDFileWriter()
{
	if (file)
	{
		close();
	}
}

void EDFileWriter::flush()
{
	if (bufLen > 0)
	{
		fwrite(buffer, 1, bufLen, file);
		bufLen = 0;
	}
}

void EDFileWriter::checkFlush(int bufInc)
{
	if (bufLen + bufInc > bufSize)
	{
		flush();
	}
}

void EDFileWriter::write(char* s)
{
	if (s == NULL)
	{
		return;
	}
	int len = strlen(s);
	if (len > 0)
	{
		checkFlush(len);
		memcpy(&buffer[bufLen], s, len);
		bufLen += len;
	}
}

void EDFileWriter::writeLnStr(char* line)
{
	write(line);
	write("\r\n");
}

void EDFileWriter::writeLnInt(int i)
{
	char buf[32];
	sprintf(buf, "%d", i);
	writeLnStr(buf);
}

void EDFileWriter::writeLnFloat(float f)
{
	char buf[32];
	sprintf(buf, "%f", f);
	writeLnStr(buf);
}

void EDFileWriter::close()
{
	flush();
	fclose(file);
	file = NULL;
}

void EDFileWriter::write(const void * ptr, size_t size, size_t count)
{
	fwrite(ptr, size, count, file);
}