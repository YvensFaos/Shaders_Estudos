#include "glbatch.h"

#include "edfile.h"

GLBatch::GLBatch(void)
{
	batchTestSize = 0;
}

GLBatch::GLBatch(char* batchPath, char* batchFilename)
{
	this->batchPath = new char[128];
	sprintf(this->batchPath, "%s", batchPath);

	this->bacthFilename = new char[128];
	sprintf(this->bacthFilename, "%s", bacthFilename);

	batchTestSize = 0;
}

GLBatch::~GLBatch(void)
{ }

std::vector<GLConfig> GLBatch::getTestBatch(void)
{
	char file[256];
	sprintf(file, "%s%s%s", this->batchPath, this->bacthFilename, BATCH_TYPE);

	EDFileReader reader = EDFileReader(file);
	char* line;

	std::vector<GLConfig> configurations;



	return configurations;
}