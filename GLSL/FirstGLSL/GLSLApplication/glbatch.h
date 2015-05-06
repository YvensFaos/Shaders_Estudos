#pragma once

#include "glplayer.h"

#include <vector>

//GL Batch File = gbf
#define BATCH_TYPE ".gbf"

class GLBatch
{
private:
	GLPlayer* player;
	char* batchPath;
	char* bacthFilename;
public:
	int batchTestSize;

	GLBatch(void);
	GLBatch(char* batchPath, char* batchFilename);
	~GLBatch(void);

	std::vector<GLConfig> getTestBatch(void);

private:
	EDE_TYPE getEdetype(char* edename);
};